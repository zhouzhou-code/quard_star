/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* 调度器包含文件。 */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "riscv_sbi.h"
#include <string.h>

extern void xPortStartFirstTask( void );
/*-----------------------------------------------------------*/

/* 允许用户覆盖初始 RA 的预加载。 */
#ifdef configTASK_RETURN_ADDRESS
    #define portTASK_RETURN_ADDRESS    configTASK_RETURN_ADDRESS
#else
    #define portTASK_RETURN_ADDRESS    0
#endif

/* 中断栈
 * 如果定义了configISR_STACK_SIZE_WORDS，则使用静态分配的数组作为中断栈。
 * 否则，如果未定义，则需要更新链接脚本，声明名为 __freertos_irq_stack_top 的变量，
 * 其值等于 main 函数使用的栈顶。
 */
#ifdef configISR_STACK_SIZE_WORDS
    /* 
     * 如果在 FreeRTOSConfig.h 中定义了 configISR_STACK_SIZE_WORDS，
     * 则在此处静态分配中断栈。
     * 
     * __attribute__((aligned(16)))：
     * RISC-V 架构要求栈指针必须保持 16 字节对齐（128位）。
     * 这确保了 xISRStack 数组的起始地址满足这一硬件要求。
     */
    static __attribute__( ( aligned( 16 ) ) ) StackType_t xISRStack[ configISR_STACK_SIZE_WORDS ] = { 0 };
     /*
    * xISRStackTop 计算栈的顶端地址（最高地址）。
    * 因为 RISC-V 的栈是向下增长的（从高地址向低地址）。
    *
    * & ~portBYTE_ALIGNMENT_MASK：
    * 这是一个位掩码操作，用于确保计算出的栈顶地址严格满足对齐要求。
    * 即使 configISR_STACK_SIZE_WORDS 导致的大小不是对齐倍数，
    * 这个操作也会将地址向下舍入到最近的对齐边界，防止非对齐访问故障。
    */
    const StackType_t xISRStackTop = ( StackType_t ) &( xISRStack[ configISR_STACK_SIZE_WORDS & ~portBYTE_ALIGNMENT_MASK ] );
    #define portISR_STACK_FILL_BYTE	0xee
#else
    /*
    * 如果未定义 configISR_STACK_SIZE_WORDS，则假设中断栈在链接脚本中定义。
    * 
    * __freertos_irq_stack_top 是链接脚本中导出的符号。
    * 这种方式允许更灵活的内存布局，例如将中断栈放置在特定的内存段（如 TCM 或 片内 SRAM）。
    */
    extern const uint32_t __freertos_irq_stack_top[];
    const StackType_t xISRStackTop = ( StackType_t ) __freertos_irq_stack_top;
#endif

/*
 * 设置定时器以生成滴答中断。
 * 此文件中的实现为弱定义，允许应用程序重写。
 */
void vPortSetupTimerInterrupt( void ) __attribute__( ( weak ) );

/*-----------------------------------------------------------*/

/* 用于编程mtimer比较寄存器。 */
uint64_t ullNextTime = 0ULL;
const uint64_t * pullNextTime = &ullNextTime;
/* 
 * 计算产生一个 FreeRTOS 系统滴答（Tick）所需的硬件定时器计数值。
 * 
 * 公式：uxTimerIncrementsForOneTick = 硬件定时器频率 / 系统滴答频率
 * 
 * 【重要提示】：
 * 在 RISC-V 架构中，mtime/time 寄存器的时钟频率（RTC/Timer频率）可能与 CPU 核心频率不同。
 * 如果它们不同，configCPU_CLOCK_HZ 必须被定义为 **Timer 的频率**，
 * 而不是 CPU 核心的频率，否则系统时间将会不准确。
 */const size_t uxTimerIncrementsForOneTick = ( size_t ) ( ( configCPU_CLOCK_HZ ) / ( configTICK_RATE_HZ ) ); /* Assumes increment won't go over 32-bits. */
volatile uint64_t * pullMachineTimerCompareRegister = NULL; /* S-Mode 不直接访问寄存器，此变量可能仅用于兼容性或特定实现 */

/* 保存临界区嵌套值 - 启动时故意非零，以确保在调度器启动前不会意外启用中断。 */
size_t xCriticalNesting = ( size_t ) 0xaaaaaaaa;
size_t * pxCriticalNesting = &xCriticalNesting;

/* 用于捕获尝试从实现函数返回的任务。 */
size_t xTaskReturnAddress = ( size_t ) portTASK_RETURN_ADDRESS;

/*-----------------------------------------------------------*/

/**
 * @brief 通过sbi调用 设置定时器中断
 *
 * 使用SBI调用(sbi_set_timer)设置下一次中断时间,
 * 在发生timer中断后调用vPortSetupTimerInterrupt重新设置下一次中断时间
 */
void vPortSetupTimerInterrupt( void )
{
    /* 获取mtimer当前计数cnt */
    uint64_t ullCurrentTime;
    __asm volatile( "rdtime %0" : "=r"( ullCurrentTime ) );

    /* 计算下一次中断时间=当前定时器cnt+一个tick的count */
    ullNextTime = ullCurrentTime + ( uint64_t ) uxTimerIncrementsForOneTick;

    /* 通过SBI设置定时器的比较值 */
    sbi_set_timer( ullNextTime );
}

/*-----------------------------------------------------------*/


/**
 * @brief 启动调度器。
 * 
 * 此函数由 vTaskStartScheduler() 调用，标志着 FreeRTOS 内核接管系统的控制权。
 * 它是多任务处理的起点。
 *
 * **功能描述 (完整移植版):**
 * 在完整的移植中，此函数必须完成硬件相关的初始化工作：
 * 1. 配置系统 Tick 定时器（在 RISC-V S 模式下通常使用 SBI 调用 sbi_set_timer）。
 * 2. 配置并启用 S 模式下的中断（设置 sie 寄存器，启用定时器中断和软件中断）。
 * 3. 将中断向量表基地址 (stvec) 指向我们编写的 trap handler。
 * 4. 调用汇编函数 xPortStartFirstTask() 来恢复第一个任务的上下文并跳转执行。
 * 
 * **注意**: 一旦成功，此函数**不应**返回，因为控制权已移交给任务。
 *
 * @return BaseType_t 如果启动失败返回 pdFALSE，否则不应返回。
 */
BaseType_t xPortStartScheduler( void )
{
    #if ( configASSERT_DEFINED == 1 )
    {
        /* Check alignment of the interrupt stack - which is the same as the
         * stack that was being used by main() prior to the scheduler being
         * started. */
        configASSERT( ( xISRStackTop & portBYTE_ALIGNMENT_MASK ) == 0 );

        #ifdef configISR_STACK_SIZE_WORDS
        {
            memset( ( void * ) xISRStack, portISR_STACK_FILL_BYTE, sizeof( xISRStack ) );
        }
        #endif /* configISR_STACK_SIZE_WORDS */
    }
    #endif /* configASSERT_DEFINED */

    // 设置定时器中断
    vPortSetupTimerInterrupt();

    /* 使能SIE中S模式Timer中断和Soft中断，注意此处使能并不会立即响应
	xPortStartFirstTask中将打开全局使能 */
    csr_set(CSR_SIE, SIP_STIP); //设置SIE寄存器的STIP/E位，使能S模式下的Timer中断
    csr_set(CSR_SIE, SIP_SSIP); //设置SIE寄存器的SSIP/E位，使能S模式下的Soft中断

    //启动第一个任务
    xPortStartFirstTask();

    /* Should not get here as after calling xPortStartFirstTask() only tasks
     * should be executing. */
    return pdFAIL;
}
/*-----------------------------------------------------------*/
/**
 * @brief 清除IPI中断标志位。
 * 
 * 用于清除SIE寄存器中的SSIP位，从而清除IPI中断标志位。
 *
 * **功能描述:**
 * 当IPI中断发生时，会设置SIE寄存器中的SSIP位。
 * 此函数用于清除该位，以准备处理下一个IPI中断。
 */
void vPortClearIpiInterrupt( void )
{
    /* 直接通过清除SIP寄存器的SSIP位，清除IPI中断标志位 
    而非通过sbi_clear_ipi_interrupt()函数清除 (没必要陷入M模式清除，本来就可以在S模式下写SIP来清除)*/
    csr_clear(CSR_SIP, SIP_SSIP);
}

/**
 * @brief 结束调度器。
 * 
 * 用于停止调度器。
 *
 * **功能描述:**
 * 在嵌入式系统中，调度器通常不会停止。如果在 PC 模拟器上运行，
 * 这里可能包含释放资源并退出的代码。
 *
 * **当前实现:**
 * 包含一个断言，表明这是不可达代码。
 */
void vPortEndScheduler( void )
{
    /* 不应到达此处。 */
    configASSERT( xPortStartScheduler == NULL );
}

void prvTaskExitError( void )
{
	/* A function that implements a task must not exit or attempt to return to
	its caller as there is nothing to return to. If a task wants to exit it
	should instead call vTaskDelete( NULL ).

	Artificially force an assert() to be triggered if configASSERT() is
	defined, then stop here. */
	configASSERT( xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED );

	for( ;; );
}


/*-----------------------------------------------------------*/
