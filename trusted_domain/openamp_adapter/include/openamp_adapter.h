/*
 * OpenAMP FreeRTOS 适配层 - 导出头文件
 *
 * Copyright (c) 2025 Quard Star Project
 */

#ifndef OPENAMP_ADAPTER_H
#define OPENAMP_ADAPTER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 基础类型定义（避免依赖 libmetal/open-amp 头文件）
 * ============================================================================ */

/* 物理地址类型 */
typedef uint64_t metal_phys_addr_t;

/* 前向声明（不完整类型）*/
struct metal_io_region;
struct virtio_device;
struct remoteproc;
struct rpmsg_device;

/* ============================================================================
 * libmetal FreeRTOS 扩展 API
 * ============================================================================ */

/**
 * 初始化 libmetal 系统（FreeRTOS 版本）
 */
int metal_freertos_sys_init(void);

/**
 * 反初始化 libmetal 系统（FreeRTOS 版本）
 */
void metal_freertos_sys_finish(void);

/**
 * 注册共享内存 I/O 区域
 */
int metal_register_shared_memory(struct metal_io_region *io,
                                 metal_phys_addr_t physmap,
                                 size_t size,
                                 const char *name);

/**
 * 触发 Mailbox 中断到 Linux
 */
void metal_mailbox_notify(void);

/**
 * 清除来自 Linux 的 Mailbox 中断
 */
void metal_mailbox_clear(void);

/**
 * 获取 Mailbox IRQ 编号
 */
int metal_mailbox_get_irq(void);

/* ============================================================================
 * open-amp FreeRTOS 扩展 API
 * ============================================================================ */

/**
 * 创建 VirtIO FreeRTOS 设备
 */
int virtio_freertos_create(struct virtio_device **vdev_ret,
                           struct metal_io_region *io,
                           int role);

/**
 * 销毁 VirtIO FreeRTOS 设备
 */
void virtio_freertos_destroy(struct virtio_device *vdev);

/**
 * 注册 Mailbox IRQ 处理器
 */
int virtio_freertos_setup_irq(struct virtio_device *vdev);

/**
 * 创建 FreeRTOS remoteproc 实例
 */
int rproc_freertos_create(struct remoteproc **rproc_ret);

/**
 * 销毁 FreeRTOS remoteproc 实例
 */
void rproc_freertos_destroy(struct remoteproc *rproc);

/**
 * 初始化 RPMsg 通信
 */
int rproc_freertos_init_rpmsg(struct remoteproc *rproc,
                              struct rpmsg_device **rpdev_ret);

/**
 * 等待 RPMsg 通道绑定完成
 */
int rproc_freertos_wait_rpmsg_bind(struct rpmsg_device *rpdev,
                                   uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* OPENAMP_ADAPTER_H */
