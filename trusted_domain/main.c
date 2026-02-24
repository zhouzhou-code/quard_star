/*
 * FreeRTOS V10.4.3 Example for RISC-V S-Mode
 * 包含双任务 SBI 打印测试
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "riscv_sbi.h"
#include <string.h>
#include "FreeRTOS.h"

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

// 物理地址，必须和 Linux 设备树里的完全一致
#define AMP_SHM_BASE_ADDR 0xbf700000 

// 共享内存数据结构
struct amp_mailbox {
    volatile uint32_t f2l_flag;  // Offset 0x00
    char payload[256];           // Offset 0x04
} __attribute__((packed));

// 直接把物理地址强转为结构体指针
#define MAILBOX ((struct amp_mailbox *)AMP_SHM_BASE_ADDR)

// 定义 FreeRTOS 发送任务
void vIPCSendTask(void *pvParameters)
{
    uint32_t counter = 0;
    char temp_str[100];

    // 初始化：确保最开始标志位是 0
    MAILBOX->f2l_flag = 0;

    while(1) {
        // 等待 Linux 读完 (只有标志位被 Linux 清 0，我们才发下一条)
        if (MAILBOX->f2l_flag == 0) {
            
            // 1. 准备字符串数据
            snprintf(temp_str, sizeof(temp_str), "Hello Linux! I'm FreeRTOS, MSG ID: %d", counter++);
            
            // 2. 拷贝数据到共享内存的 payload 区
            strncpy((char *)MAILBOX->payload, temp_str, 255);
            MAILBOX->payload[255] = '\0'; // 保底防止越界
            
            // 3. 【面试绝杀点】内存屏障！确保数据真正落入 DDR 后，再去改标志位
            __asm__ volatile("fence rw, rw" ::: "memory");
            
            // 4. 竖起 Flag，触发 Linux 侧的轮询
            MAILBOX->f2l_flag = 1;
        }
        
        // 延时 1000 个 Tick (通常是 1 秒)，每秒发一次
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


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

    /* ipc 发送任务 */
    xTaskCreate(vIPCSendTask, "IPC_Tx", 512, NULL, tskIDLE_PRIORITY + 1, NULL);

    sbi_print_string(">>> Starting Scheduler...\n");

    /* 启动调度器：此时会接管 SysTick，并跳转到最高优先级的任务 */
    vTaskStartScheduler();

    /* 正常情况下永远不会执行到这里。如果到了这里，说明内存堆 (Heap) 不够了 */
    //sbi_print_string("🚨 ERROR: Insufficient Heap Memory!\n");
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
        // sbi_print_string("[Task 1] is running! Beep...\n");
        g_counter++;

        /* 阻塞延时 1000 毫秒，交出 CPU 控制权 */
        vTaskDelay( pdMS_TO_TICKS( 4000 ) );
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
        // sbi_print_string("[Task 2] is running! Boop...\n");

        /* 阻塞延时 1500 毫秒 (时间错开，验证多任务并行) */
        vTaskDelay( pdMS_TO_TICKS( 8000 ) );
    }
}

/* ====================================================================
 * 6. FreeRTOS 异常钩子函数 (Hooks)
 * ==================================================================== */
void vApplicationMallocFailedHook( void )
{
    /* 内存分配失败钩子：疯狂打印报错并死机 */
    taskDISABLE_INTERRUPTS();
    // sbi_print_string("\n🚨 FATAL: Malloc Failed Hook Triggered!\n");
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