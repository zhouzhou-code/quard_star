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
 * VirtIO 配置状态位
 * ============================================================================ */

/**
 * VIRTIO_CONFIG_S_DRIVER_OK - 驱动程序已就绪
 *
 * 当 VirtIO 驱动完成初始化并准备好处理 I/O 时，
 * 会将此位写入设备的 status 字段。
 *
 * 在本系统中：
 * - Linux 设置此位 → 通知 FreeRTOS 可以开始 RPMsg 通信
 * - FreeRTOS 等待此位 → 确认 Linux 驱动已就绪
 */
#define VIRTIO_CONFIG_S_DRIVER_OK   0x04

/* ============================================================================
 * 硬件配置
 * ============================================================================ */

/**
 * 共享内存配置
 */
#define QUARD_STAR_RPROC_SHM_PA        0xbf700000  /* 共享内存物理地址 */
#define QUARD_STAR_RPROC_SHM_SIZE      (128 * 1024) /* 128KB */

/* 固定共享内存布局（与 FreeRTOS 侧 hwspecs.h 保持一致） */
#define QUARD_STAR_VRING0_PA           0xbf700000
#define QUARD_STAR_VRING1_PA           0xbf702000
#define QUARD_STAR_VRING_SIZE          (8 * 1024)
#define QUARD_STAR_RPMSG_BUF_PA        0xbf704000
#define QUARD_STAR_RPMSG_BUF_SIZE      (32 * 1024)

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

/*
 * QS-Mailbox v2：doorbell 编程范式参考 ARM MHU v1/v2，自主实现。
 * 双 bank：to-Linux @0x000 (IRQ50，本驱动是接收方)；to-RTOS @0x100 (IRQ51，本驱动是发送方)。
 * 每通道 stride 0x20：STAT(0x00,RO)/SET(0x04,W1S)/CLEAR(0x08,W1C)/MASK_*(0x0C-0x14)。
 * rpmsg 用 channel0 bit0 作 notify doorbell。
 */
#define QSMB_BANK_TO_LINUX             0x000
#define QSMB_BANK_TO_RTOS              0x100
#define QSMB_R_SET                     0x04
#define QSMB_R_CLEAR                   0x08
#define QSMB_R_MASK_CLEAR              0x14
#define QSMB_RPMSG_DBELL               0x1    /* channel0 bit0 */
#define QSMB_TL_CLEAR                  (QSMB_BANK_TO_LINUX + QSMB_R_CLEAR)       /* 0x08 */
#define QSMB_TL_MASK_CLEAR             (QSMB_BANK_TO_LINUX + QSMB_R_MASK_CLEAR)  /* 0x14 */
#define QSMB_TR_SET                    (QSMB_BANK_TO_RTOS  + QSMB_R_SET)         /* 0x104 */

#endif /* _QUARD_STAR_RPROC_H */
