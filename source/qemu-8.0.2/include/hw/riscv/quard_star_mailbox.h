/*
 * QEMU RISC-V Quard Star Mailbox Controller
 *
 * Copyright (c) 2025 Quard Star Project
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

#ifndef HW_RISCV_QUARD_STAR_MAILBOX_H
#define HW_RISCV_QUARD_STAR_MAILBOX_H

#include "hw/sysbus.h"
#include "qom/object.h"
#include "qemu/typedefs.h"

#define TYPE_QUARD_STAR_MAILBOX_DEVICE "quard-star-mailbox"
OBJECT_DECLARE_SIMPLE_TYPE(QuardStarMailboxState, QUARD_STAR_MAILBOX_DEVICE)

/**
 * struct QuardStarMailboxState - Mailbox device state
 * @parent: Parent SysBusDevice
 * @iomem: MMIO region for register access
 * @irq_linux: IRQ line to Linux (Hart 1-7 via PLIC input 50)
 * @irq_rtos: IRQ line to FreeRTOS (Hart 0 via PLIC input 51)
 * @linux_trig_state: Current trigger state for Linux interrupt (0=inactive, 1=active)
 * @linux_ie: Linux interrupt enable flag
 * @rtos_trig_state: Current trigger state for FreeRTOS interrupt (0=inactive, 1=active)
 * @rtos_ie: FreeRTOS interrupt enable flag
 *
 * This device implements a minimal doorbell-style mailbox for AMP
 * inter-processor communication between FreeRTOS and Linux.
 *
 * Register Map:
 * 0x00 - LINUX_TRIG  (WO): Trigger interrupt to Linux (W1S)
 * 0x04 - LINUX_ACK   (WO): Clear Linux interrupt (W1C)
 * 0x08 - LINUX_STAT  (RO): Read Linux interrupt line status
 * 0x0C - LINUX_IE    (RW): Linux interrupt enable
 * 0x10-0x1F - Reserved
 *
 * 0x20 - RTOS_TRIG   (WO): Trigger interrupt to FreeRTOS (W1S)
 * 0x24 - RTOS_ACK    (WO): Clear FreeRTOS interrupt (W1C)
 * 0x28 - RTOS_STAT   (RO): Read FreeRTOS interrupt line status
 * 0x2C - RTOS_IE     (RW): FreeRTOS interrupt enable
 * 0x30-0x3F - Reserved
 *
 * 0x40 - REVISION    (RO): IP revision (0x0100 = v1.0)
 * 0x44-0xFF - Reserved
 *
 * Interrupt Logic:
 * - IRQ output = TRIGGER_STATE && IE_STATE
 * - Level-triggered semantics: IRQ stays high until ACKed
 */
struct QuardStarMailboxState {
    /*< private >*/
    SysBusDevice parent_obj;

    /*< public >*/
    MemoryRegion iomem;

    /* Output IRQ lines */
    qemu_irq irq_linux;   /* To Linux (PLIC input 50) */
    qemu_irq irq_rtos;    /* To FreeRTOS (PLIC input 51) */

    /* Linux control state */
    uint8_t linux_trig_state;  /* Trigger state (0 or 1) */
    uint8_t linux_ie;          /* Interrupt enable (0 or 1) */

    /* FreeRTOS control state */
    uint8_t rtos_trig_state;   /* Trigger state (0 or 1) */
    uint8_t rtos_ie;           /* Interrupt enable (0 or 1) */
};

#endif /* HW_RISCV_QUARD_STAR_MAILBOX_H */
