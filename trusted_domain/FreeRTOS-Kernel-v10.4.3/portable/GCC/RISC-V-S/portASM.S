/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include "riscv_sbi.h"

/* Check the freertos_risc_v_chip_specific_extensions.h and/or command line definitions. */
#if __riscv_xlen == 64
    #define portWORD_SIZE    8
    #define store_x          sd
    #define load_x           ld
#elif __riscv_xlen == 32
    #define portWORD_SIZE    4
    #define store_x          sw
    #define load_x           lw
#else
    #error Assembler did not define __riscv_xlen
#endif

#define portCONTEXT_SIZE               ( 31 * portWORD_SIZE )
#define portCRITICAL_NESTING_OFFSET    30

.global xPortStartFirstTask
.global pxPortInitialiseStack
.global freertos_risc_v_trap_handler

.extern vTaskSwitchContext
.extern xTaskIncrementTick
.extern xPortSysTickHandler
.extern xTaskReturnAddress
.extern pxCurrentTCB
.extern xISRStackTop
.extern xCriticalNesting
.extern pxCriticalNesting

/*-----------------------------------------------------------*/

.align 8
freertos_risc_v_trap_handler:
    addi sp, sp, -portCONTEXT_SIZE
    store_x x1,  1  * portWORD_SIZE( sp )
    store_x x5,  2  * portWORD_SIZE( sp )
    store_x x6,  3  * portWORD_SIZE( sp )
    store_x x7,  4  * portWORD_SIZE( sp )
    store_x x8,  5  * portWORD_SIZE( sp )
    store_x x9,  6  * portWORD_SIZE( sp )
    store_x x10, 7  * portWORD_SIZE( sp )
    store_x x11, 8  * portWORD_SIZE( sp )
    store_x x12, 9  * portWORD_SIZE( sp )
    store_x x13, 10 * portWORD_SIZE( sp )
    store_x x14, 11 * portWORD_SIZE( sp )
    store_x x15, 12 * portWORD_SIZE( sp )
    store_x x16, 13 * portWORD_SIZE( sp )
    store_x x17, 14 * portWORD_SIZE( sp )
    store_x x18, 15 * portWORD_SIZE( sp )
    store_x x19, 16 * portWORD_SIZE( sp )
    store_x x20, 17 * portWORD_SIZE( sp )
    store_x x21, 18 * portWORD_SIZE( sp )
    store_x x22, 19 * portWORD_SIZE( sp )
    store_x x23, 20 * portWORD_SIZE( sp )
    store_x x24, 21 * portWORD_SIZE( sp )
    store_x x25, 22 * portWORD_SIZE( sp )
    store_x x26, 23 * portWORD_SIZE( sp )
    store_x x27, 24 * portWORD_SIZE( sp )
    store_x x28, 25 * portWORD_SIZE( sp )
    store_x x29, 26 * portWORD_SIZE( sp )
    store_x x30, 27 * portWORD_SIZE( sp )
    store_x x31, 28 * portWORD_SIZE( sp )

    load_x t0, xCriticalNesting
    store_x t0, portCRITICAL_NESTING_OFFSET * portWORD_SIZE( sp )

    csrr t0, sstatus
    store_x t0, 29 * portWORD_SIZE( sp )

    csrr t0, sepc
    store_x t0, 0 * portWORD_SIZE( sp )

    load_x t0, pxCurrentTCB
    store_x sp, 0( t0 )

    csrr a0, scause
    csrr a1, sepc

    /* Check for interrupts */
    blt a0, x0, handle_interrupt

handle_synchronous:
    /* Synchronous exception. */
    addi a1, a1, 4
    csrw sepc, a1
    j processed_source

handle_interrupt:
    /* Check for Timer Interrupt (Cause: 0x8000...005) */
    slli t0, a0, 1  /* Shift left to remove MSB */
    srli t0, t0, 1
    li t1, 5
    bne t0, t1, handle_other_interrupt

    /* Timer Interrupt */
    call xPortSysTickHandler
    beqz a0, processed_source
    call vTaskSwitchContext
    j processed_source

handle_other_interrupt:
    /* Other interrupts (External, Software) - not implemented in this demo */
    j processed_source

processed_source:
    load_x t1, pxCurrentTCB
    load_x sp, 0( t1 )

    /* Restore sepc */
    load_x t0, 0 * portWORD_SIZE( sp )
    csrw sepc, t0

    /* Restore sstatus */
    load_x t0, 29 * portWORD_SIZE( sp )
    csrw sstatus, t0

    load_x t0, portCRITICAL_NESTING_OFFSET * portWORD_SIZE( sp )
    load_x t1, pxCriticalNesting
    store_x t0, 0( t1 )

    load_x x1,  1  * portWORD_SIZE( sp )
    load_x x5,  2  * portWORD_SIZE( sp )
    load_x x6,  3  * portWORD_SIZE( sp )
    load_x x7,  4  * portWORD_SIZE( sp )
    load_x x8,  5  * portWORD_SIZE( sp )
    load_x x9,  6  * portWORD_SIZE( sp )
    load_x x10, 7  * portWORD_SIZE( sp )
    load_x x11, 8  * portWORD_SIZE( sp )
    load_x x12, 9  * portWORD_SIZE( sp )
    load_x x13, 10 * portWORD_SIZE( sp )
    load_x x14, 11 * portWORD_SIZE( sp )
    load_x x15, 12 * portWORD_SIZE( sp )
    load_x x16, 13 * portWORD_SIZE( sp )
    load_x x17, 14 * portWORD_SIZE( sp )
    load_x x18, 15 * portWORD_SIZE( sp )
    load_x x19, 16 * portWORD_SIZE( sp )
    load_x x20, 17 * portWORD_SIZE( sp )
    load_x x21, 18 * portWORD_SIZE( sp )
    load_x x22, 19 * portWORD_SIZE( sp )
    load_x x23, 20 * portWORD_SIZE( sp )
    load_x x24, 21 * portWORD_SIZE( sp )
    load_x x25, 22 * portWORD_SIZE( sp )
    load_x x26, 23 * portWORD_SIZE( sp )
    load_x x27, 24 * portWORD_SIZE( sp )
    load_x x28, 25 * portWORD_SIZE( sp )
    load_x x29, 26 * portWORD_SIZE( sp )
    load_x x30, 27 * portWORD_SIZE( sp )
    load_x x31, 28 * portWORD_SIZE( sp )

    addi sp, sp, portCONTEXT_SIZE
    sret

/*-----------------------------------------------------------*/

xPortStartFirstTask:
    load_x sp, pxCurrentTCB
    load_x sp, 0( sp )

    load_x x1, 0 * portWORD_SIZE( sp )
    csrw sepc, x1

    load_x x5, 29 * portWORD_SIZE( sp ) /* sstatus */
    csrw sstatus, x5

    load_x x1,  1  * portWORD_SIZE( sp )
    load_x x5,  2  * portWORD_SIZE( sp )
    load_x x6,  3  * portWORD_SIZE( sp )
    load_x x7,  4  * portWORD_SIZE( sp )
    load_x x8,  5  * portWORD_SIZE( sp )
    load_x x9,  6  * portWORD_SIZE( sp )
    load_x x10, 7  * portWORD_SIZE( sp )
    load_x x11, 8  * portWORD_SIZE( sp )
    load_x x12, 9  * portWORD_SIZE( sp )
    load_x x13, 10 * portWORD_SIZE( sp )
    load_x x14, 11 * portWORD_SIZE( sp )
    load_x x15, 12 * portWORD_SIZE( sp )
    load_x x16, 13 * portWORD_SIZE( sp )
    load_x x17, 14 * portWORD_SIZE( sp )
    load_x x18, 15 * portWORD_SIZE( sp )
    load_x x19, 16 * portWORD_SIZE( sp )
    load_x x20, 17 * portWORD_SIZE( sp )
    load_x x21, 18 * portWORD_SIZE( sp )
    load_x x22, 19 * portWORD_SIZE( sp )
    load_x x23, 20 * portWORD_SIZE( sp )
    load_x x24, 21 * portWORD_SIZE( sp )
    load_x x25, 22 * portWORD_SIZE( sp )
    load_x x26, 23 * portWORD_SIZE( sp )
    load_x x27, 24 * portWORD_SIZE( sp )
    load_x x28, 25 * portWORD_SIZE( sp )
    load_x x29, 26 * portWORD_SIZE( sp )
    load_x x30, 27 * portWORD_SIZE( sp )
    load_x x31, 28 * portWORD_SIZE( sp )

    addi sp, sp, portCONTEXT_SIZE
    sret
