/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "riscv_sbi.h"

/* Standard includes. */
#include "string.h"

/* The stack used by interrupt service routines. */
#ifdef configISR_STACK_SIZE_WORDS
    static __attribute__( ( aligned( 16 ) ) ) StackType_t xISRStack[ configISR_STACK_SIZE_WORDS ] = { 0 };
    const StackType_t xISRStackTop = ( StackType_t ) &( xISRStack[ configISR_STACK_SIZE_WORDS & ~portBYTE_ALIGNMENT_MASK ] );
#else
    extern const uint32_t __freertos_irq_stack_top[];
    const StackType_t xISRStackTop = ( StackType_t ) __freertos_irq_stack_top;
#endif

/* Used to program the machine timer compare register. */
uint64_t ullNextTime = 0ULL;
/* Counts the number of ticks. */
static uint64_t ullTickCount = 0ULL;

/*
 * Note: configCPU_CLOCK_HZ must be defined in FreeRTOSConfig.h.
 * If not defined, we default to a common value or generate an error.
 * For this example, we assume it is defined.
 */
#ifndef configCPU_CLOCK_HZ
    #error configCPU_CLOCK_HZ must be defined in FreeRTOSConfig.h
#endif

const size_t uxTimerIncrementsForOneTick = ( size_t ) ( ( configCPU_CLOCK_HZ ) / ( configTICK_RATE_HZ ) );

/* Holds the critical nesting value. */
size_t xCriticalNesting = ( size_t ) 0xaaaaaaaa;
size_t * pxCriticalNesting = &xCriticalNesting;

/* Used to catch tasks that attempt to return from their implementing function. */
#ifdef configTASK_RETURN_ADDRESS
    #define portTASK_RETURN_ADDRESS    configTASK_RETURN_ADDRESS
#else
    #define portTASK_RETURN_ADDRESS    0
#endif
size_t xTaskReturnAddress = ( size_t ) portTASK_RETURN_ADDRESS;

/*-----------------------------------------------------------*/

static uint64_t prvReadTime( void )
{
    uint64_t ullCurrentTime;
#if __riscv_xlen == 32
    uint32_t ulHigh, ulLow, ulHigh2;
    do {
        __asm volatile( "rdtimeh %0" : "=r"( ulHigh ) );
        __asm volatile( "rdtime %0" : "=r"( ulLow ) );
        __asm volatile( "rdtimeh %0" : "=r"( ulHigh2 ) );
    } while( ulHigh != ulHigh2 );
    ullCurrentTime = ( ( uint64_t ) ulHigh << 32 ) | ulLow;
#else
    __asm volatile( "rdtime %0" : "=r"( ullCurrentTime ) );
#endif
    return ullCurrentTime;
}

void vPortSetupTimerInterrupt( void )
{
    uint64_t ullCurrentTime = prvReadTime();
    ullNextTime = ullCurrentTime + uxTimerIncrementsForOneTick;
    sbi_set_timer( ullNextTime );
}

/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
    extern void xPortStartFirstTask( void );
    extern void freertos_risc_v_trap_handler( void );

    /* Set stvec to the trap handler. */
    __asm volatile( "csrw stvec, %0" :: "r"( freertos_risc_v_trap_handler ) );

    /* Enable Supervisor Timer Interrupt (SIE bit 5). */
    __asm volatile( "csrs sie, %0" :: "r"( 0x20 ) );

    vPortSetupTimerInterrupt();

    xPortStartFirstTask();

    return pdFAIL;
}

void vPortEndScheduler( void )
{
    for( ; ; );
}

/*-----------------------------------------------------------*/

/*
 * Called by the trap handler to process the tick.
 * Returns non-zero if a context switch is required.
 */
BaseType_t xPortSysTickHandler( void )
{
    BaseType_t xSwitchRequired;

    /* Update next tick time */
    ullNextTime += uxTimerIncrementsForOneTick;
    sbi_set_timer( ullNextTime );

    xSwitchRequired = xTaskIncrementTick();

    return xSwitchRequired;
}

/*-----------------------------------------------------------*/

StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
    /* Simulate the stack frame as it would be created by a context switch interrupt. */
    /* The stack frame is 31 words long. */
    pxTopOfStack -= 31;

    /* Index 0: sepc (Task Entry Point) */
    pxTopOfStack[ 0 ] = ( StackType_t ) pxCode;

    /* Index 1: x1 (ra) (Task Return Address) */
    pxTopOfStack[ 1 ] = ( StackType_t ) portTASK_RETURN_ADDRESS;

    /* Index 2..6: x5..x9. Initialize to 0. */
    memset( &pxTopOfStack[ 2 ], 0, 5 * sizeof( StackType_t ) );

    /* Index 7: x10 (a0) (Task Parameter) */
    pxTopOfStack[ 7 ] = ( StackType_t ) pvParameters;

    /* Index 8..28: x11..x31. Initialize to 0. */
    memset( &pxTopOfStack[ 8 ], 0, 21 * sizeof( StackType_t ) );

    /* Index 29: sstatus */
    /* SPP=1 (Supervisor), SPIE=1 (Enable Interrupts) */
    pxTopOfStack[ 29 ] = ( StackType_t ) 0x00000120;

    /* Index 30: Critical Nesting */
    pxTopOfStack[ 30 ] = ( StackType_t ) 0xaaaaaaaa;

    return pxTopOfStack;
}
