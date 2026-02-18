/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* FreeRTOS 包含文件。 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* SBI 包含文件 */
#include "riscv_sbi.h"
#include <stdio.h>

/* 创建任务的优先级。 */
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/* 向队列发送数据的速率。 */
#define mainQUEUE_SEND_FREQUENCY_MS         ( 1000 / portTICK_PERIOD_MS )

/* 队列可以容纳的项目数。 */
#define mainQUEUE_LENGTH                    ( 1 )

/*-----------------------------------------------------------*/

/*
 * 本文件顶部注释中描述的任务。
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/*-----------------------------------------------------------*/

/* 两个任务使用的队列。 */
static QueueHandle_t xQueue = NULL;

/*-----------------------------------------------------------*/

/* 使用 SBI 的简单 printf 实现 */
static void sbi_puts(const char *s) {
    while (*s) {
        sbi_console_putchar(*s++);
    }
}

void main_demo( void )
{
    /* 创建队列。 */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    if( xQueue != NULL )
    {
        /* 启动两个任务，如本文件顶部注释所述。 */
        xTaskCreate( prvQueueReceiveTask,             /* 实现任务的函数。 */
                    "Rx",                             /* 分配给任务的文本名称 - 仅用于调试，内核不使用。 */
                    configMINIMAL_STACK_SIZE * 2U,    /* 分配给任务的栈大小。 */
                    NULL,                             /* 传递给任务的参数 - 在此情况下未使用。 */
                    mainQUEUE_RECEIVE_TASK_PRIORITY,  /* 分配给任务的优先级。 */
                    NULL );                           /* 不需要任务句柄，因此传递 NULL。 */

        xTaskCreate( prvQueueSendTask, "Tx", configMINIMAL_STACK_SIZE * 2U, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );

        /* 启动任务和定时器运行。 */
        sbi_puts( "Starting Scheduler...\n" );
        vTaskStartScheduler();
    }

    /* 如果一切正常，调度器现在将运行，并且下一行永远不会被执行。如果下一行执行了，则说明没有足够的 FreeRTOS 堆内存可用于创建空闲和/或定时器任务。有关更多详细信息，请参阅 FreeRTOS 网站上的内存管理部分。 */
    for( ; ; );
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask( void *pvParameters )
{
    TickType_t xNextWakeTime;
    const uint32_t ulValueToSend = 100UL;

    /* 消除关于未使用参数的编译器警告。 */
    ( void ) pvParameters;

    /* 初始化 xNextWakeTime - 这只需要做一次。 */
    xNextWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        /* 将此任务置于阻塞状态，直到再次运行的时间。 */
        vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

        /* 发送到队列 - 导致队列接收任务解除阻塞并切换 LED。 0 用作阻塞时间，因此发送操作不会阻塞 - 此时队列应始终为空，因此不需要阻塞。 */
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
    uint32_t ulReceivedValue;
    char buffer[50];

    /* 消除关于未使用参数的编译器警告。 */
    ( void ) pvParameters;

    for( ; ; )
    {
        /* 等待直到队列中有东西到达 - 只要在 FreeRTOSConfig.h 中将 INCLUDE_vTaskSuspend 设置为 1，此任务就会无限期阻塞。 */
        xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

        /* 到达这里说明必须从队列中接收到了东西，但它是预期的值吗？如果是，切换 LED。 */
        if( ulReceivedValue == 100UL )
        {
            sbi_puts( "Rx Task: Received 100\n" );
        }
    }
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() 仅在 FreeRTOSConfig.h 中将 configUSE_MALLOC_FAILED_HOOK 设置为 1 时才会被调用。这是一个钩子函数，如果调用 pvPortMalloc() 失败，则会调用该函数。pvPortMalloc() 由内核在创建任务、队列、定时器或信号量时内部调用。它也被演示应用程序的各个部分调用。如果使用 heap_1.c 或 heap_2.c，则 pvPortMalloc() 可用的堆大小由 FreeRTOSConfig.h 中的 configTOTAL_HEAP_SIZE 定义，并且可以使用 xPortGetFreeHeapSize() API 函数查询剩余的空闲堆空间大小（尽管它不提供有关剩余堆可能如何碎片化的信息）。 */
    taskDISABLE_INTERRUPTS();
    for( ; ; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* 如果 configCHECK_FOR_STACK_OVERFLOW 定义为 1 或 2，则执行运行时栈溢出检查。如果检测到栈溢出，则调用此钩子函数。 */
    taskDISABLE_INTERRUPTS();
    for( ; ; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    /* 如果在 FreeRTOSConfig.h 中将 configUSE_TICK_HOOK 设置为 1，则每个滴答中断都会调用此函数。用户代码可以在此处添加，但滴答钩子是在中断上下文中调用的，因此代码不得尝试阻塞，并且只能使用中断安全的 FreeRTOS API 函数（以 FromISR() 结尾的函数）。 */
}
/*-----------------------------------------------------------*/
