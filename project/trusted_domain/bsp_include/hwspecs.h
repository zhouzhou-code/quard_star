/*
 * Quard Star Hardware Specifications
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件定义 Linux 和 FreeRTOS 都需要引用的硬件规格常量
 * 作为单一数据源，确保跨域定义的一致性
 */

#ifndef QUARD_STAR_HWSPECS_H
#define QUARD_STAR_HWSPECS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 共享内存配置 (Shared Memory for OpenAMP/RPMsg)
 * ============================================================================ */

/**
 * 共享内存物理基地址
 * 用于 vring0, vring1, rpmsg buffers 和 Resource Table
 */
#define SHM_BASE_ADDR           0xbf700000

/**
 * 共享内存总大小 (128KB)
 * 分配：
 *   - vring0 (RX):      8KB  @ 0xbf700000 - 0xbf701fff
 *   - vring1 (TX):      8KB  @ 0xbf702000 - 0xbf703fff
 *   - rpmsg buffers:   32KB  @ 0xbf704000 - 0xbf70bfff
 *   - Resource Table:   4KB  @ 0xbf70c000 - 0xbf70cfff
 *   - Reserved:        76KB  @ 0xbf70d000 - 0xbf71ffff
 */
#define SHM_SIZE                (128 * 1024)

/* VirtIO/vring 配置（固定地址契约） */
/*
 * VRING_SIZE 决定了每个 vring 的描述符数量
 * 总共有 2 个 vring (TX + RX)，每个描述符对应一个 512 Byte 的 RPMsg Buffer
 *
 * 计算：
 * - DMA 池大小：32 KB = 32,768 Bytes
 * - 每个 Buffer：512 Bytes
 * - 总 Buffer 数 = 32,768 / 512 = 64 个
 * - 每个 vring = 64 / 2 = 32 个描述符
 */
#define VRING_SIZE              32
#define VRING_ALIGN             4096
#define VRING0_SIZE             (8 * 1024)    /* 0xbf700000 - 0xbf701fff */
#define VRING1_SIZE             (8 * 1024)    /* 0xbf702000 - 0xbf703fff */
#define RPMSG_BUF_SIZE          (32 * 1024)   /* 0xbf704000 - 0xbf70bfff */

/* ============================================================================
 * UART 配置
 * ============================================================================ */

/* UART2 基地址（QEMU 16550 UART）- FreeRTOS 专用 */
#define UART_BASE           0x10002000  /* UART2: 0x10002000 */

/* UART 时钟频率（QEMU 中通常使用 1843200 Hz 或系统时钟）*/
#define UART_CLK_FREQ       1843200

/* UART 波特率 */
#define UART_BAUDRATE       115200

/* 固定共享内存布局 */
#define VRING0_PA               (SHM_BASE_ADDR + 0x0000)
#define VRING1_PA               (SHM_BASE_ADDR + 0x2000)
#define RPMSG_BUF_PA            (SHM_BASE_ADDR + 0x4000)

/* Resource Table 固定在共享内存尾部 */
#define RESOURCE_TABLE_OFFSET   0xc000
#define RESOURCE_TABLE_ADDR     (SHM_BASE_ADDR + RESOURCE_TABLE_OFFSET)
#define RESOURCE_TABLE_SIZE     (4 * 1024)

/* ============================================================================
 * Mailbox 中断号映射 (Mailbox IRQ Mapping)
 * ============================================================================ */

/* ============================================================================
 * 硬件寄存器基址
 * ============================================================================ */

/**
 * PLIC (Platform-Level Interrupt Controller) 基地址
 * QEMU RISC-V 标准 PLIC 地址
 */
#define PLIC_BASE               0x0C000000

/**
 * CLINT (Core-Local Interrupt Controller) 基地址
 */
#define CLINT_BASE              0x02000000

/**
 * Mailbox 控制器寄存器基地址
 */
#define MAILBOX_BASE_ADDR       0x10004000
#define MAILBOX_REG_SIZE        0x1000

/**
 * Mailbox 寄存器偏移
 */
#define REG_LINUX_TRIG          0x00    /* 触发中断到 Linux (WO) */
#define REG_LINUX_ACK           0x04    /* 清除 Linux 中断 (W1C) */
#define REG_LINUX_STAT          0x08    /* Linux 中断状态 (RO) */
#define REG_LINUX_IE            0x0C    /* Linux 中断使能 (RW) */

#define REG_RTOS_TRIG           0x20    /* 触发中断到 FreeRTOS (WO) */
#define REG_RTOS_ACK            0x24    /* 清除 FreeRTOS 中断 (W1C) */
#define REG_RTOS_STAT           0x28    /* FreeRTOS 中断状态 (RO) */
#define REG_RTOS_IE             0x2C    /* FreeRTOS 中断使能 (RW) */

#define REG_REVISION            0x40    /* IP 版本号 (RO) */

/**
 * PLIC 中断号
 *
 * 硬件连接：
 *   - FreeRTOS → Linux: PLIC input 50 (Linux Hart 1-7)
 *   - Linux → FreeRTOS: PLIC input 51 (FreeRTOS Hart 0)
 *
 * QEMU 源码参考：qemu-8.0.2/hw/riscv/quard_star.c:279-280
 */
#define MAILBOX_IRQ_TO_LINUX    50
#define MAILBOX_IRQ_TO_RTOS     51

/* Linux 端 mailbox 中断号（设备树 interrupts = <50>） */
#define LINUX_MAILBOX_IRQ       MAILBOX_IRQ_TO_LINUX

/* ============================================================================
 * OpenAMP/RPMsg 配置
 * ============================================================================ */

/**
 * RPMsg 端点地址
 */
#define RPMSG_ADDR_ANY          0xFFFFFFFF
#define RPMSG_EPT_ADDR          (0x100)  /* 本端地址 */

/* ============================================================================
 * 调试与日志
 * ============================================================================ */

#define OPENAMP_LOG_LEVEL       2  /* 0=OFF, 1=ERR, 2=WARN, 3=INFO, 4=DEBUG */

#ifdef __cplusplus
}
#endif

#endif /* QUARD_STAR_HWSPECS_H */
