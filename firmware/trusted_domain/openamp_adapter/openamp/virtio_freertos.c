/*
 * open-amp FreeRTOS VirtIO 传输层实现
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件实现 open-amp 在 FreeRTOS 上的 VirtIO 传输层
 * 主要处理通过 Mailbox 的 virtqueue 通知
 * 参考：open-amp/lib/virtio/virtio.c (generic 实现)
 */

#include "openamp/virtio.h"
#include "openamp/virtqueue.h"
#include "openamp/rpmsg_virtio.h"
#include "metal/io.h"
#include "metal/device.h"
#include "metal/irq.h"
#include "metal/log.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hwspecs.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================================
 * VirtIO FreeRTOS 设备结构
 * ============================================================================ */

struct virtio_freertos_device {
    struct virtio_device vdev;           /* 基类 virtio 设备 */
    struct metal_io_region *io;          /* 共享内存 I/O 区域 */
    volatile uint32_t *mailbox_base;     /* Mailbox 寄存器基址 */
    uint32_t mailbox_irq_to_linux;       /* FreeRTOS → Linux IRQ */
    uint32_t mailbox_irq_to_rtos;        /* Linux → FreeRTOS IRQ */
};

/* ============================================================================
 * VirtIO 操作函数
 * ============================================================================ */

/**
 * 创建 virtqueue 实例
 */
static int virtio_freertos_create_virtqueues(struct virtio_device *vdev,
                                            unsigned int flags,
                                            unsigned int nvqs,
                                            const char *names[],
                                            vq_callback callbacks[],
                                            void *callback_args[])
{
    (void)vdev;
    (void)flags;
    (void)nvqs;
    (void)names;
    (void)callbacks;
    (void)callback_args;

    /* 简化实现：virtqueue 将在 RPMsg 初始化时创建 */
    return 0;
}

/**
 * 删除 virtqueue 实例
 */
static void virtio_freertos_delete_virtqueues(struct virtio_device *vdev)
{
    (void)vdev;
    /* 简化实现 */
}

/**
 * 获取设备状态
 * libmetal v1.9.0 不再支持 status
 */
static uint8_t virtio_freertos_get_status(struct virtio_device *dev)
{
    (void)dev;
    return 0;  /* 简化实现 */
}

/**
 * 设置设备状态
 * libmetal v1.9.0 不再支持 status
 */
static void virtio_freertos_set_status(struct virtio_device *dev,
                                       uint8_t status)
{
    (void)dev;
    (void)status;
    /* 简化实现 */
}

/**
 * 读取设备特征
 */
static uint32_t virtio_freertos_get_features(struct virtio_device *dev)
{
    (void)dev;
    /* 支持 RPMsg 特征 */
    return (1UL << VIRTIO_RPMSG_F_NS);
}

/**
 * 设置设备特征
 */
static void virtio_freertos_set_features(struct virtio_device *dev,
                                        uint32_t feature)
{
    dev->features = feature;
}

/**
 * 协商设备特征
 */
static uint32_t virtio_freertos_negotiate_features(struct virtio_device *dev,
                                                   uint32_t features)
{
    uint32_t supported = virtio_freertos_get_features(dev);
    dev->features = features & supported;
    return dev->features;
}

/**
 * 读取配置空间
 */
static void virtio_freertos_read_config(struct virtio_device *dev,
                                       uint32_t offset,
                                       void *dst,
                                       int length)
{
    (void)dev;
    (void)offset;
    (void)dst;
    (void)length;
    /* 简化实现 */
}

/**
 * 写入配置空间
 */
static void virtio_freertos_write_config(struct virtio_device *dev,
                                        uint32_t offset,
                                        void *src,
                                        int length)
{
    (void)dev;
    (void)offset;
    (void)src;
    (void)length;
    /* 简化实现 */
}

/**
 * 重置设备
 */
static void virtio_freertos_reset_device(struct virtio_device *dev)
{
    dev->features = 0;
}

/**
 * 通知远程处理器（Mailbox 中断）
 *
 * 这是 VirtIO 的 "kick" 操作：
 * - 写 Mailbox 寄存器触发中断到 Linux (IRQ 50)
 * - Linux 收到中断后，会检查并处理 virtqueue
 */
static void virtio_freertos_notify(struct virtqueue *vq)
{
    struct virtio_device *vdev = vq->vq_dev;
    struct virtio_freertos_device *fdev;

    fdev = (struct virtio_freertos_device *)vdev->priv;

    /* 写 Mailbox 寄存器触发中断到 Linux */
    if (fdev && fdev->mailbox_base) {
        *(volatile uint32_t *)(fdev->mailbox_base + REG_RTOS_TRIG) = 1;

        /* RISC-V 内存屏障，确保写操作完成 */
        asm volatile("fence ow,ow" ::: "memory");
    }
}

/* ============================================================================
 * VirtIO 操作表
 * ============================================================================ */

static const struct virtio_dispatch virtio_freertos_dispatch = {
    .create_virtqueues = virtio_freertos_create_virtqueues,
    .delete_virtqueues = virtio_freertos_delete_virtqueues,
    .get_status = virtio_freertos_get_status,
    .set_status = virtio_freertos_set_status,
    .get_features = virtio_freertos_get_features,
    .set_features = virtio_freertos_set_features,
    .negotiate_features = virtio_freertos_negotiate_features,
    .read_config = virtio_freertos_read_config,
    .write_config = virtio_freertos_write_config,
    .reset_device = virtio_freertos_reset_device,
    .notify = virtio_freertos_notify,
};

/* ============================================================================
 * 全局设备实例（简化实现）
 * ============================================================================ */

static struct virtio_freertos_device g_virtio_freertos_dev;

/* ============================================================================
 * API 实现
 * ============================================================================ */

/**
 * 创建 VirtIO FreeRTOS 设备
 *
 * @param vdev_ret     virtio 设备结构指针
 * @param io          共享内存 I/O 区域
 * @param role        VIRTIO_DEV_DEVICE (远程) 或 VIRTIO_DEV_DRIVER (主机)
 */
int virtio_freertos_create(struct virtio_device **vdev_ret,
                           struct metal_io_region *io,
                           int role)
{
    struct virtio_freertos_device *fdev;

    if (!vdev_ret || !io) {
        return -EINVAL;
    }

    fdev = &g_virtio_freertos_dev;
    memset(fdev, 0, sizeof(*fdev));

    /* 初始化 virtio 设备 */
    fdev->vdev.role = role;
    fdev->vdev.priv = fdev;
    fdev->vdev.func = &virtio_freertos_dispatch;

    /* 保存 I/O 区域 */
    fdev->io = io;

    /* 设置 Mailbox 基地址 */
    fdev->mailbox_base = (volatile uint32_t *)MAILBOX_BASE_ADDR;
    fdev->mailbox_irq_to_linux = MAILBOX_IRQ_TO_LINUX;
    fdev->mailbox_irq_to_rtos = MAILBOX_IRQ_TO_RTOS;

    *vdev_ret = &fdev->vdev;

    printf("VirtIO FreeRTOS device created (role=%d)\n", role);

    return 0;
}

/**
 * 销毁 VirtIO FreeRTOS 设备
 */
void virtio_freertos_destroy(struct virtio_device *vdev)
{
    (void)vdev;
    /* 简化实现：全局设备无需销毁 */
}

/**
 * 注册 Mailbox IRQ 处理器
 *
 * 使用场景：
 * - FreeRTOS 初始化 RPMsg 后，调用此函数注册 IRQ 51 处理器
 * - 当 Linux 通过 Mailbox 发送数据时，会触发此回调
 */
int virtio_freertos_setup_irq(struct virtio_device *vdev)
{
    struct virtio_freertos_device *fdev;
    int ret;

    if (!vdev) {
        return -EINVAL;
    }

    fdev = (struct virtio_freertos_device *)vdev->priv;

    /* 注册 Mailbox IRQ 处理器 */
    ret = metal_irq_register(fdev->mailbox_irq_to_rtos,
                            NULL,  /* 回调将在 virtqueue 处理中设置 */
                            fdev);

    if (ret) {
        printf("Failed to register Mailbox IRQ %d\n", fdev->mailbox_irq_to_rtos);
        return ret;
    }

    printf("Mailbox IRQ %d registered for VirtIO\n", fdev->mailbox_irq_to_rtos);

    return 0;
}
