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
 * VirtIO RPMsg 特性位定义
 * ============================================================================ */

#define VIRTIO_RPMSG_F_NS  0   /* Name Service 特性 */

/* ============================================================================
 * Resource Table 全局实例
 * ============================================================================ */

/**
 * Resource Table 实例
 *
 * 地址：0xbf900018（在 .data 段中）
 */
struct resource_table resource_table __attribute__((aligned(4))) = {
    .ver = 1,
    .num = 1,
    .reserved = {0, 0},
};

/**
 * vdev 资源条目 - VirtIO 设备信息
 */
static struct fw_rsc_vdev vdev_rsc __attribute__((aligned(4))) = {
    .type = RSC_VDEV,
    .id = VIRTIO_ID_RPMSG,
    .dfeatures = (1UL << VIRTIO_RPMSG_F_NS),
    .gfeatures = 0,
    .config_len = 0,
    .status = 0,
    .num_of_vrings = 2,
    .notifyid = MAILBOX_IRQ_TO_LINUX,
    .reserved = {0},
};

/**
 * vring 资源条目 - VirtIO 环队列信息
 */
static struct fw_rsc_vdev_vring vring0_rsc __attribute__((aligned(4))) = {
    .da = VRING0_PA,
    .align = VRING_ALIGN,
    .num = VRING_SIZE,
    .notifyid = MAILBOX_IRQ_TO_LINUX,
};

static struct fw_rsc_vdev_vring vring1_rsc __attribute__((aligned(4))) = {
    .da = VRING1_PA,
    .align = VRING_ALIGN,
    .num = VRING_SIZE,
    .notifyid = MAILBOX_IRQ_TO_LINUX,
};

/* ============================================================================
 * 初始化函数
 * ============================================================================ */

/**
 * 初始化 Resource Table
 *
 * 此函数将 vdev、vring0、vring1 复制到共享内存 0xbf70c000
 */
void resource_table_init(void)
{
    volatile uint32_t *dst;
    uint32_t *src;
    int i, words;

    /* 计算 vdev + 2 个 vring 的总大小 */
    words = (sizeof(vdev_rsc) + sizeof(vring0_rsc) + sizeof(vring1_rsc)) / 4;

    /* 目标地址：resource_table 在共享内存中的位置 */
    dst = (volatile uint32_t *)RESOURCE_TABLE_ADDR;
    dst += sizeof(struct resource_table) / 4;  /* 跳过 resource_table 头部 */

    /* 复制 vdev */
    src = (uint32_t *)&vdev_rsc;
    for (i = 0; i < (int)(sizeof(vdev_rsc) / 4); i++) {
        dst[i] = src[i];
    }
    dst += sizeof(vdev_rsc) / 4;

    /* 复制 vring0 */
    src = (uint32_t *)&vring0_rsc;
    for (i = 0; i < (int)(sizeof(vring0_rsc) / 4); i++) {
        dst[i] = src[i];
    }
    dst += sizeof(vring0_rsc) / 4;

    /* 复制 vring1 */
    src = (uint32_t *)&vring1_rsc;
    for (i = 0; i < (int)(sizeof(vring1_rsc) / 4); i++) {
        dst[i] = src[i];
    }

    /* 内存屏障，确保写入完成 */
    __asm__ volatile("fence ow, ow" ::: "memory");
}
