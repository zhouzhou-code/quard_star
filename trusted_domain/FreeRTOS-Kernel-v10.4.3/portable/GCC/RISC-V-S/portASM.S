/*
 * FreeRTOS Kernel <DEVELOPMENT BRANCH>
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 */

#include "riscv_encoding.h"

/**
 * @brief 根据架构位宽定义字长和加载/存储指令。
 * 
 * RISC-V 架构支持 32 位和 64 位实现。
 * 此处根据编译器定义的 __riscv_xlen 宏来适配指令集。
 */
#if __riscv_xlen == 64
	#define portWORD_SIZE 8
	#define store_x sd //存储双字
	#define load_x ld   //加载双字
#elif __riscv_xlen == 32
	#define portWORD_SIZE 4
	#define store_x sw //存储字
	#define load_x lw  //加载字
#else
	#error Assembler did not define __riscv_xlen
#endif

/* 默认只保存标准的整数核心寄存器。 */
/**
 * @brief 定义上下文大小和浮点指令。
 * 
 * 如果启用了硬件浮点单元 (FPU)，则需要额外的空间来保存浮点寄存器。
 * portCONTEXT_SIZE 计算了保存所有必要寄存器所需的栈空间大小。
 */
 //64位有浮点，保存30个通用寄存器(x1,x5-x31 + sstatus+sepc)+32个浮点寄存器(f0-f31)
#if __riscv_flen == 64 
	#define portCONTEXT_SIZE ( 62 * portWORD_SIZE )
	#define store_fx fsd
	#define load_fx fld
#elif __riscv_xlen == 32
	#define portCONTEXT_SIZE ( 62 * portWORD_SIZE )
	#define store_fx fsw
	#define load_fx flw
#else //64位无浮点，保存30个通用寄存器(x1,x5-x31 + sstatus+sepc)
	#define portCONTEXT_SIZE ( 30 * portWORD_SIZE )
#endif

/* 导出全局符号，供链接器和 C 代码使用。 */
.global xPortStartFirstTask
.global freertos_risc_v_trap_handler
.global pxPortInitialiseStack
.extern pxCurrentTCB
.extern ulPortTrapHandler
.extern vTaskSwitchContext
.extern xTaskIncrementTick
.extern Timer_IRQHandler
.extern pullMachineTimerCompareRegister
.extern pullNextTime
.extern uxTimerIncrementsForOneTick /* size_t 类型，在 32 位核上为 32 位，在 64 位核上为 64 位。 */
.extern xISRStackTop //中断栈顶指针，指向中断栈的顶部
.extern prvTaskExitError

/*-----------------------------------------------------------*/

/**
 * @brief FreeRTOS RISC-V S-Mode 陷阱处理程序 (Trap Handler)。
 * 
 * 这是所有中断和异常的统一入口点。
 * 
 * **主要功能:**
 * 1. 保存当前任务的上下文（寄存器）。
 * 2. 检查是异步中断还是同步异常。
 * 3. 分发处理：定时器中断、软件中断(IPI)、外部中断或同步异常。
 * 4. 调用调度器 (vTaskSwitchContext) 进行任务切换。
 * 5. 恢复上下文（可能是同一个任务，也可能是新任务）。
 * 6. 执行 sret 返回。
 */
.align 8
.func
freertos_risc_v_trap_handler:
	/* 1. 分配栈空间sp往下减portCONTEXT_SIZE */
	addi sp, sp, -portCONTEXT_SIZE
	
	/* 2. 保存通用寄存器 (x1, x5-x31) */
    //store_x x1, Offset(Base)  把x1写入到内存地址Base+Offset处
	store_x x1, 1 * portWORD_SIZE( sp ) 
	store_x x5, 2 * portWORD_SIZE( sp )
	store_x x6, 3 * portWORD_SIZE( sp )
	store_x x7, 4 * portWORD_SIZE( sp )
	store_x x8, 5 * portWORD_SIZE( sp )
	store_x x9, 6 * portWORD_SIZE( sp )
	store_x x10, 7 * portWORD_SIZE( sp )
	store_x x11, 8 * portWORD_SIZE( sp )
	store_x x12, 9 * portWORD_SIZE( sp )
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

	/* 3. 保存 sstatus (Supervisor Status) 寄存器 */
	csrr t0, sstatus	/* 获取 sstatus，主要为了保存SPIE(Supervisor Previous Interrupt Enable) 位。 */
	store_x t0, 29 * portWORD_SIZE( sp )

	/* 4. 如果启用了 FPU，保存浮点寄存器 */
#ifdef __riscv_flen
	csrr t2, sstatus
	li t0, SSTATUS_FS       /* 浮点状态位掩码 */
	and t1,t2,t0
	bne t1,t0,1f            /* 如果 FPU 状态不是 Dirty/Initial，跳过保存 */

	store_fx f0, 30 * portWORD_SIZE( sp )
	store_fx f1, 31 * portWORD_SIZE( sp )
	store_fx f2, 32 * portWORD_SIZE( sp )
	store_fx f3, 33 * portWORD_SIZE( sp )
	store_fx f4, 34 * portWORD_SIZE( sp )
	store_fx f5, 35 * portWORD_SIZE( sp )
	store_fx f6, 36 * portWORD_SIZE( sp )
	store_fx f7, 37 * portWORD_SIZE( sp )
	store_fx f8, 38 * portWORD_SIZE( sp )
	store_fx f9, 39 * portWORD_SIZE( sp )
	store_fx f10, 40 * portWORD_SIZE( sp )
	store_fx f11, 41 * portWORD_SIZE( sp )
	store_fx f12, 42 * portWORD_SIZE( sp )
	store_fx f13, 43 * portWORD_SIZE( sp )
	store_fx f14, 44 * portWORD_SIZE( sp )
	store_fx f15, 45 * portWORD_SIZE( sp )
	store_fx f16, 46 * portWORD_SIZE( sp )
	store_fx f17, 47 * portWORD_SIZE( sp )
	store_fx f18, 48 * portWORD_SIZE( sp )
	store_fx f19, 49 * portWORD_SIZE( sp )
	store_fx f20, 50 * portWORD_SIZE( sp )
	store_fx f21, 51 * portWORD_SIZE( sp )
	store_fx f22, 52 * portWORD_SIZE( sp )
	store_fx f23, 53 * portWORD_SIZE( sp )
	store_fx f24, 54 * portWORD_SIZE( sp )
	store_fx f25, 55 * portWORD_SIZE( sp )
	store_fx f26, 56 * portWORD_SIZE( sp )
	store_fx f27, 57 * portWORD_SIZE( sp )
	store_fx f28, 58 * portWORD_SIZE( sp )
	store_fx f29, 59 * portWORD_SIZE( sp )
	store_fx f30, 60 * portWORD_SIZE( sp )
	store_fx f31, 61 * portWORD_SIZE( sp )
1:	
	/* 更新 sstatus，清除 FS 位或进行其他设置 */
	li t0, (0x1 << 14)
	csrs sstatus,t0
	li t0, (0x1 << 13)
	csrc sstatus,t0   
#endif

	/* 5. 保存sepc寄存器到sp+0处*/
	csrr t0, sepc
	store_x t0, 0 * portWORD_SIZE( sp )

	/* 6. 将当前的栈指针sp保存到当前任务的TCB中 */
	load_x  t0, pxCurrentTCB			/* 加载 pxCurrentTCB 指针 */
	store_x  sp, 0( t0 )				/* 将 sp 写入 TCB 的第一个成员 (即 pxTopOfStack) */

	/* 7. 读取中断原因和异常地址 */
	csrr a0, scause
	csrr a1, sepc

test_if_asynchronous:
	/* 检查 scause 的最高位 (MSB)。如果是 1，表示是异步中断；如果是 0，表示是同步异常。 */
	srli a2, a0, __riscv_xlen - 1		/* 将 MSB 移到最低位 */
	beq a2, x0, handle_synchronous		/* 如果 MSB 为 0，跳转到同步异常handle_synchronous处理 */
	
	store_x a1, 0( sp )					/* 是异步中断，保存未修改的 sepc（返回地址） */

handle_asynchronous:

test_if_ipi:
	/* 检查是否为软件中断 (IPI) */
	addi t0, x0, 1
	slli t0, t0, __riscv_xlen - 1   /* 构造掩码：0x80...000 */
	addi t1, t0, 1					/* 构造比较值：0x80...0001 (Supervisor Software Interrupt) */
	bne a0, t1, test_if_mtimer      /* 如果不是 IPI，检查定时器中断 */

	load_x sp, xISRStackTop			/* 切换到中断栈,准备执行中断处理函数 */
	jal vPortClearIpiInterrupt      /* 清除 IPI 中断标志 */
	jal vTaskSwitchContext          /* 调用vTaskSwitchContext函数执行任务切换 */
	j processed_source              /* 处理完毕，跳转到恢复上下文 */

test_if_mtimer:						/* 检查是否为定时器中断 */
	addi t1, t1, 4					/* 0x80...0001 + 4 = 0x80...0005 (Supervisor Timer Interrupt) */
	bne a0, t1, test_if_external_interrupt

	load_x sp, xISRStackTop			/* 切换到中断栈,准备执行中断处理函数 */
	jal vPortSetupTimerInterrupt    /* 调用vPortSetupTimerInterrupt设置下一次定时器中断 */
	jal xTaskIncrementTick          /* 增加系统节拍，检查是否有任务超时唤醒 */
	beqz a0, processed_source		/* 如果 xTaskIncrementTick 返回 false (0)，说明无需任务切换，直接退出 */
	jal vTaskSwitchContext          /* 否则需要调用vTaskSwitchContext函数执行任务切换 */
	j processed_source

test_if_external_interrupt:			/* 检查是否为外部中断 */
	addi t1, t1, 4					/* 0x80...0005 + 4 = 0x80...0009 (Supervisor External Interrupt) */
	bne a0, t1, processed_trap   	/* 如果都不是，跳转到默认 Trap 处理 */

	load_x sp, xISRStackTop			/* 切换到 ISR 专用栈 */
	jal handle_interrupt	        /* 调用外部中断处理函数,weak函数，用户可以重写 */
	j processed_source

handle_synchronous:
	/* 处理同步异常（如 ecall） */
	addi a1, a1, 4					/* 将 sepc + 4，跳过产生异常的指令（例如 ecall 指令本身 4 字节） */
	store_x a1, 0( sp )				/* 保存更新后的返回地址 */

processed_trap:
	/* 统一的 Trap 处理（用于处理未知的异常或同步异常） */
	csrr a0, scause
	csrr a1, sepc
	csrr a2, stval
	mv   a4, sp
	load_x sp, xISRStackTop			/* 切换到 ISR 专用栈 */
	jal handle_trap	                /* 调用 C 语言的 handle_trap */
	j processed_source

processed_source:
	/* 8. 恢复上下文 */
	load_x  t1, pxCurrentTCB		/* 加载 pxCurrentTCB（可能已经被 vTaskSwitchContext 更新了） */
	load_x  sp, 0( t1 )				/* 从新任务的 TCB 中读取 sp */

	/* 恢复 sepc */
	load_x t0, 0 * portWORD_SIZE( sp )
	csrw sepc, t0

#ifdef __riscv_flen
	csrr t2, sstatus
#endif

	/* 恢复 sstatus */
	load_x  t0, 29 * portWORD_SIZE( sp )
	csrw sstatus, t0			

#ifdef __riscv_flen
	/* 恢复浮点寄存器 */
	li t0, SSTATUS_FS
	and t1,t2,t0
	bne t1,t0,1f

	load_fx f0, 30 * portWORD_SIZE( sp )
	load_fx f1, 31 * portWORD_SIZE( sp )
	load_fx f2, 32 * portWORD_SIZE( sp )
	load_fx f3, 33 * portWORD_SIZE( sp )
	load_fx f4, 34 * portWORD_SIZE( sp )
	load_fx f5, 35 * portWORD_SIZE( sp )
	load_fx f6, 36 * portWORD_SIZE( sp )
	load_fx f7, 37 * portWORD_SIZE( sp )
	load_fx f8, 38 * portWORD_SIZE( sp )
	load_fx f9, 39 * portWORD_SIZE( sp )
	load_fx f10, 40 * portWORD_SIZE( sp )
	load_fx f11, 41 * portWORD_SIZE( sp )
	load_fx f12, 42 * portWORD_SIZE( sp )
	load_fx f13, 43 * portWORD_SIZE( sp )
	load_fx f14, 44 * portWORD_SIZE( sp )
	load_fx f15, 45 * portWORD_SIZE( sp )
	load_fx f16, 46 * portWORD_SIZE( sp )
	load_fx f17, 47 * portWORD_SIZE( sp )
	load_fx f18, 48 * portWORD_SIZE( sp )
	load_fx f19, 49 * portWORD_SIZE( sp )
	load_fx f20, 50 * portWORD_SIZE( sp )
	load_fx f21, 51 * portWORD_SIZE( sp )
	load_fx f22, 52 * portWORD_SIZE( sp )
	load_fx f23, 53 * portWORD_SIZE( sp )
	load_fx f24, 54 * portWORD_SIZE( sp )
	load_fx f25, 55 * portWORD_SIZE( sp )
	load_fx f26, 56 * portWORD_SIZE( sp )
	load_fx f27, 57 * portWORD_SIZE( sp )
	load_fx f28, 58 * portWORD_SIZE( sp )
	load_fx f29, 59 * portWORD_SIZE( sp )
	load_fx f30, 60 * portWORD_SIZE( sp )
	load_fx f31, 61 * portWORD_SIZE( sp )
1:	
#endif

	/* 恢复通用寄存器 */
	load_x  x1, 1 * portWORD_SIZE( sp )
	load_x  x5, 2 * portWORD_SIZE( sp )		/* t0 */
	load_x  x6, 3 * portWORD_SIZE( sp )		/* t1 */
	load_x  x7, 4 * portWORD_SIZE( sp )		/* t2 */
	load_x  x8, 5 * portWORD_SIZE( sp )		/* s0/fp */
	load_x  x9, 6 * portWORD_SIZE( sp )		/* s1 */
	load_x  x10, 7 * portWORD_SIZE( sp )	/* a0 */
	load_x  x11, 8 * portWORD_SIZE( sp )	/* a1 */
	load_x  x12, 9 * portWORD_SIZE( sp )	/* a2 */
	load_x  x13, 10 * portWORD_SIZE( sp )	/* a3 */
	load_x  x14, 11 * portWORD_SIZE( sp )	/* a4 */
	load_x  x15, 12 * portWORD_SIZE( sp )	/* a5 */
	load_x  x16, 13 * portWORD_SIZE( sp )	/* a6 */
	load_x  x17, 14 * portWORD_SIZE( sp )	/* a7 */
	load_x  x18, 15 * portWORD_SIZE( sp )	/* s2 */
	load_x  x19, 16 * portWORD_SIZE( sp )	/* s3 */
	load_x  x20, 17 * portWORD_SIZE( sp )	/* s4 */
	load_x  x21, 18 * portWORD_SIZE( sp )	/* s5 */
	load_x  x22, 19 * portWORD_SIZE( sp )	/* s6 */
	load_x  x23, 20 * portWORD_SIZE( sp )	/* s7 */
	load_x  x24, 21 * portWORD_SIZE( sp )	/* s8 */
	load_x  x25, 22 * portWORD_SIZE( sp )	/* s9 */
	load_x  x26, 23 * portWORD_SIZE( sp )	/* s10 */
	load_x  x27, 24 * portWORD_SIZE( sp )	/* s11 */
	load_x  x28, 25 * portWORD_SIZE( sp )	/* t3 */
	load_x  x29, 26 * portWORD_SIZE( sp )	/* t4 */
	load_x  x30, 27 * portWORD_SIZE( sp )	/* t5 */
	load_x  x31, 28 * portWORD_SIZE( sp )	/* t6 */
	
	/* 释放栈空间 */
	addi sp, sp, portCONTEXT_SIZE

	/* 9. 从中断/异常返回 */
	sret
	.endfunc
/*-----------------------------------------------------------*/

.align 8
.weak handle_trap
.func
handle_trap:
	j handle_trap
	.endfunc
/*-----------------------------------------------------------*/

.align 8
.weak handle_interrupt
.func
handle_interrupt:
	j handle_interrupt
	ret
	.endfunc
/*-----------------------------------------------------------*/

/**
 * @brief 启动第一个任务。
 * 
 * 此函数由 xPortStartScheduler 调用。
 * 它不使用标准的函数返回机制，而是直接跳转到第一个任务的入口点。
 */
.align 8
.func
xPortStartFirstTask:
	/* 配置中断向量表基地址 stvec 指向 FreeRTOS 的 trap handler */
	la t0, freertos_risc_v_trap_handler
	csrw stvec, t0

	/* 获取当前最高优先级任务的 TCB */
	load_x  sp, pxCurrentTCB			/* Load pxCurrentTCB. */
	load_x  sp, 0( sp )				 	/* 从 TCB 中读取栈指针 sp */

	/* 0(sp)处是sepc，在我们伪造的栈帧中，sepc指向任务入口点pxCode，
	 * 所以这里加载0(sp)到x1(也就是ra)，作为ret的返回地址 */
	load_x  x1, 0( sp )

#ifdef __riscv_flen
	/* 恢复浮点寄存器 */
	load_fx f0, 30 * portWORD_SIZE( sp )
	load_fx f1, 31 * portWORD_SIZE( sp )
	load_fx f2, 32 * portWORD_SIZE( sp )
	load_fx f3, 33 * portWORD_SIZE( sp )
	load_fx f4, 34 * portWORD_SIZE( sp )
	load_fx f5, 35 * portWORD_SIZE( sp )
	load_fx f6, 36 * portWORD_SIZE( sp )
	load_fx f7, 37 * portWORD_SIZE( sp )
	load_fx f8, 38 * portWORD_SIZE( sp )
	load_fx f9, 39 * portWORD_SIZE( sp )
	load_fx f10, 40 * portWORD_SIZE( sp )
	load_fx f11, 41 * portWORD_SIZE( sp )
	load_fx f12, 42 * portWORD_SIZE( sp )
	load_fx f13, 43 * portWORD_SIZE( sp )
	load_fx f14, 44 * portWORD_SIZE( sp )
	load_fx f15, 45 * portWORD_SIZE( sp )
	load_fx f16, 46 * portWORD_SIZE( sp )
	load_fx f17, 47 * portWORD_SIZE( sp )
	load_fx f18, 48 * portWORD_SIZE( sp )
	load_fx f19, 49 * portWORD_SIZE( sp )
	load_fx f20, 50 * portWORD_SIZE( sp )
	load_fx f21, 51 * portWORD_SIZE( sp )
	load_fx f22, 52 * portWORD_SIZE( sp )
	load_fx f23, 53 * portWORD_SIZE( sp )
	load_fx f24, 54 * portWORD_SIZE( sp )
	load_fx f25, 55 * portWORD_SIZE( sp )
	load_fx f26, 56 * portWORD_SIZE( sp )
	load_fx f27, 57 * portWORD_SIZE( sp )
	load_fx f28, 58 * portWORD_SIZE( sp )
	load_fx f29, 59 * portWORD_SIZE( sp )
	load_fx f30, 60 * portWORD_SIZE( sp )
	load_fx f31, 61 * portWORD_SIZE( sp )
#endif

	/* 恢复通用寄存器 */
	load_x  x6, 3 * portWORD_SIZE( sp )		/* t1 */
	load_x  x7, 4 * portWORD_SIZE( sp )		/* t2 */
	load_x  x8, 5 * portWORD_SIZE( sp )		/* s0/fp */
	load_x  x9, 6 * portWORD_SIZE( sp )		/* s1 */
	load_x  x10, 7 * portWORD_SIZE( sp )	/* a0 */
	load_x  x11, 8 * portWORD_SIZE( sp )	/* a1 */
	load_x  x12, 9 * portWORD_SIZE( sp )	/* a2 */
	load_x  x13, 10 * portWORD_SIZE( sp )	/* a3 */
	load_x  x14, 11 * portWORD_SIZE( sp )	/* a4 */
	load_x  x15, 12 * portWORD_SIZE( sp )	/* a5 */
	load_x  x16, 13 * portWORD_SIZE( sp )	/* a6 */
	load_x  x17, 14 * portWORD_SIZE( sp )	/* a7 */
	load_x  x18, 15 * portWORD_SIZE( sp )	/* s2 */
	load_x  x19, 16 * portWORD_SIZE( sp )	/* s3 */
	load_x  x20, 17 * portWORD_SIZE( sp )	/* s4 */
	load_x  x21, 18 * portWORD_SIZE( sp )	/* s5 */
	load_x  x22, 19 * portWORD_SIZE( sp )	/* s6 */
	load_x  x23, 20 * portWORD_SIZE( sp )	/* s7 */
	load_x  x24, 21 * portWORD_SIZE( sp )	/* s8 */
	load_x  x25, 22 * portWORD_SIZE( sp )	/* s9 */
	load_x  x26, 23 * portWORD_SIZE( sp )	/* s10 */
	load_x  x27, 24 * portWORD_SIZE( sp )	/* s11 */
	load_x  x28, 25 * portWORD_SIZE( sp )	/* t3 */
	load_x  x29, 26 * portWORD_SIZE( sp )	/* t4 */
	load_x  x30, 27 * portWORD_SIZE( sp )	/* t5 */
	load_x  x31, 28 * portWORD_SIZE( sp )	/* t6 */

	/* 恢复 sstatus 的初始值 (启用中断等) */
	load_x  x5, 29 * portWORD_SIZE( sp )	/* Initial mstatus into x5 (t0) */
	ori x5, x5, 0x2					    	/* 设置 SIE (Supervisor Interrupt Enable) 位，确保任务启动时开中断。 */
	csrrw  x0, sstatus, x5					/* 写入 sstatus，中断从此启用 */
	
	load_x  x5, 2 * portWORD_SIZE( sp )		/* 恢复 x5 (t0) */

	/* sp上移，释放栈空间 */
	addi	sp, sp, portCONTEXT_SIZE
	
	/* 使用ret返回。这将跳转到x1/ra(保存了任务入口点pxCode)执行 */
	ret
	.endfunc
/*-----------------------------------------------------------*/

/**
 * @brief 初始化新任务的栈
 * 
 * 当使用 xTaskCreate() 或 xTaskCreateStatic() 创建新任务时，FreeRTOS会调用此函数
 * 来初始化该任务的栈帧
 *
 * @b 功能描述:
 * 它的作用是伪装一个“被中断”的任务上下文。这样，当调度器第一次切换到该任务时，
 * 可以通过执行上下文恢复代码,自然地跳转到任务的入口函数开始执行。
 *
 * 在 RISC-V S 模式下，这通常涉及：
 * 1. 预留空间保存所有通用寄存器 (x1, x5-x31)。
 * 2. 设置 sepc (Supervisor Exception Program Counter) 为任务入口地址。
 * 3. 设置 sstatus (Supervisor Status) 寄存器，确保进入任务时中断被启用 (SPIE=1)
 *    且特权级正确 (SPP=1 for S-mode)。
 * 4. 将任务参数 (pvParameters) 放入 a0 寄存器对应的栈位置。
 * 5. 设置返回地址 (ra) 为错误处理函数 (portTASK_RETURN_ADDRESS)，以防任务意外返回。 *
 * @param pxTopOfStack 分配给任务的栈内存的顶部地址。
 * @param pxCode 任务函数的入口地址。
 * @param pvParameters 传递给任务函数的参数。
 * @return StackType_t* 初始化后的新栈顶地址。
 */
.align 8
.func
pxPortInitialiseStack:
	csrr t0, sstatus					/* 获取当前sstatus */
	la t1, sstatus_init                 /* 加载预设的sstatus初始化值 */
	lw t2, 0(t1)
	or t0, t0, t2                       /* 合并设置 */
#ifdef __riscv_flen
	lw t2, 4(t1)
	and t0, t0, t2
#endif
	andi t0, t0, ~0x2					/* 确保初始时中断是禁用的(调度时关中断SIE=0)*/
	addi t1, x0, 0x120					/* 设置 SPIE=1 (Previous Interrupt Enable) 和 SPP=1 (Previous Mode = Supervisor) */
	or t0, t0, t1						/* 合并到 sstatus 值中 */

#ifdef __riscv_flen
	addi a0, a0, -(32 * portWORD_SIZE)	/* 为浮点寄存器预留空间 */
#endif
	addi a0, a0, -portWORD_SIZE
	store_x t0, 0(a0)					/* 将 sstatus 压栈 */
	addi a0, a0, -(22 * portWORD_SIZE)	/* 为通用寄存器预留空间 (x11-x31) */
	store_x a2, 0(a0)					/* 将 pvParameters (a2) 存入栈中对应 a0 (x10) 的位置 */
	addi a0, a0, -(6 * portWORD_SIZE)	/* 为通用寄存器预留空间 (x5-x9) */
	la t0, prvTaskExitError
	store_x t0, 0(a0)					/* 将错误处理函数地址存入栈中对应 ra (x1) 的位置 */

	addi a0, a0, -portWORD_SIZE         /* 为 sepc预留空间 */
	store_x a1, 0(a0)					/* 将任务入口地址 (a1:pxCode) 存入栈中对应 sepc 的位置 */
	
	/* 此时 a0 已经是新的栈顶指针，直接返回 */
	ret

sstatus_init:
	.word (~SSTATUS_FS)                 /* 禁用浮点状态 (FS) */
#ifdef __riscv_flen
	.word (0x1 << 13)                   /* 如果有 FPU，设置 FS 初始状态 */
#endif
	.endfunc
/*-----------------------------------------------------------*/