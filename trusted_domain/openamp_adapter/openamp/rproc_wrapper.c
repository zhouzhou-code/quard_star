/*
 * open-amp FreeRTOS remoteproc 封装实现
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件实现 open-amp 在 FreeRTOS 上的 remoteproc 生命周期封装
 * 简化版本，用于快速集成测试
 */

#include "openamp/remoteproc.h"
#include "openamp/remoteproc_virtio.h"
#include "openamp/rpmsg_virtio.h"
#include "metal/io.h"
#include "metal/irq.h"

#include "openamp_adapter.h"

#include "FreeRTOS.h"
#include "task.h"

#include "hwspecs.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================================
 * 全局状态（简化实现）
 * ============================================================================ */

static struct metal_io_region g_shm_io;
static struct metal_io_region g_rsc_io;
static struct virtio_device *g_virtio_dev = NULL;
static struct rpmsg_virtio_device g_rpmsg_vdev;
static int g_initialized = 0;

/* ============================================================================
 * API 实现
 * ============================================================================ */

/**
 * 创建 FreeRTOS remoteproc 实例（简化版）
 */
int rproc_freertos_create(struct remoteproc **rproc_ret)
{
    int ret;

    if (!rproc_ret) {
        return -EINVAL;
    }

    if (g_initialized) {
        *rproc_ret = NULL;  /* 简化：不需要 remoteproc 实例 */
        return 0;
    }

    /* 初始化 libmetal 系统 */
    ret = metal_freertos_sys_init();
    if (ret) {
        printf("Failed to initialize libmetal: %d\n", ret);
        return ret;
    }

    /* 注册共享内存 I/O 区域 */
    ret = metal_register_shared_memory(&g_shm_io,
                                       SHM_BASE_ADDR,
                                       SHM_SIZE,
                                       "shared_memory");
    if (ret) {
        printf("Failed to register shared memory: %d\n", ret);
        return ret;
    }

    /* 注册 Resource Table I/O 区域 */
    ret = metal_register_shared_memory(&g_rsc_io,
                                       RESOURCE_TABLE_ADDR,
                                       RESOURCE_TABLE_SIZE,
                                       "resource_table");
    if (ret) {
        printf("Failed to register resource table I/O: %d\n", ret);
        return ret;
    }

    /* 创建 VirtIO 设备 */
    ret = virtio_freertos_create(&g_virtio_dev,
                                 &g_shm_io,
                                 VIRTIO_DEV_DEVICE);
    if (ret) {
        printf("Failed to create VirtIO device: %d\n", ret);
        return ret;
    }

    g_initialized = 1;

    *rproc_ret = NULL;  /* 简化：不需要 remoteproc 实例 */

    printf("FreeRTOS OpenAMP initialized successfully\n");

    return 0;
}

/**
 * 销毁 FreeRTOS remoteproc 实例（简化版）
 */
void rproc_freertos_destroy(struct remoteproc *rproc)
{
    (void)rproc;

    if (!g_initialized) {
        return;
    }

    if (g_virtio_dev) {
        virtio_freertos_destroy(g_virtio_dev);
        g_virtio_dev = NULL;
    }

    metal_freertos_sys_finish();

    g_initialized = 0;
}

/**
 * 初始化 RPMsg 通信
 */
int rproc_freertos_init_rpmsg(struct remoteproc *rproc,
                              struct rpmsg_device **rpdev_ret)
{
    int ret;

    (void)rproc;

    if (!g_initialized || !g_virtio_dev) {
        printf("OpenAMP not initialized\n");
        return -EIO;
    }

    /* 注册 Mailbox IRQ 处理器 */
    ret = virtio_freertos_setup_irq(g_virtio_dev);
    if (ret) {
        printf("Failed to setup Mailbox IRQ: %d\n", ret);
        return ret;
    }

    /* 初始化 RPMsg VirtIO 设备 */
    memset(&g_rpmsg_vdev, 0, sizeof(g_rpmsg_vdev));

    ret = rpmsg_init_vdev(&g_rpmsg_vdev,
                          g_virtio_dev,
                          NULL,
                          &g_shm_io,
                          NULL);

    if (ret) {
        printf("Failed to initialize RPMsg vdev: %d\n", ret);
        return ret;
    }

    *rpdev_ret = &g_rpmsg_vdev.rdev;

    printf("RPMsg initialized successfully\n");

    return 0;
}

/**
 * 等待 RPMsg 通道绑定完成
 */
int rproc_freertos_wait_rpmsg_bind(struct rpmsg_device *rpdev,
                                   uint32_t timeout_ms)
{
    (void)rpdev;
    (void)timeout_ms;

    /* RPMsg 绑定在 NS 回调中异步完成 */
    return 0;
}
