/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif
#include "riscv_asm.h"
#include "riscv_encoding.h"

/* ------------------------------------移植基础数据类型定义------------------------------- */
/**
 * @brief RISC-V 架构字长（XLEN）定义。
 *
 * 该宏通常由 RISC-V 编译器（如 GCC/Clang）根据目标架构选项（-march/-mabi）自动定义。
 *
 * - 取值 64: 表示 RV64 架构，通用寄存器宽度为 64 位。
 * - 取值 32: 表示 RV32 架构，通用寄存器宽度为 32 位。
 *
 * 在 FreeRTOS 移植层中，该宏用于：
 * 1. 决定 StackType_t 和 BaseType_t 的位宽。
 * 2. 区分处理 32 位与 64 位系统的特定逻辑（如 SBI 调用参数处理）。
 * 3. 确保指针算术运算的正确性。
 */
#if __riscv_xlen == 64
    #define portSTACK_TYPE           uint64_t
    #define portBASE_TYPE            int64_t
    #define portUBASE_TYPE           uint64_t
    #define portMAX_DELAY            ( TickType_t ) 0xffffffffffffffffUL
    #define portPOINTER_SIZE_TYPE    uint64_t
#elif __riscv_xlen == 32
    #define portSTACK_TYPE           uint32_t
    #define portBASE_TYPE            int32_t
    #define portUBASE_TYPE           uint32_t
    #define portMAX_DELAY            ( TickType_t ) 0xffffffffUL
#else
    #error "Assembler did not define __riscv_xlen"
#endif

typedef portSTACK_TYPE   StackType_t;
typedef portBASE_TYPE    BaseType_t;
typedef portUBASE_TYPE   UBaseType_t;
typedef portUBASE_TYPE   TickType_t;

#define portCHAR                   char
#define portFLOAT                  float
#define portDOUBLE                 double
#define portLONG                   long
#define portSHORT                  short

/* 32位架构上的32位滴答类型，因此读取滴答计数不需要临界区保护。 */
#define portTICK_TYPE_IS_ATOMIC    1
/*-----------------------------------------------------------*/

/**
 * @brief 定义任务栈的增长方向。
 * - 取值 -1: 栈从高地址向低地址增长（向下增长）。
 * - 取值  1: 栈从低地址向高地址增长（向上增长）。
 *
 * 依据 RISC-V 应用程序二进制接口（ABI）规范，RISC-V 架构的栈是向下增长的。
 * 在 FreeRTOS 中，此宏影响任务创建时栈顶指针（pxTopOfStack）的初始位置计算
 * 以及栈溢出检查的逻辑。
 */
#define portSTACK_GROWTH          ( -1 )

/**
 * @brief 系统 Tick 周期（单位：毫秒）。
 *
 * 该宏计算一个系统 Tick 所持续的毫秒数，公式为 1000 / configTICK_RATE_HZ。
 *
 * - 用途: 供用户应用程序将时间（毫秒）转换为 Tick 数，或反之。
 * - 注意: 如果 configTICK_RATE_HZ 大于 1000，此值将为 0，可能导致除零错误或逻辑错误，
 *   因此通常建议 configTICK_RATE_HZ 设置在 100 到 1000 之间。
 */
#define portTICK_PERIOD_MS        ( ( TickType_t ) 1000 / configTICK_RATE_HZ )

/**
 * @brief 内存分配和栈对齐字节数。
 *
 * - 取值 16: 表示 16 字节（128 位）对齐。
 *
 * RISC-V ABI 规范：栈指针（sp）必须保持 16 字节对齐。
 * 即使在 RV32 上，为了兼容 `double` (64-bit) 和 `long double` (128-bit) 类型
 * 以及未来的扩展（如向量扩展），标准也推荐或强制要求 16 字节对齐。
 *
 * 在 FreeRTOS 中，此宏确保：
 * 1. 动态分配的任务栈起始地址是 16 字节对齐的。
 * 2. heap_x.c 内存管理方案分配的内存块是 16 字节对齐的。
 */
#define portBYTE_ALIGNMENT        16

/*------------------------调度触发,上下文切换相关宏定义-----------------------------------------*/

/**
 * @brief 选择下一个要运行的任务。
 *
 * 这是 FreeRTOS 内核的核心函数（定义在 tasks.c 中）。
 * 它会检查就绪列表，找到当前处于就绪态的最高优先级任务，
 * 并更新 pxCurrentTCB 指针指向该任务。
 */
extern void vTaskSwitchContext( void );
/**
 * @brief 主动请求任务切换（Yield）
 *
 * 当任务想要主动放弃 CPU（例如调用 taskYIELD()）时使用此宏。
 * 实现机制:
   设置SIP寄存器的SSIP位，触发S模式下的Soft中断
 */
#define portYIELD()        csr_set(CSR_SIP, SIP_SSIP) 

/**
 * @brief 中断服务例程（ISR）结束时的上下文切换处理。
 *
 * 此宏通常在 ISR 的末尾使用，用于检查中断处理过程中是否唤醒了更高优先级的任务。
 *
 * @param xSwitchRequired
 * - pdTRUE: 需要进行上下文切换（例如，ISR 发送信号量唤醒了一个高优先级任务）。
 * - pdFALSE: 不需要切换。
 *
 * **工作原理:**
 * 如果 xSwitchRequired 为真,它会调用vTaskSwitchContext()更新pxCurrentTCB
 * 当中断处理程序最终执行上下文恢复（restore context）并执行 `sret` 返回时，
 * 它将从新的 pxCurrentTCB（新任务）的栈中恢复数据，从而实现中断返回时的任务切换。
 */
#define portEND_SWITCHING_ISR( xSwitchRequired ) \
    do                                           \
    {                                            \
        if( xSwitchRequired != pdFALSE )         \
        {                                        \
            traceISR_EXIT_TO_SCHEDULER();        \
            vTaskSwitchContext();                \
        }                                        \
        else                                     \
        {                                        \
            traceISR_EXIT();                     \
        }                                        \
    } while( 0 )
#define portYIELD_FROM_ISR( x )    portEND_SWITCHING_ISR( x )
/*-----------------------------------------------------------*/

/* ----------------------------------中断和临界区管理-------------------------------------- */

#define portCRITICAL_NESTING_IN_TCB    0

/* S模式使用sstatus.SIE控制中断使能。SIE是第1位，1为使能，0为禁用。 */
#define portDISABLE_INTERRUPTS()   __asm volatile ( "csrc sstatus, 2" )
#define portENABLE_INTERRUPTS()    __asm volatile ( "csrs sstatus, 2" )

//通过xCriticalNesting计数器实现嵌套临界区:进几次临界区就要出几次临界区
/* - 如果你在一个函数 A 中进入了临界区，然后 A 调用了函数 B，函数 B 也需要进入临界区，这就是嵌套。
   - 如果没有计数器机制，函数 B 退出临界区时就会直接开启中断，导致函数 A 剩下的代码不再受保护。 
*/
extern size_t xCriticalNesting;
#define portENTER_CRITICAL()      \
    {                             \
        portDISABLE_INTERRUPTS(); \
        xCriticalNesting++;       \
    }

#define portEXIT_CRITICAL()          \
    {                                \
        xCriticalNesting--;          \
        if( xCriticalNesting == 0 )  \
        {                            \
            portENABLE_INTERRUPTS(); \
        }                            \
    }


/*----------------------------------可选的优化算法------------------------------------------*/

/**
 * @brief 启用端口特定的任务选择优化算法。
 *
 * FreeRTOS 默认使用通用算法（Generic Method）来查找最高优先级的就绪任务，这通常涉及
 * 遍历链表或检查数组，时间复杂度可能随优先级数量增加。
 *
 * 通过将此宏设置为 1，我们启用特定于硬件的Bitmap算法
 *
 * **优势:**
 * - **O(1) 时间复杂度**: 无论有多少优先级，查找速度都是恒定的。
 * - **指令级加速**: 利用处理器提供的“计算前导零”（Count Leading Zeros, CLZ）指令
 *   直接定位最高优先级的置位位。
 */
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION    1
#endif

#if ( configUSE_PORT_OPTIMISED_TASK_SELECTION == 1 )

/* 检查配置。 */
    /**
     * @brief 优化算法的限制。
     *
     * 由于我们使用一个 32 位的整型变量（uxReadyPriorities）作为位图，
     * 每个位代表一个优先级，因此最大优先级数不能超过 32。
     */
    #if ( configMAX_PRIORITIES > 32 )
        #error "configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32."
    #endif

/* 在位图中存储/清除就绪优先级。 */

    /**
     * @brief 标记某优先级有就绪任务。
     *
     * 将 uxReadyPriorities 位图中的第 uxPriority 位设置为 1。
     */
    #define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities )    ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )

    /**
     * @brief 清除某优先级的就绪标记。
     *
     * 将 uxReadyPriorities 位图中的第 uxPriority 位清除为 0。
     */
    #define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities )     ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )

    /**
     * @brief 快速查找最高优先级。
     *
     * 这是优化算法的核心。
     *
     * **原理:**
     * 1. `__builtin_clz(x)`: GCC内建函数，返回x二进制表示中最高有效位（MSB）之前0的个数。
     *    例如，对于 32 位整数，如果最高位是 1，clz 返回 0；如果只有最低位是 1，clz 返回 31。
     * 2. `31UL - ...`: 将前导零个数转换为最高置位位的索引。
     *    例如，如果 uxReadyPriorities = 0x80000000 (最高位为1)，clz=0，结果=31。
     *
     * 这条指令通常对应 RISC-V 的位操作扩展（如 B 扩展中的 CLZ），或者由编译器生成高效的指令序列。
     */
    #define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities )    uxTopPriority = ( 31UL - __builtin_clz( uxReadyPriorities ) )

#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */

/*-----------------------------------------------------------*/

#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )    void vFunction( void * pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )          void vFunction( void * pvParameters )

/*-----------------------------------------------------------*/

#define portNOP()    __asm volatile ( " nop " )
#define portINLINE              __inline

#ifndef portFORCE_INLINE
    #define portFORCE_INLINE    inline __attribute__( ( always_inline ) )
#endif

#define portMEMORY_BARRIER()    __asm volatile ( "" ::: "memory" )

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
