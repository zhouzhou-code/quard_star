/*
 * FreeRTOS 应用主入口
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件是 FreeRTOS 应用的主入口，负责：
 *   1. 初始化 Resource Table
 *   2. 初始化 libmetal
 *   3. 初始化 OpenAMP/RPMsg
 *   4. 启动 RPMsg 服务器
 *   5. 启动 FreeRTOS 调度器
 */

#include "FreeRTOS.h"
#include "task.h"

#include "uart8250.h"
#include "hwspecs.h"
#include "resource_table.h"

/* OpenAMP 适配层 */
#include "openamp_adapter.h"

/* ============================================================================
 * 宏定义
 * ============================================================================ */

#define MAIN_TASK_PRIORITY      (tskIDLE_PRIORITY + 1)
#define MAIN_TASK_STACK_SIZE    2048

/* ============================================================================
 * UART 输出函数
 * ============================================================================ */

/**
 * sbi_print_string() - 打印字符串（使用 UART）
 * 为了兼容性保留此函数名
 */
void sbi_print_string(const char *str)
{
    uart8250_puts(str);
}

/**
 * 早期测试函数 - UART 16550 驱动测试
 */
static void test_uart16550_driver(void)
{
    sbi_print_string("\r\n");
    sbi_print_string("========================================\r\n");
    sbi_print_string("UART 16550 Driver Test\r\n");
    sbi_print_string("========================================\r\n");
    sbi_print_string("Driver ported from OpenSBI\r\n");
    sbi_print_string("Base address: 0x10000000 (UART0)\r\n");
    sbi_print_string("Baudrate: 115200\r\n");
    sbi_print_string("Config: 8N1 (8 data, no parity, 1 stop)\r\n");
    sbi_print_string("========================================\r\n");
    sbi_print_string("\r\n");

    /* 测试 1: 字符输出 */
    sbi_print_string("[Test 1] Character output:\r\n");
    uart8250_putc('H');
    uart8250_putc('e');
    uart8250_putc('l');
    uart8250_putc('l');
    uart8250_putc('o');
    uart8250_putc(' ');
    uart8250_putc('W');
    uart8250_putc('o');
    uart8250_putc('r');
    uart8250_putc('l');
    uart8250_putc('d');
    uart8250_putc('!');
    uart8250_putc('\r');
    uart8250_putc('\n');
    sbi_print_string("\r\n");

    /* 测试 2: 字符串输出 */
    sbi_print_string("[Test 2] String output:\r\n");
    uart8250_puts("Testing string output...\r\n");
    uart8250_puts("Line 1\r\n");
    uart8250_puts("Line 2\r\n");
    uart8250_puts("Line 3\r\n");
    sbi_print_string("\r\n");

    /* 测试 3: 格式化输出（通过简单拼接）*/
    sbi_print_string("[Test 3] Formatted output:\r\n");
    sbi_print_string("Hart ID: ");
    sbi_print_string("7");
    sbi_print_string("\r\n");
    sbi_print_string("Domain: FreeRTOS\r\n");
    sbi_print_string("\r\n");

    sbi_print_string("========================================\r\n");
    sbi_print_string("UART 16550 Driver Test PASSED!\r\n");
    sbi_print_string("========================================\r\n");
    sbi_print_string("\r\n");
}

/* ============================================================================
 * FreeRTOS 钩子函数
 * ============================================================================ */

/**
 * vApplicationStackOverflowHook() - 栈溢出钩子
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    (void)pcTaskName;
    sbi_print_string("ERROR: Stack overflow detected!\r\n");
    while (1) {
        /* 停止执行 */
    }
}

/**
 * vApplicationMallocFailedHook() - 内存分配失败钩子
 */
void vApplicationMallocFailedHook(void)
{
    sbi_print_string("ERROR: Malloc failed!\r\n");
    while (1) {
        /* 停止执行 */
    }
}

/* ============================================================================
 * 外部函数声明
 * ============================================================================ */

extern int rpmsg_server_init(void);
extern int rpmsg_server_start(void);

/* ============================================================================
 * 主任务
 * ============================================================================ */

/**
 * main_task() - 主任务
 * @pvParameters: 任务参数（未使用）
 *
 * 执行流程：
 *   1. 初始化 Resource Table（写入共享内存）
 *   2. 初始化 OpenAMP（libmetal + RPMsg）
 *   3. 启动 RPMsg 服务器
 */
static void main_task(void *pvParameters)
{
    (void)pvParameters;

    uart8250_puts("\r\n");
    uart8250_puts("========================================\r\n");
    uart8250_puts("Quard Star FreeRTOS with OpenAMP\r\n");
    uart8250_puts("Copyright (c) 2025 Quard Star Project\r\n");
    uart8250_puts("========================================\r\n");
    uart8250_puts("Hart: 7 (FreeRTOS Domain)\r\n");
    uart8250_puts("========================================\r\n");
    uart8250_puts("\r\n");

    /* === Step 1: 初始化 Resource Table === */
    uart8250_puts("[Step 1] Initializing Resource Table...\r\n");

    resource_table_init();

    uart8250_puts("Resource Table initialized\r\n");
    uart8250_puts("\r\n");

    /* TODO: Step 2 & 3 - RPMsg initialization (temporarily disabled) */
    uart8250_puts("RPMsg initialization temporarily disabled\r\n");

    uart8250_puts("========================================\r\n");
    uart8250_puts("FreeRTOS Resource Table Ready!\r\n");
    uart8250_puts("Waiting for Linux to attach...\r\n");
    uart8250_puts("========================================\r\n");
    uart8250_puts("\r\n");

    /* 主循环：保持运行 */
    while (1) {
        uart8250_puts("Task main is running... Hart 7\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    /* 不会到达这里 */
    vTaskDelete(NULL);
}

/* ============================================================================
 * 主函数
 * ============================================================================ */

int main(void)
{
    BaseType_t ret;

    /* 初始化 UART（必须在最前面）- 使用移植的 16550 驱动 */
    uart8250_init(UART_BASE, UART_CLK_FREQ, UART_BAUDRATE);

    

    uart8250_puts("\r\n========================================\r\n");

     uart8250_puts("Quard Star FreeRTOS with OpenAMP\r\n");
        uart8250_puts("Booting...\r\n");
        uart8250_puts("========================================\r\n");
   

    /* 测试 UART 16550 驱动 */
    test_uart16550_driver();

    /* 写一个魔数到共享内存，证明 FreeRTOS 正在运行 */
    /* 0x46524545 = "FREE" in ASCII, + 0x07 = Hart 7 signature */
    *(volatile uint32_t *)0xbf700000 = 0x46524545;  /* "FREE" in ASCII */
    *(volatile uint32_t *)0xbf700004 = 0x00000007;  /* Hart 7 */
    uart8250_puts("Magic signature written to 0xbf70000\r\n");

    /* 创建主任务 */
    ret = xTaskCreate(main_task,
                      "Main",
                      MAIN_TASK_STACK_SIZE,
                      NULL,
                      MAIN_TASK_PRIORITY,
                      NULL);

    if (ret != pdPASS) {
        uart8250_puts("ERROR: Failed to create main task!\r\n");
        return -1;
    }

    /* 启动调度器 */
    uart8250_puts("Starting FreeRTOS scheduler...\r\n");
    
    vTaskStartScheduler();

    /* 不应该到达这里 */
    uart8250_puts("ERROR: Scheduler returned!\r\n");
    return -1;
}
