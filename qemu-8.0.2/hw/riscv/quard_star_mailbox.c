/*
 * QEMU RISC-V Quard Star Mailbox Controller
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * A minimal doorbell-style mailbox IP for AMP (Asymmetric Multi-Processing)
 * inter-processor communication between FreeRTOS (Hart 0) and Linux (Hart 1-7).
 *
 * This device implements level-triggered interrupts with Write-1-to-Set (W1S)
 * trigger registers and Write-1-to-Clear (W1C) acknowledge registers.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "qemu/module.h"
#include "migration/vmstate.h"
#include "hw/sysbus.h"
#include "hw/irq.h"
#include "hw/riscv/quard_star_mailbox.h"

/* Register offsets */
#define REG_LINUX_TRIG      0x00    /* WO: Trigger interrupt to Linux */
#define REG_LINUX_ACK       0x04    /* W1C: Clear Linux interrupt */
#define REG_LINUX_STAT      0x08    /* RO: Read Linux interrupt line status */
#define REG_LINUX_IE        0x0C    /* RW: Linux interrupt enable */

#define REG_RTOS_TRIG       0x20    /* WO: Trigger interrupt to FreeRTOS */
#define REG_RTOS_ACK        0x24    /* W1C: Clear FreeRTOS interrupt */
#define REG_RTOS_STAT       0x28    /* RO: Read FreeRTOS interrupt line status */
#define REG_RTOS_IE         0x2C    /* RW: FreeRTOS interrupt enable */

#define REG_REVISION        0x40    /* RO: IP revision (0x0100 = v1.0) */

/* IP Revision: Major.Minor (BCD) */
#define MAILBOX_REVISION    0x0100

/* Helper macros to update interrupt lines based on state machine */
#define UPDATE_LINUX_IRQ(s) \
    qemu_set_irq((s)->irq_linux, \
                 ((s)->linux_trig_state && (s)->linux_ie) ? 1 : 0)

#define UPDATE_RTOS_IRQ(s) \
    qemu_set_irq((s)->irq_rtos, \
                 ((s)->rtos_trig_state && (s)->rtos_ie) ? 1 : 0)

static uint64_t quard_star_mailbox_read(void *opaque, hwaddr offset,
                                        unsigned size)
{
    QuardStarMailboxState *s = QUARD_STAR_MAILBOX_DEVICE(opaque);
    uint64_t value = 0;

    switch (offset) {
    case REG_LINUX_TRIG:
    case REG_LINUX_ACK:
        /* Write-only registers return 0 on read */
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Read from WO register 0x%02"HWADDR_PRIx"\n",
                      __func__, offset);
        value = 0;
        break;

    case REG_LINUX_STAT:
        /* Return current physical interrupt line state to Linux */
        value = s->linux_trig_state && s->linux_ie;
        break;

    case REG_LINUX_IE:
        /* Return interrupt enable state */
        value = s->linux_ie;
        break;

    case REG_RTOS_TRIG:
    case REG_RTOS_ACK:
        /* Write-only registers return 0 on read */
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Read from WO register 0x%02"HWADDR_PRIx"\n",
                      __func__, offset);
        value = 0;
        break;

    case REG_RTOS_STAT:
        /* Return current physical interrupt line state to FreeRTOS */
        value = s->rtos_trig_state && s->rtos_ie;
        break;

    case REG_RTOS_IE:
        /* Return interrupt enable state */
        value = s->rtos_ie;
        break;

    case REG_REVISION:
        /* Return IP revision (read-only) */
        value = MAILBOX_REVISION;
        break;

    default:
        /* Reserved registers return 0 */
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Invalid register read: 0x%02"HWADDR_PRIx"\n",
                      __func__, offset);
        value = 0;
        break;
    }

    return value;
}

static void quard_star_mailbox_write(void *opaque, hwaddr offset,
                                     uint64_t value, unsigned size)
{
    QuardStarMailboxState *s = QUARD_STAR_MAILBOX_DEVICE(opaque);

    switch (offset) {
    case REG_LINUX_TRIG:
        /* W1S (Write-1-to-Set): Only setting bit 0 has effect */
        if (value & 0x1) {
            if (!s->linux_trig_state) {
                s->linux_trig_state = 1;
                UPDATE_LINUX_IRQ(s);
            }
        }
        break;

    case REG_LINUX_ACK:
        /* W1C (Write-1-to-Clear): Only clearing bit 0 has effect */
        if (value & 0x1) {
            if (s->linux_trig_state) {
                s->linux_trig_state = 0;
                UPDATE_LINUX_IRQ(s);
            }
        }
        break;

    case REG_LINUX_STAT:
        /* Read-only register */
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Write to RO register 0x%02"HWADDR_PRIx"\n",
                      __func__, offset);
        break;

    case REG_LINUX_IE:
        /* Interrupt enable: bit 0 controls enable, others ignored */
        s->linux_ie = (value & 0x1) ? 1 : 0;
        UPDATE_LINUX_IRQ(s);
        break;

    case REG_RTOS_TRIG:
        /* W1S (Write-1-to-Set): Only setting bit 0 has effect */
        if (value & 0x1) {
            if (!s->rtos_trig_state) {
                s->rtos_trig_state = 1;
                UPDATE_RTOS_IRQ(s);
            }
        }
        break;

    case REG_RTOS_ACK:
        /* W1C (Write-1-to-Clear): Only clearing bit 0 has effect */
        if (value & 0x1) {
            if (s->rtos_trig_state) {
                s->rtos_trig_state = 0;
                UPDATE_RTOS_IRQ(s);
            }
        }
        break;

    case REG_RTOS_STAT:
        /* Read-only register */
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Write to RO register 0x%02"HWADDR_PRIx"\n",
                      __func__, offset);
        break;

    case REG_RTOS_IE:
        /* Interrupt enable: bit 0 controls enable, others ignored */
        s->rtos_ie = (value & 0x1) ? 1 : 0;
        UPDATE_RTOS_IRQ(s);
        break;

    case REG_REVISION:
        /* Read-only register */
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Write to RO register REVISION\n", __func__);
        break;

    default:
        /* Reserved registers: ignore writes */
        qemu_log_mask(LOG_GUEST_ERROR,
                      "%s: Invalid register write: 0x%02"HWADDR_PRIx"\n",
                      __func__, offset);
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

    /* Initialize MMIO region (4KB as per spec for page alignment) */
    memory_region_init_io(&s->iomem, obj, &quard_star_mailbox_ops,
                          s, "quard-star-mailbox", 0x1000);
    sysbus_init_mmio(sbd, &s->iomem);

    /* Initialize two output IRQ lines for bidirectional communication */
    sysbus_init_irq(sbd, &s->irq_linux);   /* IRQ to Linux (PLIC input 50) */
    sysbus_init_irq(sbd, &s->irq_rtos);    /* IRQ to FreeRTOS (PLIC input 51) */
}

static void quard_star_mailbox_reset(DeviceState *dev)
{
    QuardStarMailboxState *s = QUARD_STAR_MAILBOX_DEVICE(dev);

    /* Reset all internal states to hardware defaults */
    s->linux_trig_state = 0;
    s->linux_ie = 0;
    s->rtos_trig_state = 0;
    s->rtos_ie = 0;

    /* Ensure both interrupt lines are low after reset */
    qemu_set_irq(s->irq_linux, 0);
    qemu_set_irq(s->irq_rtos, 0);
}

static const VMStateDescription vmstate_quard_star_mailbox = {
    .name = TYPE_QUARD_STAR_MAILBOX_DEVICE,
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_UINT8(linux_trig_state, QuardStarMailboxState),
        VMSTATE_UINT8(linux_ie, QuardStarMailboxState),
        VMSTATE_UINT8(rtos_trig_state, QuardStarMailboxState),
        VMSTATE_UINT8(rtos_ie, QuardStarMailboxState),
        VMSTATE_END_OF_LIST()
    }
};

static void quard_star_mailbox_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);

    dc->reset = quard_star_mailbox_reset;
    dc->desc = "Quard Star Mailbox Controller";
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
