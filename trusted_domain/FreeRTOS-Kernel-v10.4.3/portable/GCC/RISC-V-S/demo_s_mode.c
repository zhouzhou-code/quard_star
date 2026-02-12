/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* SBI Includes */
#include "riscv_sbi.h"
#include <stdio.h>

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue. */
#define mainQUEUE_SEND_FREQUENCY_MS         ( 1000 / portTICK_PERIOD_MS )

/* The number of items the queue can hold. */
#define mainQUEUE_LENGTH                    ( 1 )

/*-----------------------------------------------------------*/

/*
 * The tasks as described in the comments at the top of this file.
 */
static void prvQueueReceiveTask( void *pvParameters );
static void prvQueueSendTask( void *pvParameters );

/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

/*-----------------------------------------------------------*/

/* Simple printf implementation using SBI */
static void sbi_puts(const char *s) {
    while (*s) {
        sbi_console_putchar(*s++);
    }
}

void main_demo( void )
{
    /* Create the queue. */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( uint32_t ) );

    if( xQueue != NULL )
    {
        /* Start the two tasks as described in the comments at the top of this
        file. */
        xTaskCreate( prvQueueReceiveTask,             /* The function that implements the task. */
                    "Rx",                             /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                    configMINIMAL_STACK_SIZE * 2U,    /* The size of the stack to allocate to the task. */
                    NULL,                             /* The parameter passed to the task - not used in this case. */
                    mainQUEUE_RECEIVE_TASK_PRIORITY,  /* The priority assigned to the task. */
                    NULL );                           /* The task handle is not required, so NULL is passed. */

        xTaskCreate( prvQueueSendTask, "Tx", configMINIMAL_STACK_SIZE * 2U, NULL, mainQUEUE_SEND_TASK_PRIORITY, NULL );

        /* Start the tasks and timer running. */
        sbi_puts( "Starting Scheduler...\n" );
        vTaskStartScheduler();
    }

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the idle and/or
    timer tasks to be created.  See the memory management section on the
    FreeRTOS web site for more details. */
    for( ; ; );
}
/*-----------------------------------------------------------*/

static void prvQueueSendTask( void *pvParameters )
{
    TickType_t xNextWakeTime;
    const uint32_t ulValueToSend = 100UL;

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    /* Initialise xNextWakeTime - this only needs to be done once. */
    xNextWakeTime = xTaskGetTickCount();

    for( ; ; )
    {
        /* Place this task in the blocked state until it is time to run again. */
        vTaskDelayUntil( &xNextWakeTime, mainQUEUE_SEND_FREQUENCY_MS );

        /* Send to the queue - causing the queue receive task to unblock and
        toggle the LED.  0 is used as the block time so the sending operation
        will not block - it shouldn't need to block as the queue should always
        be empty at this point in the code. */
        xQueueSend( xQueue, &ulValueToSend, 0U );
    }
}
/*-----------------------------------------------------------*/

static void prvQueueReceiveTask( void *pvParameters )
{
    uint32_t ulReceivedValue;
    char buffer[50];

    /* Remove compiler warning about unused parameter. */
    ( void ) pvParameters;

    for( ; ; )
    {
        /* Wait until something arrives in the queue - this task will block
        indefinitely provided INCLUDE_vTaskSuspend is set to 1 in
        FreeRTOSConfig.h. */
        xQueueReceive( xQueue, &ulReceivedValue, portMAX_DELAY );

        /*  To get here something must have been received from the queue, but
        is it the expected value?  If it is, toggle the LED. */
        if( ulReceivedValue == 100UL )
        {
            sbi_puts( "Rx Task: Received 100\n" );
        }
    }
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for( ; ; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ; ; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    /* This function will be called by each tick interrupt if
    configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
    added here, but the tick hook is called from an interrupt context, so
    code must not attempt to block, and only the interrupt safe FreeRTOS API
    functions can be used (those that end in FromISR()). */
}
/*-----------------------------------------------------------*/
