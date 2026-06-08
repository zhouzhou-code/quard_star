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
 * 标准 ARM MHU(SSE-200, hw/misc/armsse-mhu.c)寄存器模型：
 *   CPU0 方向 = to-Linux(IRQ50，本驱动是接收方)：STAT 0x00 / SET 0x04 / CLR 0x08
 *   CPU1 方向 = to-RTOS (IRQ51，本驱动是发送方)：STAT 0x10 / SET 0x14 / CLR 0x18
 * 中断 = (STATy != 0)，写 CLR 自动落下；无 mask 寄存器。doorbell 用 bit0。
 * 设备身份用 ARM Primecell CID 魔数 0xB105F00D 校验(CID0..3 @0xFF0..0xFFC)。
 */
#define MHU_CPU0_STAT                  0x00   /* to-Linux STAT (RO) */
#define MHU_CPU0_CLR                   0x08   /* to-Linux CLR (W1C)：Linux 应答 */
#define MHU_CPU1_SET                   0x14   /* to-RTOS SET (W1S)：Linux 通知 FreeRTOS */
#define MHU_RPMSG_DBELL                0x1    /* doorbell bit0 */
#define MHU_CIDR0                      0xFF0  /* Primecell Component ID byte0 */
#define MHU_CID_MAGIC                  0xB105F00DU

#endif /* _QUARD_STAR_RPROC_H */
