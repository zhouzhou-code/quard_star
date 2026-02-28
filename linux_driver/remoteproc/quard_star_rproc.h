/*
 * Quard Star Remoteproc Driver
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件定义 Quard Star SoC 的 remoteproc 平台驱动常量和结构
 * 用于管理 FreeRTOS (Hart 0) 作为远程处理器
 */

#ifndef _QUARD_STAR_RPROC_H
#define _QUARD_STAR_RPROC_H

/* ============================================================================
 * 硬件配置
 * ============================================================================ */

/**
 * 共享内存配置
 */
#define QUARD_STAR_RPROC_SHM_PA        0xbf700000  /* 共享内存物理地址 */
#define QUARD_STAR_RPROC_SHM_SIZE      (128 * 1024) /* 128KB */

/**
 * Resource Table 配置
 */
#define QUARD_STAR_RPROC_RSC_PA        0xbf70c000  /* Resource Table 物理地址 */
#define QUARD_STAR_RPROC_RSC_SIZE      (4 * 1024)  /* 4KB */

/**
 * Mailbox IRQ 配置
 * Linux 收到 IRQ 50 (FreeRTOS → Linux)
 */
#define QUARD_STAR_RPROC_VQ_IRQ        50  /* virtqueue kick IRQ */

/**
 * Mailbox 寄存器配置
 */
#define QUARD_STAR_MAILBOX_PA          0x10004000
#define QUARD_STAR_MAILBOX_SIZE        0x1000

#define MAILBOX_REG_LINUX_TRIG         0x00  /* 触发中断到 FreeRTOS (WO) */
#define MAILBOX_REG_LINUX_ACK          0x04  /* 清除 Linux 中断 (W1C) */
#define MAILBOX_REG_LINUX_STAT         0x08  /* Linux 中断状态 (RO) */
#define MAILBOX_REG_LINUX_IE           0x0C  /* Linux 中断使能 (RW) */

#define MAILBOX_REG_RTOS_TRIG          0x20  /* 触发中断到 FreeRTOS (WO) */
#define MAILBOX_REG_RTOS_ACK           0x24  /* 清除 FreeRTOS 中断 (W1C) */
#define MAILBOX_REG_RTOS_STAT          0x28  /* FreeRTOS 中断状态 (RO) */
#define MAILBOX_REG_RTOS_IE            0x2C  /* FreeRTOS 中断使能 (RW) */

#endif /* _QUARD_STAR_RPROC_H */
