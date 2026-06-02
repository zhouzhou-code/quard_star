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
