/*
 * QEMU RISC-V Mailbox Controller —— RV-Mailbox (通用 SysBus 设备)
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 机器无关的多通道 doorbell 邮箱，填补 QEMU RISC-V 平台无邮箱设备的空白，
 * 可挂载到任意 RISC-V 机器(quard_star / virt / ...)。
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

#ifndef HW_RISCV_MAILBOX_H
#define HW_RISCV_MAILBOX_H

#include "hw/sysbus.h"
#include "qom/object.h"
#include "qemu/typedefs.h"

#define TYPE_RISCV_MAILBOX "riscv.mailbox"
OBJECT_DECLARE_SIMPLE_TYPE(RISCVMailboxState, RISCV_MAILBOX)

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
struct RISCVMailboxState {
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

#endif /* HW_RISCV_MAILBOX_H */
