/*
 * FreeRTOS V10.4.3 Example for RISC-V S-Mode
 * 包含双任务 SBI 打印测试
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "riscv_sbi.h"

void sbi_print_string(const char *str)
{
    /* 逐个字符塞给 OpenSBI，注意加上 \r 防止串口输出变阶梯状 */
    while (*str) {
        if (*str == '\n') {
            sbi_console_putchar('\r');
        }
        sbi_console_putchar(*str++);
    }
}

#define TASK1_PRIORITY    ( tskIDLE_PRIORITY + 1 )
#define TASK2_PRIORITY    ( tskIDLE_PRIORITY + 1 )

static void prvTask1( void *pvParameters );
static void prvTask2( void *pvParameters );

/* Global counter for debug */
volatile uint64_t g_counter = 0;

int main( void )
{
    sbi_print_string("\n========================================\n");
    sbi_print_string("🚀 FreeRTOS is Booting on RISC-V S-Mode!\n");
    sbi_print_string("========================================\n");

    /* 创建测试任务 1 (稍微给大点栈，防止字符串压栈导致溢出) */
    xTaskCreate( prvTask1,                  
                 "Task_1",                  
                 configMINIMAL_STACK_SIZE * 2, 
                 NULL,                  
                 TASK1_PRIORITY,            
                 NULL );                

    /* 创建测试任务 2 */
    xTaskCreate( prvTask2,                  
                 "Task_2",                  
                 configMINIMAL_STACK_SIZE * 2, 
                 NULL,                  
                 TASK2_PRIORITY,            
                 NULL );                

    sbi_print_string(">>> Starting Scheduler...\n");

    /* 启动调度器：此时会接管 SysTick，并跳转到最高优先级的任务 */
    vTaskStartScheduler();

    /* 正常情况下永远不会执行到这里。如果到了这里，说明内存堆 (Heap) 不够了 */
    sbi_print_string("🚨 ERROR: Insufficient Heap Memory!\n");
    for( ;; );
    return 0;
}

/* ====================================================================
 * 4. 任务 1 实现
 * ==================================================================== */
static void prvTask1( void *pvParameters )
{
    ( void ) pvParameters;

    for( ;; )
    {
        /* 打印自己正在运行 */
        //sbi_print_string("[Task 1] is running! Beep...\n");
        g_counter++;

        /* 阻塞延时 1000 毫秒，交出 CPU 控制权 */
        vTaskDelay( pdMS_TO_TICKS( 200 ) );
    }
}

/* ====================================================================
 * 5. 任务 2 实现
 * ==================================================================== */
static void prvTask2( void *pvParameters )
{
    ( void ) pvParameters;

    for( ;; )
    {
        /* 打印自己正在运行 */
        //sbi_print_string("[Task 2] is running! Boop...\n");

        /* 阻塞延时 1500 毫秒 (时间错开，验证多任务并行) */
        vTaskDelay( pdMS_TO_TICKS( 200 ) );
    }
}

/* ====================================================================
 * 6. FreeRTOS 异常钩子函数 (Hooks)
 * ==================================================================== */
void vApplicationMallocFailedHook( void )
{
    /* 内存分配失败钩子：疯狂打印报错并死机 */
    taskDISABLE_INTERRUPTS();
    sbi_print_string("\n🚨 FATAL: Malloc Failed Hook Triggered!\n");
    for( ;; );
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pxTask;
    /* 栈溢出钩子：把溢出的任务名字打出来，极其方便查 Bug */
    taskDISABLE_INTERRUPTS();
    sbi_print_string("\n🚨 FATAL: Stack Overflow in Task: ");
    sbi_print_string(pcTaskName);
    sbi_print_string("\n");
    for( ;; );
}