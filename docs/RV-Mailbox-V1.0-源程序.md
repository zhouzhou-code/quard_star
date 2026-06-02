# RV-Mailbox 多核邮箱控制器及驱动软件 V1.0
## 源程序清单（软件著作权登记附件）

> 软件全称：RV-Mailbox 多核邮箱控制器及驱动软件　|　版本：V1.0　|　语言：C
> 本清单为软件全部核心源代码，由开发者独立编写，享有完整著作权。
> 源代码总计约 1829 行（7 个源文件）。

---

## 文件：quard_star_mailbox.h（设备模型头：寄存器布局/状态结构）

路径：`source/qemu-8.0.2/include/hw/riscv/quard_star_mailbox.h`

```c
/*
 * QEMU RISC-V RV-Mailbox Controller (Quard Star machine)
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 参考 ARM MHU v1/v2 的 doorbell 编程模型，自主设计实现的多通道邮箱控制器。
 * 与 ARM MHU 无源码/RTL 关联，仅在寄存器编程范式上对齐（SET/CLEAR/STAT + MASK）。
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#ifndef HW_RISCV_QUARD_STAR_MAILBOX_H
#define HW_RISCV_QUARD_STAR_MAILBOX_H

#include "hw/sysbus.h"
#include "qom/object.h"
#include "qemu/typedefs.h"

#define TYPE_QUARD_STAR_MAILBOX_DEVICE "quard-star-mailbox"
OBJECT_DECLARE_SIMPLE_TYPE(QuardStarMailboxState, QUARD_STAR_MAILBOX_DEVICE)

/* 通道数（对齐 ARM MHU v1 经典 low/high/secure 三通道） */
#define RVMB_NUM_CHANNELS   3
/* bank 索引：0 = 发往 Linux(IRQ50)，1 = 发往 FreeRTOS(IRQ51) */
#define RVMB_BANK_TO_LINUX  0
#define RVMB_BANK_TO_RTOS   1
#define RVMB_NUM_BANKS      2

/*
 * 寄存器映射（每 bank 基址：to-Linux=0x000, to-RTOS=0x100；通道 stride=0x20）
 *   +0x00 CHx_STAT       (RO)  doorbell 状态(32 位，每位一个事件)
 *   +0x04 CHx_SET        (W1S) 置位 → 可能拉高中断（发送方写）
 *   +0x08 CHx_CLEAR      (W1C) 清位 → 全清后中断落下（接收方写）
 *   +0x0C CHx_MASK_STAT  (RO)  中断屏蔽(1=屏蔽)
 *   +0x10 CHx_MASK_SET   (W1S) 置屏蔽位
 *   +0x14 CHx_MASK_CLEAR (W1C) 清屏蔽位(=使能该位中断)
 * 设备级:
 *   0x0F0 REVISION(RO)=0x0100(v1.0)  0x0F4 NUM_CHANNELS  0x0F8 NUM_BANKS
 * 中断逻辑(电平、合并中断（MHU v2 风格）):
 *   irq_linux = OR_ch( stat[TL][ch] & ~mask[TL][ch] ) != 0
 *   irq_rtos  = OR_ch( stat[TR][ch] & ~mask[TR][ch] ) != 0
 */
struct QuardStarMailboxState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    MemoryRegion iomem;

    qemu_irq irq_linux;   /* to-Linux bank → PLIC input 50 */
    qemu_irq irq_rtos;    /* to-RTOS  bank → PLIC input 51 */

    /* [bank][channel]：doorbell 状态与屏蔽位 */
    uint32_t stat[RVMB_NUM_BANKS][RVMB_NUM_CHANNELS];
    uint32_t mask[RVMB_NUM_BANKS][RVMB_NUM_CHANNELS];
};

#endif /* HW_RISCV_QUARD_STAR_MAILBOX_H */
```

---

## 文件：quard_star_mailbox.c（QEMU 邮箱设备模型主体）

路径：`source/qemu-8.0.2/hw/riscv/quard_star_mailbox.c`

```c
/*
 * QEMU RISC-V RV-Mailbox Controller (Quard Star machine)
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 多通道 doorbell 邮箱，用于 FreeRTOS(Hart7) 与 Linux(Hart0-6) 的 AMP 跨核通信。
 * 寄存器编程范式参考 ARM MHU v1/v2（SET/CLEAR/STAT + MASK），自主设计实现，
 * 与 ARM 源码/RTL 无关联。
 *
 * 双 bank（to-Linux / to-RTOS）× 3 通道 × 32-bit doorbell；合并中断（MHU v2 风格）：
 * 某 bank 内任一通道 (STAT & ~MASK) != 0 即拉高该 bank 对应的中断线（电平）。
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "migration/vmstate.h"
#include "hw/sysbus.h"
#include "hw/irq.h"
#include "hw/riscv/quard_star_mailbox.h"

/* 每 bank 基址与通道布局 */
#define RVMB_BANK_TL_BASE   0x000        /* to-Linux  (IRQ50) */
#define RVMB_BANK_TR_BASE   0x100        /* to-RTOS   (IRQ51) */
#define RVMB_CH_STRIDE      0x20
#define RVMB_BANK_SPAN      (RVMB_NUM_CHANNELS * RVMB_CH_STRIDE)   /* 0x60 */

/* 通道内寄存器偏移 */
#define R_STAT              0x00         /* RO  */
#define R_SET               0x04         /* W1S */
#define R_CLEAR             0x08         /* W1C */
#define R_MASK_STAT         0x0C         /* RO  */
#define R_MASK_SET          0x10         /* W1S */
#define R_MASK_CLEAR        0x14         /* W1C */

/* 设备级寄存器 */
#define R_REVISION          0x0F0        /* RO，0x0100 = v1.0 */
#define R_NUM_CHANNELS      0x0F4        /* RO */
#define R_NUM_BANKS         0x0F8        /* RO */
#define RVMB_REVISION       0x0100

/* 把 MMIO 偏移解码成 (bank, channel, 通道内寄存器)；返回 false 表示非通道区 */
static bool qsmb_decode(hwaddr offset, int *bank, int *ch, hwaddr *reg)
{
    hwaddr o;
    if (offset < RVMB_BANK_TL_BASE + RVMB_BANK_SPAN) {   /* TL_BASE=0 */
        *bank = RVMB_BANK_TO_LINUX;
        o = offset - RVMB_BANK_TL_BASE;
    } else if (offset >= RVMB_BANK_TR_BASE && offset < RVMB_BANK_TR_BASE + RVMB_BANK_SPAN) {
        *bank = RVMB_BANK_TO_RTOS;
        o = offset - RVMB_BANK_TR_BASE;
    } else {
        return false;
    }
    *ch = o / RVMB_CH_STRIDE;
    *reg = o % RVMB_CH_STRIDE;
    return true;
}

/* 重新计算某 bank 的中断线电平（合并中断，MHU v2 风格） */
static void qsmb_update_irq(QuardStarMailboxState *s, int bank)
{
    uint32_t pending = 0;
    int ch;
    for (ch = 0; ch < RVMB_NUM_CHANNELS; ch++) {
        pending |= s->stat[bank][ch] & ~s->mask[bank][ch];
    }
    qemu_set_irq(bank == RVMB_BANK_TO_LINUX ? s->irq_linux : s->irq_rtos,
                 pending ? 1 : 0);
}

static uint64_t quard_star_mailbox_read(void *opaque, hwaddr offset,
                                        unsigned size)
{
    QuardStarMailboxState *s = QUARD_STAR_MAILBOX_DEVICE(opaque);
    int bank, ch;
    hwaddr reg;

    switch (offset) {
    case R_REVISION:
        return RVMB_REVISION;
    case R_NUM_CHANNELS:
        return RVMB_NUM_CHANNELS;
    case R_NUM_BANKS:
        return RVMB_NUM_BANKS;
    }

    if (!qsmb_decode(offset, &bank, &ch, &reg)) {
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: invalid read @0x%02"HWADDR_PRIx"\n", __func__, offset);
        return 0;
    }

    switch (reg) {
    case R_STAT:
        return s->stat[bank][ch];
    case R_MASK_STAT:
        return s->mask[bank][ch];
    case R_SET:
    case R_CLEAR:
    case R_MASK_SET:
    case R_MASK_CLEAR:
        /* 只写寄存器读返回 0 */
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: read WO reg @0x%02"HWADDR_PRIx"\n", __func__, offset);
        return 0;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: read reserved @0x%02"HWADDR_PRIx"\n", __func__, offset);
        return 0;
    }
}

static void quard_star_mailbox_write(void *opaque, hwaddr offset,
                                     uint64_t value, unsigned size)
{
    QuardStarMailboxState *s = QUARD_STAR_MAILBOX_DEVICE(opaque);
    uint32_t val = (uint32_t)value;
    int bank, ch;
    hwaddr reg;

    switch (offset) {
    case R_REVISION:
    case R_NUM_CHANNELS:
    case R_NUM_BANKS:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: write RO reg @0x%02"HWADDR_PRIx"\n", __func__, offset);
        return;
    }

    if (!qsmb_decode(offset, &bank, &ch, &reg)) {
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: invalid write @0x%02"HWADDR_PRIx"\n", __func__, offset);
        return;
    }

    switch (reg) {
    case R_SET:                 /* W1S：置 doorbell 位 */
        s->stat[bank][ch] |= val;
        qsmb_update_irq(s, bank);
        break;
    case R_CLEAR:               /* W1C：清 doorbell 位 */
        s->stat[bank][ch] &= ~val;
        qsmb_update_irq(s, bank);
        break;
    case R_MASK_SET:            /* W1S：置屏蔽位（关该位中断）*/
        s->mask[bank][ch] |= val;
        qsmb_update_irq(s, bank);
        break;
    case R_MASK_CLEAR:          /* W1C：清屏蔽位（开该位中断）*/
        s->mask[bank][ch] &= ~val;
        qsmb_update_irq(s, bank);
        break;
    case R_STAT:
    case R_MASK_STAT:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: write RO reg @0x%02"HWADDR_PRIx"\n", __func__, offset);
        break;
    default:
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: write reserved @0x%02"HWADDR_PRIx"\n", __func__, offset);
        break;
    }
}

static const MemoryRegionOps quard_star_mailbox_ops = {
    .read = quard_star_mailbox_read,
    .write = quard_star_mailbox_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .impl.min_access_size = 4,
    .impl.max_access_size = 4,
    .valid.min_access_size = 4,
    .valid.max_access_size = 4,
};

static void quard_star_mailbox_init(Object *obj)
{
    QuardStarMailboxState *s = QUARD_STAR_MAILBOX_DEVICE(obj);
    SysBusDevice *sbd = SYS_BUS_DEVICE(obj);

    memory_region_init_io(&s->iomem, obj, &quard_star_mailbox_ops,
                          s, "quard-star-mailbox", 0x1000);
    sysbus_init_mmio(sbd, &s->iomem);

    sysbus_init_irq(sbd, &s->irq_linux);   /* to-Linux bank → PLIC input 50 */
    sysbus_init_irq(sbd, &s->irq_rtos);    /* to-RTOS  bank → PLIC input 51 */
}

static void quard_star_mailbox_reset(DeviceState *dev)
{
    QuardStarMailboxState *s = QUARD_STAR_MAILBOX_DEVICE(dev);
    int b, ch;

    /* 复位：状态全 0；屏蔽全 1（默认全部屏蔽，接收方需显式 unmask）*/
    for (b = 0; b < RVMB_NUM_BANKS; b++) {
        for (ch = 0; ch < RVMB_NUM_CHANNELS; ch++) {
            s->stat[b][ch] = 0;
            s->mask[b][ch] = 0xFFFFFFFF;
        }
    }
    qemu_set_irq(s->irq_linux, 0);
    qemu_set_irq(s->irq_rtos, 0);
}

static const VMStateDescription vmstate_quard_star_mailbox = {
    .name = TYPE_QUARD_STAR_MAILBOX_DEVICE,
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_UINT32_2DARRAY(stat, QuardStarMailboxState,
                               RVMB_NUM_BANKS, RVMB_NUM_CHANNELS),
        VMSTATE_UINT32_2DARRAY(mask, QuardStarMailboxState,
                               RVMB_NUM_BANKS, RVMB_NUM_CHANNELS),
        VMSTATE_END_OF_LIST()
    }
};

static void quard_star_mailbox_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = quard_star_mailbox_reset;
    dc->desc = "RV-Mailbox Controller (multi-channel doorbell)";
    dc->vmsd = &vmstate_quard_star_mailbox;
    set_bit(DEVICE_CATEGORY_MISC, dc->categories);
}

static const TypeInfo quard_star_mailbox_type_info = {
    .name = TYPE_QUARD_STAR_MAILBOX_DEVICE,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(QuardStarMailboxState),
    .instance_init = quard_star_mailbox_init,
    .class_init = quard_star_mailbox_class_init,
};

static void quard_star_mailbox_register_types(void)
{
    type_register_static(&quard_star_mailbox_type_info);
}

type_init(quard_star_mailbox_register_types)
```

---

## 文件：quard_star_rproc.h（remoteproc 驱动头：RVMB 寄存器偏移）

路径：`project/drivers/remoteproc/quard_star_rproc.h`

```c
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
 * RV-Mailbox：doorbell 编程范式参考 ARM MHU v1/v2，自主实现。
 * 双 bank：to-Linux @0x000 (IRQ50，本驱动是接收方)；to-RTOS @0x100 (IRQ51，本驱动是发送方)。
 * 每通道 stride 0x20：STAT(0x00,RO)/SET(0x04,W1S)/CLEAR(0x08,W1C)/MASK_*(0x0C-0x14)。
 * rpmsg 用 channel0 bit0 作 notify doorbell。
 */
#define RVMB_BANK_TO_LINUX             0x000
#define RVMB_BANK_TO_RTOS              0x100
#define RVMB_R_SET                     0x04
#define RVMB_R_CLEAR                   0x08
#define RVMB_R_MASK_CLEAR              0x14
#define RVMB_RPMSG_DBELL               0x1    /* channel0 bit0 */
#define RVMB_TL_CLEAR                  (RVMB_BANK_TO_LINUX + RVMB_R_CLEAR)       /* 0x08 */
#define RVMB_TL_MASK_CLEAR             (RVMB_BANK_TO_LINUX + RVMB_R_MASK_CLEAR)  /* 0x14 */
#define RVMB_TR_SET                    (RVMB_BANK_TO_RTOS  + RVMB_R_SET)         /* 0x104 */

#endif /* _QUARD_STAR_RPROC_H */
```

---

## 文件：quard_star_rproc.c（Linux remoteproc 驱动）

路径：`project/drivers/remoteproc/quard_star_rproc.c`

```c
/*
 * Quard Star Remoteproc Driver Implementation
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件实现 Quard Star SoC 的 remoteproc 平台驱动，用于管理 FreeRTOS (Hart 0)
 * 作为远程处理器，实现 AMP 异构多处理器通信。
 *
 * ============================================================================
 * 架构背景
 * ============================================================================
 *
 * 本系统基于 RISC-V OpenSBI Domain 机制实现资源隔离：
 * - Hart 7: FreeRTOS 实时域 (由 OpenSBI Domains 独立启动)
 * - Hart 0-6: Linux 计算域 (由 OpenSBI Domains 独立启动)
 *
 * 两个域同步启动，Linux 不负责 FreeRTOS 的固件加载与复位。
 * 因此本驱动工作在 "Attach-Only" (纯附加) 模式，仅负责：
 * 1. 读取 FreeRTOS 在共享内存中准备好的 Resource Table
 * 2. 解析 VirtIO/vring 配置
 * 3. 注册 Mailbox 中断处理 (IRQ 50)
 * 4. 提供 VirtIO notify 机制
 *
 * ============================================================================
 * 通信机制
 * ============================================================================
 *
 * 【共享内存布局】(与 FreeRTOS 侧 trusted_domain/bsp_include/hwspecs.h 保持一致)
 * 0xbf700000 - 0xbf701fff: vring0 (TX, 8KB)
 * 0xbf702000 - 0xbf703fff: vring1 (RX, 8KB)
 * 0xbf704000 - 0xbf70bfff: rpmsg buffers (32KB)
 * 0xbf70c000 - 0xbf70cfff: Resource Table (4KB) ← 静态初始化，双核同步启动时序安全
 *
 * 【Mailbox 中断机制】
 * - FreeRTOS → Linux: 写 MAILBOX_REG_LINUX_TRIG → 触发 IRQ 50 → Linux ISR 处理
 * - Linux → FreeRTOS: 写 MAILBOX_REG_RTOS_TRIG → 触发 IRQ 51 → FreeRTOS ISR 处理
 *
 * 【VirtIO/RPMsg 协议栈】
 * Linux                     FreeRTOS
 *   |                          |
 *   |-- RPMsg Driver          |-- RPMsg Endpoint
 *   |                          |
 *   |-- Virtio RPMsg          |-- Virtio Device
 *   |        |                 |       |
 *   |-- vring0 (TX) ---------> vring1 (RX)
 *   |                          |
 *   |-- vring1 (RX) <--------- vring0 (TX)
 *            \                 /
 *             \               /
 *            remoteproc.kick  virtio_notify()
 *                 |              |
 *           Mailbox IRQ 50   Mailbox IRQ 51
 *
 * ============================================================================
 * 使用方法
 * ============================================================================
 *
 * 1. 编译驱动：
 *    cd linux_driver/remoteproc
 *    make
 *
 * 2. 加载驱动：
 *    insmod quard_star_rproc.ko
 *
 * 3. 附加到 FreeRTOS：
 *    echo start > /sys/class/remoteproc/remoteproc0/state
 *
 * 4. 检查 RPMsg 设备：
 *    ls -l /dev/rpmsg*
 *
 * 5. 测试通信：
 *    # 发送消息到 FreeRTOS
 *    echo "Hello FreeRTOS" > /dev/rpmsg0
 *
 * ============================================================================
 * 参考资料
 * ============================================================================
 *
 * - Linux remoteproc 框架: Documentation/remoteproc.txt
 * - Resource Table 格式: include/linux/remoteproc.h
 * - VirtIO 规范: https://docs.oasis-open.org/virtio/
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/remoteproc.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_reserved_mem.h>
#include <linux/io.h>
#include <linux/slab.h>

#include "quard_star_rproc.h"

/*
 * remoteproc core exports rproc_vq_interrupt(), but the declaration is in
 * internal headers not installed for OOT modules.
 *
 * rproc_vq_interrupt() 用于通知 remoteproc 框架：某个 virtqueue 有新消息可用
 *
 * 参数：
 *   @rproc: remoteproc 实例指针
 *   @notifyid: 通知的 virtqueue 索引 (0 或 1)
 *
 * 返回：
 *   IRQ_HANDLED: 如果有 pending 消息被处理
 *   IRQ_NONE: 如果没有 pending 消息
 */
extern irqreturn_t rproc_vq_interrupt(struct rproc *rproc, int notifyid);

/* ============================================================================
 * Module 信息
 * ============================================================================ */

#define DRIVER_NAME		"quard_star-rproc"
#define DRIVER_VERSION	"1.0"
#define DRIVER_AUTHOR	"Quard Star Project"
#define DRIVER_DESC		"Quard Star Remoteproc Driver for FreeRTOS Domain"

/* ============================================================================
 * 设备私有数据
 * ============================================================================ */

/**
 * struct quard_star_rproc - Quard Star remoteproc 私有数据
 *
 * @dev: 平台设备
 * @rproc: remoteproc 实例
 * @mailbox_base: Mailbox 寄存器映射基址
 * @vq_irq: virtqueue kick IRQ 编号
 * @rsc_table: Resource Table 映射地址
 * @rsc_table_size: Resource Table 大小
 */
struct quard_star_rproc {
	struct device *dev;
	struct rproc *rproc;
	void __iomem *mailbox_base;
	int vq_irq;
	void *rsc_table;
	size_t rsc_table_size;
};

static int quard_star_carveout_map(struct rproc *rproc, struct rproc_mem_entry *mem)
{
	void *va;
	(void)rproc;

	/*
	 * 我们使用固定物理地址契约，避免remoteproc默认的dma_alloc_coherent
	 * 在attach模式下把vring/buffer分配到不可预测地址
	 */
	va = memremap(mem->dma, mem->len, MEMREMAP_WB);
	if (!va)
		return -ENOMEM;

	mem->va = va;
	return 0;
}

static int quard_star_carveout_unmap(struct rproc *rproc, struct rproc_mem_entry *mem)
{
	(void)rproc;
	if (mem->va)
		memunmap(mem->va);
	mem->va = NULL;
	return 0;
}

/* ============================================================================
 * Remoteproc ops 操作实现
 * ============================================================================ */

/**
 * quard_star_rproc_start() - 启动远程处理器
 *
 * 在 "Attach-Only" 模式下，FreeRTOS 已经运行
 * 此函数不应该被调用（使用 attach 代替）
 */
static int quard_star_rproc_start(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;
	dev_warn(priv->dev, "Attach-Only mode: start() should not be called, use attach\n");
	
	/* "Attach-Only" 模式：不应调用 start，返回成功即可 */
	return 0;
}

/**
 * quard_star_rproc_stop()-停止远程处理器
 * 在"Attach-Only"模式下，不实际停止FreeRTOS
 * 仅清理资源
 */
static int quard_star_rproc_stop(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;
	dev_dbg(priv->dev, "Attach-Only mode: not stopping FreeRTOS\n");
	/* "Attach-Only"模式, 不停止FreeRTOS */
	return 0;
}

/**
 * quard_star_rproc_kick()-通知远程处理器
 *
 * 通过mailbox发送IRQ 51到FreeRTOS
 */
static void quard_star_rproc_kick(struct rproc *rproc, int vqid)
{
	struct quard_star_rproc *priv = rproc->priv;

	if (!priv->mailbox_base) {
		dev_err(priv->dev, "Mailbox not mapped\n");
		return;
	}

	/* Linux -> FreeRTOS: 写 to-RTOS bank ch0 SET 触发 IRQ 51 (writel 自带隐式屏障) */
	writel(RVMB_RPMSG_DBELL, priv->mailbox_base + RVMB_TR_SET);

	dev_dbg(priv->dev, "Kicked FreeRTOS via Mailbox (vq=%d)\n", vqid);
}

/**
 * quard_star_rproc_attach() - 附加到远程处理器
 *
 * 注册 Mailbox IRQ 处理器 (IRQ 50)，并通知 FreeRTOS Linux 已就绪
 *
 * 【关键操作】设置 vdev.status = VIRTIO_CONFIG_S_DRIVER_OK (0x04)
 *
 * 这会让 FreeRTOS 知道 Linux 驱动已准备好，可以开始 RPMsg 通信。
 * 此步骤是 VirtIO 握手的关键部分，缺少它会导致 FreeRTOS 超时等待。
 */
static int quard_star_rproc_attach(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;
	struct resource_table *table = priv->rsc_table;
	struct fw_rsc_hdr *hdr;
	struct fw_rsc_vdev *vdev;

	dev_info(priv->dev, "Attaching to FreeRTOS remoteproc\n");

	/*
	 * 通过 Resource Table 结构体定位 vdev.status 字段，
	 * 复用 probe 阶段的 devm_memremap 映射，不再重复 ioremap。
	 *
	 * 布局: resource_table -> offset[0] -> fw_rsc_hdr -> fw_rsc_vdev
	 */
	hdr = (void *)table + table->offset[0];
	vdev = (struct fw_rsc_vdev *)hdr->data;

	dev_dbg(priv->dev, "Current vdev.status = 0x%x\n", vdev->status);

	/*
	 * 设置 DRIVER_OK 状态，通知 FreeRTOS:
	 * "Linux VirtIO 驱动已就绪，可以开始通信"
	 */
	vdev->status = VIRTIO_CONFIG_S_DRIVER_OK;

	/* 写屏障，确保 FreeRTOS 可见 */
	wmb();

	dev_info(priv->dev, "Set vdev.status = 0x%x (DRIVER_OK)\n",
		 VIRTIO_CONFIG_S_DRIVER_OK);

	return 0;
}

/**
 * quard_star_rproc_detach() - 从远程处理器分离
 *
 * 清理资源
 */
static int quard_star_rproc_detach(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;

	dev_dbg(priv->dev, "Detaching from FreeRTOS remoteproc\n");

	return 0;
}

/**
 * quard_star_rproc_prepare() - 为远程处理器准备内存
 *
 * 注册 vring 内存给 remoteproc 框架（固定物理地址）
 * RPMsg buffer 由 DTS 的 shared-dma-pool 自动分配
 */
static int quard_star_rproc_prepare(struct rproc *rproc)
{
	struct device *dev = rproc->dev.parent;
	struct rproc_mem_entry *mem;

	dev_dbg(dev, "Preparing remoteproc resources\n");

	/*
	 * 注册 vring0 的静态物理映射 (TX: FreeRTOS -> Linux)
	 * 必须显式注册，否则 remoteproc 会动态分配错误的地址
	 */
	mem = rproc_mem_entry_init(dev, NULL,
				   (dma_addr_t)QUARD_STAR_VRING0_PA,
				   QUARD_STAR_VRING_SIZE,
				   QUARD_STAR_VRING0_PA,
				   quard_star_carveout_map,
				   quard_star_carveout_unmap,
				   "vdev0vring0");
	if (!mem) {
		dev_err(dev, "Failed to create vring0 carveout\n");
		return -ENOMEM;
	}
	rproc_add_carveout(rproc, mem);
	dev_dbg(dev, "Registered vring0 carveout: PA=0x%pa, size=0x%x\n",
		&(dma_addr_t){QUARD_STAR_VRING0_PA}, QUARD_STAR_VRING_SIZE);

	/*
	 * 注册 vring1 的静态物理映射 (RX: Linux -> FreeRTOS)
	 */
	mem = rproc_mem_entry_init(dev, NULL,
				   (dma_addr_t)QUARD_STAR_VRING1_PA,
				   QUARD_STAR_VRING_SIZE,
				   QUARD_STAR_VRING1_PA,
				   quard_star_carveout_map,
				   quard_star_carveout_unmap,
				   "vdev0vring1");
	if (!mem) {
		dev_err(dev, "Failed to create vring1 carveout\n");
		return -ENOMEM;
	}
	rproc_add_carveout(rproc, mem);
	dev_dbg(dev, "Registered vring1 carveout: PA=0x%pa, size=0x%x\n",
		&(dma_addr_t){QUARD_STAR_VRING1_PA}, QUARD_STAR_VRING_SIZE);

	/*
	 * vdev0buffer (RPMsg 数据 Buffer):
	 * 由 DTS 的 shared-dma-pool 自动分配
	 * remoteproc_virtio.c 会通过 rproc_find_carveout_by_name("vdev0buffer")
	 * 查找并绑定给 virtio_rpmsg_bus 使用
	 */
	mem = rproc_of_resm_mem_entry_init(dev, 0,
					   QUARD_STAR_RPMSG_BUF_SIZE,
					   QUARD_STAR_RPMSG_BUF_PA,
					   "vdev0buffer");
	if (!mem) {
		dev_err(dev, "Failed to create vdev0buffer carveout\n");
		return -ENOMEM;
	}
	rproc_add_carveout(rproc, mem);
	dev_dbg(dev, "Registered vdev0buffer carveout: PA=0x%pa, size=0x%x\n",
		&(dma_addr_t){QUARD_STAR_RPMSG_BUF_PA}, QUARD_STAR_RPMSG_BUF_SIZE);

	return 0;
}

/**
 * quard_star_rproc_get_loaded_rsc_table() - 获取 Resource Table
 *
 * 返回 probe 阶段已映射的 Resource Table 指针（devm_memremap 管理生命周期）
 */
static struct resource_table *quard_star_rproc_get_loaded_rsc_table(struct rproc *rproc,
								   size_t *rsc_table_size)
{
	struct quard_star_rproc *priv = rproc->priv;
	struct resource_table *table = priv->rsc_table;

	if (table->ver != 1) {
		dev_err(priv->dev, "Invalid Resource Table version: %u\n",
			table->ver);
		return ERR_PTR(-EINVAL);
	}

	*rsc_table_size = priv->rsc_table_size;

	dev_dbg(priv->dev, "Resource Table: ver=%u, num=%u entries\n",
		table->ver, table->num);

	return table;
}

/* ============================================================================
 * Mailbox IRQ 处理器
 * ============================================================================ */

/**
 * quard_star_vq_irq_handler() - virtqueue kick IRQ 硬中断处理器
 *
 * 只做最快速的中断确认，然后唤醒 threaded handler
 * 当 FreeRTOS 通过 Mailbox 发送 IRQ 到 Linux 时调用
 */
static irqreturn_t quard_star_vq_irq_handler(int irq, void *dev_id)
{
	struct quard_star_rproc *priv = dev_id;

	/* 清除 to-Linux bank ch0 doorbell（W1C），中断线随之落下 */
	writel(RVMB_RPMSG_DBELL, priv->mailbox_base + RVMB_TL_CLEAR);

	/* 唤醒 threaded handler */
	return IRQ_WAKE_THREAD;
}

/**
 * quard_star_vq_irq_thread() - virtqueue kick IRQ 线程处理器
 *
 * 在进程上下文中处理 virtqueue 中断，可以睡眠
 */
static irqreturn_t quard_star_vq_irq_thread(int irq, void *dev_id)
{
	struct quard_star_rproc *priv = dev_id;
	struct rproc *rproc = priv->rproc;
	irqreturn_t handled = IRQ_NONE;

	/*
	 * 该 mailbox IRQ 复用为双队列 doorbell，无法直接区分 vq id，
	 * 因此依次探测 vq0/vq1。
	 */
	if (rproc) {
		if (rproc_vq_interrupt(rproc, 0) == IRQ_HANDLED)
			handled = IRQ_HANDLED;
		if (rproc_vq_interrupt(rproc, 1) == IRQ_HANDLED)
			handled = IRQ_HANDLED;
	}

	dev_dbg(priv->dev, "VQ IRQ %d handled\n", irq);

	return handled;
}

/* ============================================================================
 * Remoteproc 操作表
 * ============================================================================ */

static const struct rproc_ops quard_star_rproc_ops = {
	.prepare = quard_star_rproc_prepare,
	.start = quard_star_rproc_start,
	.stop = quard_star_rproc_stop,
	.kick = quard_star_rproc_kick,
	.attach = quard_star_rproc_attach,
	.detach = quard_star_rproc_detach,
	.get_loaded_rsc_table = quard_star_rproc_get_loaded_rsc_table,
};

/* ============================================================================
 * 平台设备操作
 * ============================================================================ */

/**
 * quard_star_rproc_probe() - 平台设备probe
 */
static int quard_star_rproc_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct quard_star_rproc *priv;
	int ret, irq;

	dev_dbg(dev, "Probing Quard Star remoteproc driver\n");

	/*
	 * 1) 分配驱动私有数据（driver context）
	 * devm_kzalloc: 与设备生命周期绑定，probe 失败或 remove 时自动释放。
	 */
	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->dev = dev;
	/* 将 priv 绑定到 pdev，后续 remove/ISR 可通过 platform_get_drvdata 获取 */
	platform_set_drvdata(pdev, priv);

	/*
	 * 2) 初始化 remoteproc 设备的 DMA 内存池
	 * of_reserved_mem_device_init(): 将 DTS 中 reserved-memory 绑定到该设备，
	 * 使 dma_alloc_coherent() 优先从固定保留内存分配（用于 rpmsg buffer 池）。
	 */
	ret = of_reserved_mem_device_init(dev);
	if (ret) {
		dev_warn(dev, "No DMA memory pool configured: %d\n", ret);
		dev_warn(dev, "virtio_rpmsg_bus may fail to allocate buffers\n");
	}

	/*
	 * 3) 映射 Mailbox MMIO
	 * devm_ioremap(): 仅做映射，不申请独占 mem region，
	 * 允许 mailbox_test 并行加载；若用 devm_ioremap_resource 会独占资源。
	 */
	priv->mailbox_base = devm_ioremap(dev, QUARD_STAR_MAILBOX_PA,
					  QUARD_STAR_MAILBOX_SIZE);
	if (!priv->mailbox_base) {
		dev_err(dev, "Failed to map Mailbox registers\n");
		ret = -ENOMEM;
		goto err_dma_mem;
	}

	/*
	 * 4) 映射 Resource Table 共享内存
	 * 使用 devm_memremap 确保设备移除时自动释放，
	 * 映射结果在 get_loaded_rsc_table / attach 中复用。
	 */
	priv->rsc_table = devm_memremap(dev, QUARD_STAR_RPROC_RSC_PA,
					QUARD_STAR_RPROC_RSC_SIZE, MEMREMAP_WC);
	if (IS_ERR_OR_NULL(priv->rsc_table)) {
		dev_err(dev, "Failed to map Resource Table at 0x%x\n",
			QUARD_STAR_RPROC_RSC_PA);
		ret = priv->rsc_table ? PTR_ERR(priv->rsc_table) : -ENOMEM;
		goto err_dma_mem;
	}
	priv->rsc_table_size = QUARD_STAR_RPROC_RSC_SIZE;

	/*
	 * 5) 获取 IRQ
	 * platform_get_irq() 返回的是 Linux 虚拟 IRQ 号，
	 * 硬件 PLIC 中断号来自 DTS 的 interrupts 字段。
	 */
	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		/* 如果设备树没有指定，使用硬编码的 IRQ 50 */
		irq = QUARD_STAR_RPROC_VQ_IRQ;
		dev_warn(dev, "DTS IRQ not found, using default VQ IRQ: %d\n", irq);
	}

	priv->vq_irq = irq;

	/*
	 * 6) 注册 Mailbox IRQ 处理器（threaded IRQ）
	 * 原因：rproc_vq_interrupt -> virtqueue 回调路径可能睡眠（例如 mutex_lock），
	 * 硬 IRQ 上下文禁止睡眠，因此需要 threaded IRQ。
	 *
	 * hardirq: quard_star_vq_irq_handler  (只 ACK 中断并返回 IRQ_WAKE_THREAD)
	 * thread:  quard_star_vq_irq_thread   (调用 rproc_vq_interrupt，可睡眠)
	 */
	ret = devm_request_threaded_irq(dev, irq,
						 quard_star_vq_irq_handler,
						 quard_star_vq_irq_thread,
					 IRQF_TRIGGER_RISING | IRQF_SHARED,
					 dev_name(dev), priv);
	if (ret) {
		dev_err(dev, "Failed to request Mailbox IRQ %d: %d\n", irq, ret);
		goto err_dma_mem;
	}

	dev_dbg(dev, "Mailbox IRQ %d registered\n", irq);

	/* 清理 to-Linux bank ch0 历史 pending，并 unmask bit0（使能该位中断）*/
	writel(RVMB_RPMSG_DBELL, priv->mailbox_base + RVMB_TL_CLEAR);
	writel(RVMB_RPMSG_DBELL, priv->mailbox_base + RVMB_TL_MASK_CLEAR);

	/* 分配并注册 remoteproc 实例 (使用 managed API) */
	priv->rproc = devm_rproc_alloc(dev, "quard-star-rproc",
				       &quard_star_rproc_ops, NULL, 0);
	if (!priv->rproc) {
		dev_err(dev, "Failed to allocate remoteproc instance\n");
		ret = -ENOMEM;
		goto err_dma_mem;
	}

	priv->rproc->priv = priv;
	priv->rproc->has_iommu = false; /* 无需 IOMMU */
	priv->rproc->auto_boot = false; /* "Attach-Only" 模式，不自动启动 */

	/* "Attach-Only" 模式：FreeRTOS 已运行，走 detached -> attach 路径 */
	priv->rproc->state = RPROC_DETACHED;

	/* 注册 remoteproc 设备 */
	ret = devm_rproc_add(dev, priv->rproc);
	if (ret) {
		dev_err(dev, "Failed to register remoteproc: %d\n", ret);
		goto err_dma_mem;
	}

	dev_info(dev, "Remoteproc driver initialized (IRQ=%d, state=DETACHED)\n", irq);

	return 0;

err_dma_mem:
	of_reserved_mem_device_release(dev);
	return ret;
}

/**
 * quard_star_rproc_remove() - 平台设备remove
 */
static int quard_star_rproc_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;

	of_reserved_mem_device_release(dev);

	return 0;
}

/* ============================================================================
 * 设备树匹配表
 * ============================================================================ */

static const struct of_device_id quard_star_rproc_match[] = {
	{ .compatible = "quard-star,rproc", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, quard_star_rproc_match);

static struct platform_driver quard_star_rproc_driver = {
	.probe = quard_star_rproc_probe,
	.remove = quard_star_rproc_remove,
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = quard_star_rproc_match,
	},
};
module_platform_driver(quard_star_rproc_driver);

/* ============================================================================
 * Module 元数据
 * ============================================================================ */

MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
```

---

## 文件：mailbox_test.c（Linux 邮箱中断测试驱动）

路径：`project/drivers/mailbox_test/src/mailbox_test.c`

```c
/*
 * Quard Star Mailbox Test Driver
 *
 * 这是一个极简的中断测试驱动，用于验证 Mailbox IRQ 50 是否正确工作。
 *
 * 编译：
 *   make ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- -C ~/linux M=$(pwd) modules
 *
 * 使用：
 *   insmod mailbox_test.ko
 *   # 触发中断：devmem 0x10004000 32 1
 *   # 查看日志：dmesg | tail
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/io.h>

/* RV-Mailbox: to-Linux bank channel0（本驱动是接收方） */
#define RVMB_TL_STAT         0x00   /* doorbell 状态 (RO) */
#define RVMB_TL_SET          0x04   /* 置位 (W1S)，测试时手写它模拟对端 kick */
#define RVMB_TL_CLEAR        0x08   /* 清位 (W1C) */
#define RVMB_TL_MASK_CLEAR   0x14   /* 清屏蔽=使能该位中断 (W1C) */
#define RVMB_DBELL_BIT       0x1    /* channel0 bit0 */

static void __iomem *mailbox_base;
static int mailbox_irq;
static int irq_count = 0;

/*
 * 中断服务程序 (ISR)
 *
 * 当 Mailbox 拉高 IRQ 线时，PLIC 会调用这个函数。
 */
static irqreturn_t mailbox_isr(int irq, void *dev_id)
{
    uint32_t stat;

    /* 读取 doorbell 状态寄存器 */
    stat = readl(mailbox_base + RVMB_TL_STAT);

    /* 清除该 doorbell 位 (W1C 语义) */
    writel(RVMB_DBELL_BIT, mailbox_base + RVMB_TL_CLEAR);

    irq_count++;

    pr_info("[MAILBOX TEST] IRQ received! count=%d, stat=0x%x\n", irq_count, stat);

    return IRQ_HANDLED;
}

/*
 * 平台设备 Probe 函数
 */
static int mailbox_probe(struct platform_device *pdev)
{
    struct resource *res;
    int ret;
    uint32_t reg_val;

    dev_info(&pdev->dev, "Quard Star Mailbox Test Driver probing...\n");

    /* 1. 获取并映射 MMIO 资源 */
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        dev_err(&pdev->dev, "Failed to get MMIO resource\n");
        return -ENODEV;
    }

    mailbox_base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(mailbox_base)) {
        dev_err(&pdev->dev, "Failed to map MMIO\n");
        return PTR_ERR(mailbox_base);
    }

    dev_info(&pdev->dev, "MMIO mapped: 0x%pa - 0x%pa\n", &res->start, &res->end);

    /* 2. 读取 REVISION 寄存器验证设备 (RV-Mailbox 在 0xF0，应为 0x0100) */
    reg_val = readl(mailbox_base + 0xF0);
    dev_info(&pdev->dev, "Mailbox REVISION: 0x%x\n", reg_val);

    if (reg_val != 0x0100) {
        dev_warn(&pdev->dev, "Unexpected revision! Expected 0x0100 (v1)\n");
    }

    /* 3. 获取 IRQ 编号 */
    mailbox_irq = platform_get_irq(pdev, 0);
    if (mailbox_irq < 0) {
        dev_err(&pdev->dev, "Failed to get IRQ resource\n");
        return mailbox_irq;
    }

    dev_info(&pdev->dev, "IRQ number: %d\n", mailbox_irq);

    /* 4. 注册中断处理函数 */
    ret = devm_request_irq(&pdev->dev, mailbox_irq, mailbox_isr,
                           IRQF_SHARED | IRQF_TRIGGER_RISING,
                           "mailbox_test", pdev);
    if (ret) {
        dev_err(&pdev->dev, "Failed to request IRQ %d (error: %d)\n", mailbox_irq, ret);
        return ret;
    }

    dev_info(&pdev->dev, "IRQ %d registered successfully\n", mailbox_irq);

    /* 5. unmask to-Linux ch0 bit0（清屏蔽位 = 使能该位中断）*/
    writel(RVMB_DBELL_BIT, mailbox_base + RVMB_TL_MASK_CLEAR);
    dev_info(&pdev->dev, "Mailbox ch0 bit0 unmasked (中断已使能)\n");

    dev_info(&pdev->dev, "Probe complete. Ready to receive interrupts!\n");
    dev_info(&pdev->dev, "Test with: devmem 0x%llx 32 1\n",
             (unsigned long long)res->start + RVMB_TL_SET);

    return 0;
}
 
/*
 * 平台设备 Remove 函数
 */
static int mailbox_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Removing driver... Total IRQs received: %d\n", irq_count);

    /* 禁用中断：重新屏蔽 to-Linux ch0 bit0（置屏蔽位 W1S，0x10）*/
    if (mailbox_base) {
        writel(RVMB_DBELL_BIT, mailbox_base + 0x10);
    }

    return 0;
}

/*
 * 设备树匹配表
 */
static const struct of_device_id mailbox_of_match[] = {
    { .compatible = "quard-star,mailbox", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mailbox_of_match);

/*
 * 平台驱动结构体
 */
static struct platform_driver mailbox_driver = {
    .probe = mailbox_probe,
    .remove = mailbox_remove,
    .driver = {
        .name = "mailbox_test",
        .of_match_table = mailbox_of_match,
    },
};

module_platform_driver(mailbox_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quard Star Project");
MODULE_DESCRIPTION("Quard Star Mailbox Interrupt Test Driver");
MODULE_VERSION("1.0");
```

---

## 文件：hwspecs.h（可信域硬件规格头：寄存器偏移）

路径：`project/trusted_domain/bsp_include/hwspecs.h`

```c
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
 * Mailbox 寄存器（RV-Mailbox：参考 ARM MHU v1/v2 自主实现）
 *   双 bank × 3 通道 × 32-bit doorbell。to-Linux @0x000 (FreeRTOS 写它通知 Linux, IRQ50)，
 *   to-RTOS @0x100 (IRQ51)。每通道 stride 0x20：STAT/SET(W1S)/CLEAR(W1C)/MASK_*。
 *   FreeRTOS 是发送方：写 to-Linux ch0 SET 即 kick；接收靠轮询 vring。
 */
#define RVMB_BANK_TO_LINUX      0x000
#define RVMB_BANK_TO_RTOS       0x100
#define RVMB_R_STAT             0x00
#define RVMB_R_SET              0x04
#define RVMB_R_CLEAR            0x08
#define RVMB_RPMSG_DBELL        0x1     /* channel0 bit0 */
#define RVMB_TL_SET             (RVMB_BANK_TO_LINUX + RVMB_R_SET)   /* 0x04：kick Linux */
#define REG_REVISION            0xF0    /* IP 版本号 (RO)，v1=0x0100 */

/* 兼容旧名（openamp_adapter 等遗留代码引用；映射到 channel0 等价偏移）*/
#define REG_LINUX_TRIG          RVMB_TL_SET                          /* 0x04 */
#define REG_RTOS_TRIG           (RVMB_BANK_TO_RTOS + RVMB_R_SET)     /* 0x104 */
#define REG_LINUX_ACK           (RVMB_BANK_TO_LINUX + RVMB_R_CLEAR)  /* 0x08 */
#define REG_RTOS_ACK            (RVMB_BANK_TO_RTOS + RVMB_R_CLEAR)   /* 0x108 */

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
```

---

## 文件：simple_rpmsg.c（FreeRTOS 侧轻量 RPMsg 收发实现）

路径：`project/trusted_domain/simple_rpmsg.c`

```c
/*
 * Simple RPMsg Implementation for FreeRTOS (Fixed - Correct Host/Device Roles)
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * Virtio 角色分配（Gemini 修正）：
 * - Host (Linux): 生产者，写入 avail，读取 used
 * - Device (FreeRTOS): 消费者，读取 avail，写入 used
 *
 * 关键修复：
 * 1. 发送时：从 avail 读取 buffer，写入 used
 * 2. 接收时：从 avail 读取消息，写入 used
 * 3. 维护本地消费索引 (tx_avail_idx, rx_avail_idx)
 * 4. 使用 Resource Table 中的实际 vring 地址
 * 5. 等待 Linux 提供空 buffer（Race Condition 修复）
 */

#include "simple_rpmsg.h"
#include "hwspecs.h"
#include "uart8250.h"
#include "bsp_include/resource_table.h"

#include "FreeRTOS.h"
#include "task.h"

#include <string.h>
#include <stdio.h>



/* ============================================================================
 * VirtQueue 结构定义
 * ============================================================================ */

#define VIRTIO_DESC_F_NEXT     1
#define VIRTIO_DESC_F_WRITE    2

struct vring_desc {
    uint64_t addr;   /* Address (guest-physical) */
    uint32_t len;    /* Length */
    uint16_t flags;
    uint16_t next;
};

struct vring_avail {
    uint16_t flags;
    uint16_t idx;
    uint16_t ring[];
};

struct vring_used {
    uint16_t flags;
    uint16_t idx;
    struct {
        uint32_t id;
        uint32_t len;
    } ring[];
};

struct vring {
    struct vring_desc *desc;
    struct vring_avail *avail;
    struct vring_used *used;
};

/* RPMsg 地址定义 */
#define RPMSG_ADDR_ANY        0xFFFFFFFF
#define RPMSG_NS_ADDR         53

/* NS 消息标志 */
#define RPMSG_NS_DESTROY      1
#define RPMSG_NS_CREATE       0

/* RPMsg 消息头 */
struct rpmsg_hdr {
    uint32_t src;
    uint32_t dst;
    uint32_t reserved;
    uint16_t len;
    uint16_t flags;
    uint8_t  data[];
} __attribute__((packed));

/* RPMsg NS (Name Service) 消息结构 */
struct rpmsg_ns_msg {
    char name[32];
    uint32_t addr;
    uint32_t flags;
} __attribute__((packed));

/* ============================================================================
 * 全局变量
 * ============================================================================ */

static struct vring g_vring0;  /* TX vring (FreeRTOS -> Linux) */
static struct vring g_vring1;  /* RX vring (Linux -> FreeRTOS) */
static struct rpmsg_endpoint g_ept;
static uint32_t g_local_addr = 0x30;  /* 本地端点地址 */
static uint32_t g_remote_addr = RPMSG_NS_ADDR; /* 业务消息目的地址，收到首包后更新 */
static int g_rpmsg_initialized = 0;

/* ⚠️ 关键：本地消费索引（由 Device 维护）*/
static uint16_t tx_avail_idx = 0;  /* TX 环的本地消费索引 */
static uint16_t rx_avail_idx = 0;  /* RX 环的本地消费索引 */

/* Mailbox 寄存器 */
/* 注意: offset 是字节偏移。base 转成字节指针再加，否则 uint32_t* 指针运算会 ×4。
 * (v1 时 offset 恒为 0x00 故未暴露; v2 偏移 0x04 起必须按字节算) */
#define MAILBOX_REG(base, offset)  (*(volatile uint32_t *)((volatile uint8_t *)(base) + (offset)))

/* ============================================================================
 * VirtQueue 初始化
 * ============================================================================ */

/**
 * 初始化 VirtQueue 指针
 */
static void vring_init(struct vring *vr, uintptr_t pa)
{
    vr->desc  = (struct vring_desc *)pa;
    vr->avail = (struct vring_avail *)(pa + VRING_SIZE * sizeof(struct vring_desc));

    /* used 部分需要对齐到页边界 */
    uint32_t used_offset = (VRING_SIZE * sizeof(struct vring_desc) +
                           sizeof(uint16_t) + sizeof(uint16_t) +
                           VRING_SIZE * sizeof(uint16_t));
    used_offset = (used_offset + 0xfff) & ~0xfff;  /* 页对齐 */
    vr->used = (struct vring_used *)(pa + used_offset);
}

/* ============================================================================
 * Mailbox 操作（硬件门铃）
 * ============================================================================ */

/**
 * 触发 Mailbox 中断到 Linux
 */
static void mailbox_kick_to_linux(void)
{
    volatile uint32_t *mailbox = (volatile uint32_t *)MAILBOX_BASE_ADDR;

    /* RV-Mailbox：写 to-Linux bank ch0 SET 置 bit0 → 触发 IRQ50 通知 Linux */
    MAILBOX_REG(mailbox, RVMB_TL_SET) = RVMB_RPMSG_DBELL;

    /* RISC-V 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");
}

/* ============================================================================
 * RPMsg 实现
 * ============================================================================ */

/**
 * 初始化 Simple RPMsg
 * ✅ 修复：使用 Resource Table 中的实际 vring 地址
 */
int simple_rpmsg_init(void)
{
    if (g_rpmsg_initialized) {
        return 0;
    }

    /* ⚠️ 关键修复：从 Resource Table 读取 vring 地址 */
    volatile struct shared_resource_table *rsc =
        (volatile struct shared_resource_table *)RESOURCE_TABLE_ADDR;

    uintptr_t vring0_pa = (uintptr_t)rsc->vring0.da;
    uintptr_t vring1_pa = (uintptr_t)rsc->vring1.da;

    /* 使用 Resource Table 中的实际地址初始化 vring */
    vring_init(&g_vring0, vring0_pa);
    vring_init(&g_vring1, vring1_pa);

    /* 初始化本地消费索引 */
    tx_avail_idx = 0;
    rx_avail_idx = 0;
    g_remote_addr = RPMSG_NS_ADDR;

    /* ✅ 修复：正确打印 vring PA */
    char buf[64];
    snprintf(buf, sizeof(buf), "[RPMsg] vring0 PA = 0x%lx\r\n", vring0_pa);
    uart8250_puts(buf);

    snprintf(buf, sizeof(buf), "[RPMsg] vring1 PA = 0x%lx\r\n", vring1_pa);
    uart8250_puts(buf);

    uart8250_puts("[RPMsg] Local consumer indices reset\r\n");

    /* 清空端点 */
    memset(&g_ept, 0, sizeof(g_ept));

    g_rpmsg_initialized = 1;

    uart8250_puts("[RPMsg] Initialized successfully\r\n");

    return 0;
}

/**
 * 发送 NS (Name Service) 宣告包
 * ✅ 修正：从 avail 读取 buffer，写入 used
 * ✅ 修正：等待 Linux 提供空 buffer（Race Condition 修复）
 */
static int send_ns_announce(struct rpmsg_endpoint *ept)
{
    struct vring *vr = &g_vring0;  /* TX 环 */
    struct vring_desc *desc;
    struct rpmsg_hdr *hdr;
    struct rpmsg_ns_msg *ns_msg;
    uint16_t desc_idx;
    uint32_t msg_len;
    uint8_t *buffer;
    uint16_t used_idx;

    if (!g_rpmsg_initialized) {
        return -1;
    }

    msg_len = sizeof(struct rpmsg_hdr) + sizeof(struct rpmsg_ns_msg);

    /* ✅ 终极修复：死等 Linux 提供空 buffer（Race Condition 修复）*/
    uint32_t wait_count = 0;
    const uint32_t MAX_WAIT = 100;  /* 最多等待 5 秒 */

    while (tx_avail_idx == vr->avail->idx) {
        if (wait_count == 0) {
            uart8250_puts("[RPMsg] Waiting for Linux to provide empty buffers...\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(50));  /* 等 50ms */
        wait_count++;

        if (wait_count >= MAX_WAIT) {
            uart8250_puts("[RPMsg] Timeout waiting for buffers!\r\n");
            return -1;
        }

        /* 每 1 秒打印一次 */
        if (wait_count % 20 == 0) {
            uart8250_puts("[RPMsg] Still waiting... (");
            uart8250_putc('0' + (wait_count / 20));
            uart8250_puts("s)\r\n");
        }
    }

    uart8250_puts("[RPMsg] Empty buffers received from Linux!\r\n");

    /* ✅ 1. 从 avail 环消费一个描述符（Linux 提供的）*/
    desc_idx = vr->avail->ring[tx_avail_idx % VRING_SIZE];
    desc = &vr->desc[desc_idx];

    uart8250_puts("[RPMsg] NS: Got descriptor ");
    /* 简单打印 */
    if (desc_idx < 10) {
        uart8250_putc('0' + desc_idx);
    }
    uart8250_puts(" from avail\r\n");

    /* 使用 descriptor 中提供的 buffer 地址 */
    buffer = (uint8_t *)(uintptr_t)desc->addr;

    if (msg_len > desc->len) {
        uart8250_puts("[RPMsg] NS payload too large for descriptor\r\n");
        return -1;
    }

    /* ✅ 修复：正确打印 buffer 地址 */
    char buf[64];
    snprintf(buf, sizeof(buf), "[RPMsg] NS: buffer addr = 0x%lx\r\n", (uintptr_t)desc->addr);
    uart8250_puts(buf);

    /* 2. 构建 RPMsg + NS 宣告包 */
    hdr = (struct rpmsg_hdr *)buffer;
    hdr->src = g_local_addr;
    hdr->dst = RPMSG_NS_ADDR;
    hdr->reserved = 0;
    hdr->len = sizeof(struct rpmsg_ns_msg);
    hdr->flags = 0;

    ns_msg = (struct rpmsg_ns_msg *)hdr->data;
    memset(ns_msg, 0, sizeof(*ns_msg));
    strncpy(ns_msg->name, ept->name, sizeof(ns_msg->name) - 1);
    ns_msg->addr = g_local_addr;
    ns_msg->flags = RPMSG_NS_CREATE;

    /* 3. ✅ 将描述符生产到 used 环交差 */
    used_idx = vr->used->idx % VRING_SIZE;
    vr->used->ring[used_idx].id = desc_idx;
    vr->used->ring[used_idx].len = msg_len;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    /* 更新 used->idx */
    vr->used->idx++;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    /* 4. 更新本地消费索引 */
    tx_avail_idx++;

    /* 5. 发送硬件门铃（Kick Linux）*/
    mailbox_kick_to_linux();

    uart8250_puts("[RPMsg] NS announce sent: ");
    uart8250_puts(ept->name);
    uart8250_puts(" -> addr 0x30\r\n");

    return 0;
}

/**
 * 创建 RPMsg 端点
 */
int simple_rpmsg_create_ept(struct rpmsg_endpoint *ept,
                            const char *name,
                            void (*cb)(struct rpmsg_endpoint *, void *, size_t, uint32_t),
                            void *priv)
{
    if (!g_rpmsg_initialized) {
        uart8250_puts("[RPMsg] Not initialized!\r\n");
        return -1;
    }

    memset(ept, 0, sizeof(*ept));
    ept->addr = g_local_addr;
    if (name) {
        strncpy(ept->name, name, sizeof(ept->name) - 1);
    }
    ept->cb = cb;
    ept->priv = priv;

    /* 复制到全局端点 */
    memcpy(&g_ept, ept, sizeof(*ept));

    uart8250_puts("[RPMsg] Endpoint created: ");
    uart8250_puts(name ? name : "null");
    uart8250_puts("\r\n");

    return 0;
}

/**
 * 发送 NS 宣告包（在 Linux attach 之后调用）
 */
int simple_rpmsg_announce_endpoint(void)
{
    return send_ns_announce(&g_ept);
}

/**
 * 发送 RPMsg 消息
 * ✅ 修正：从 avail 读取 buffer，写入 used
 * ✅ 修正：等待 Linux 提供空 buffer
 */
int simple_rpmsg_send(struct rpmsg_endpoint *ept, const void *data, size_t len)
{
    struct vring *vr = &g_vring0;  /* TX 环 */
    struct vring_desc *desc;
    struct rpmsg_hdr *hdr;
    uint16_t desc_idx;
    uint32_t msg_len;
    uint8_t *buffer;
    uint16_t used_idx;

    (void)ept;  /* 未使用 */

    if (!g_rpmsg_initialized) {
        return -1;
    }

    msg_len = sizeof(struct rpmsg_hdr) + len;

    /* ✅ 等待 Linux 提供空 buffer（简短等待）*/
    uint32_t wait_count = 0;
    const uint32_t MAX_WAIT = 20;  /* 最多等待 1 秒 */

    while (tx_avail_idx == vr->avail->idx) {
        vTaskDelay(pdMS_TO_TICKS(50));
        wait_count++;

        if (wait_count >= MAX_WAIT) {
            /* 没有可用 buffer，放弃 */
            return -1;
        }
    }

    /* ✅ 1. 从 avail 环消费一个描述符 */
    desc_idx = vr->avail->ring[tx_avail_idx % VRING_SIZE];
    desc = &vr->desc[desc_idx];

    /* 使用 descriptor 中提供的 buffer 地址 */
    buffer = (uint8_t *)(uintptr_t)desc->addr;

    if (msg_len > desc->len) {
        return -1;
    }

    /* 2. 构建 RPMsg 消息头 */
    hdr = (struct rpmsg_hdr *)buffer;
    hdr->src = g_local_addr;
    hdr->dst = g_remote_addr;
    hdr->reserved = 0;
    hdr->len = len;
    hdr->flags = 0;

    /* 拷贝数据 */
    if (data && len > 0) {
        memcpy(hdr->data, data, len);
    }

    /* 3. ✅ 将描述符生产到 used 环 */
    used_idx = vr->used->idx % VRING_SIZE;
    vr->used->ring[used_idx].id = desc_idx;
    vr->used->ring[used_idx].len = msg_len;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    vr->used->idx++;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    /* 4. 更新本地消费索引 */
    tx_avail_idx++;

    /* 5. 发送硬件门铃 */
    mailbox_kick_to_linux();

    uart8250_puts("[RPMsg] Sent: ");
    uart8250_puts((const char *)data);
    uart8250_puts("\r\n");

    return 0;
}

/**
 * 轮询处理 RPMsg 消息
 * ✅ 修正：从 avail 读取消息，写入 used
 */
void simple_rpmsg_poll(void)
{
    struct vring *vr = &g_vring1;  /* RX 环 */
    struct vring_desc *desc;
    struct rpmsg_hdr *hdr;
    uint16_t desc_idx;
    uint8_t *buffer;
    uint16_t used_idx;

    if (!g_rpmsg_initialized) {
        return;
    }

    /* ✅ 检查 Linux 是否有新消息 */
    if (rx_avail_idx == vr->avail->idx) {
        return;  /* 没有新消息 */
    }

    /* ✅ 1. 从 avail 环消费一条新消息 */
    desc_idx = vr->avail->ring[rx_avail_idx % VRING_SIZE];
    desc = &vr->desc[desc_idx];
    buffer = (uint8_t *)(uintptr_t)desc->addr;
    hdr = (struct rpmsg_hdr *)buffer;

    if (desc->len >= sizeof(*hdr) && hdr->len <= (desc->len - sizeof(*hdr))) {
        /* 记录对端业务地址，后续 simple_rpmsg_send 使用 */
        g_remote_addr = hdr->src;

        uart8250_puts("[RPMsg] Received: ");
        uart8250_puts((const char *)hdr->data);
        uart8250_puts("\r\n");

        /* 2. 触发回调处理业务 */
        if (g_ept.cb) {
            g_ept.cb(&g_ept, hdr->data, hdr->len, hdr->src);
        }
    } else {
        uart8250_puts("[RPMsg] Drop malformed packet\r\n");
    }

    /* 3. ✅ 将 Buffer 生产到 used 环，退还给 Linux */
    used_idx = vr->used->idx % VRING_SIZE;
    vr->used->ring[used_idx].id = desc_idx;
    vr->used->ring[used_idx].len = 0;  /* 退还空 Buffer，长度填 0 */

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    vr->used->idx++;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    /* 4. 更新本地消费索引 */
    rx_avail_idx++;

    /* 5. 发送硬件门铃通知 Linux 回收 */
    mailbox_kick_to_linux();
}
```

---

*—— 源程序清单结束 ——*
