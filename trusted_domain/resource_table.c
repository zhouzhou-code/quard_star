/*
 * Resource Table 初始化实现
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件实现 Resource Table 的初始化
 */

#include "bsp_include/resource_table.h"
#include <string.h>

/* ============================================================================
 * Resource Table 实例（紧凑型，使用静态初始化）
 * ============================================================================ */

static struct shared_resource_table rsc_table_payload = {
    .ver = 1,
    .num = 1,
    .reserved = {0, 0},
    .offset = { offsetof(struct shared_resource_table, vdev) },  // 极其关键！

    .vdev = {
        .type = RSC_VDEV,
        .id = VIRTIO_ID_RPMSG,
        .notifyid = MAILBOX_IRQ_TO_LINUX,
        .dfeatures = (1UL << VIRTIO_RPMSG_F_NS),
        .gfeatures = 0,
        .config_len = 0,
        .status = 0,
        .num_of_vrings = 2,
        .reserved = {0, 0}
    },
    .vring0 = { .da = VRING0_PA, .align = VRING_ALIGN, .num = VRING_SIZE, .notifyid = MAILBOX_IRQ_TO_LINUX },
    .vring1 = { .da = VRING1_PA, .align = VRING_ALIGN, .num = VRING_SIZE, .notifyid = MAILBOX_IRQ_TO_LINUX }
};

/* ============================================================================
 * 初始化函数
 * ============================================================================ */

void resource_table_init(void)
{
    memcpy((void *)RESOURCE_TABLE_ADDR, &rsc_table_payload, sizeof(rsc_table_payload));
    __asm__ volatile("fence ow, ow" ::: "memory");
}
