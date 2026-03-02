/*
 * Quard Star Resource Table Definitions
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * Resource Table 是 OpenAMP 框架的核心数据结构
 * 用于远程处理器（FreeRTOS）和主处理器（Linux）之间协商 virtio/rpmsg 通道参数
 *
 * 本文件提供独立的 Resource Table 定义，不依赖 open-amp 库
 */

#ifndef QUARD_STAR_RESOURCE_TABLE_H
#define QUARD_STAR_RESOURCE_TABLE_H

#include "hwspecs.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 资源类型定义（对齐 Linux remoteproc.h） */
#define RSC_VDEV                3       // 虚拟设备资源类型
#define VIRTIO_ID_RPMSG         7       // RPMSG virtio 设备 ID
#define VIRTIO_RPMSG_F_NS       0       // RPMSG 名称服务特性位
#define FW_RSC_ADDR_ANY         0xFFFFFFFFU  // 任意地址标记（由主处理器分配）

/* Virtio 设备状态位（对齐 virtio_config.h） */
#define VIRTIO_CONFIG_S_DRIVER_OK   0x04  // 驱动程序已就绪状态位

#pragma pack(push, 1)

/**
 * @brief 资源表头部结构
 *
 * 资源表中的每个资源都有一个头部，用于标识资源类型
 */
struct fw_rsc_hdr {
    uint32_t type;      // 资源类型（如 RSC_VDEV）
};

/**
 * @brief Virtio vring 配置结构
 *
 * 描述 virtqueue 使用的 virtio 环形缓冲区配置参数
 *
 * 【DA vs PA 说明】（行业黑话翻译）：
 * - PA (Physical Address) = 物理地址，内存的真实地址
 * - DA (Device Address) = 设备地址（虚拟地址），远程处理器看到的地址
 *
 * 在本系统（attach 模式）中：
 * - Linux 不会分配内存，所以 da/pa 必须硬编码成相同的物理地址
 * - 两个地址都指向共享内存中预分配的 vring 缓冲区位置
 */
struct fw_rsc_vdev_vring {
    uint32_t da;        // 设备地址，远程处理器视角的地址（attach 模式下 = pa）
    uint32_t align;     // 对齐要求，通常是 4096 字节（页对齐）
    uint32_t num;       // 描述符数量，vring 容量（通常是 256）
    uint32_t notifyid;  // 通知 ID，用于 mailbox 中断通知（Linux 启动后回填）
    uint32_t pa;        // 物理地址，内存的真实地址
};

/**
 * @brief Virtio 设备资源结构
 *
 * 描述一个虚拟设备（如 RPMSG）的配置信息
 */
struct fw_rsc_vdev {
    uint32_t id;            // virtio 设备 ID（如 VIRTIO_ID_RPMSG）
    uint32_t notifyid;      // 设备通知 ID
    uint32_t dfeatures;     // 设备支持的特性位
    uint32_t gfeatures;     // 协商后的特性位
    uint32_t config_len;    // 配置数据长度
    uint8_t status;         // 设备状态（按 virtio 规范）
    uint8_t num_of_vrings;  // 跟随的 vring 数量（通常为 2）
    uint8_t reserved[2];    // 保留字段（必须是 2 字节以对齐）
};

/**
 * @brief 共享资源表结构
 *
 * 这是 Resource Table 的主结构，用于 Linux 和 FreeRTOS 之间的 IPC 通信。
 * 必须放置在双方都能访问的共享内存区域，且地址需要传递给 Linux remoteproc。
 *
 * 布局：
 * - 固定头部（版本号、资源数量）
 * - 偏移量数组
 * - 资源数据（vdev + 2 个 vring）
 */
struct shared_resource_table {
    uint32_t ver;              // 资源表版本号（当前为 1）
    uint32_t num;              // 资源数量（当前为 1，即 vdev）
    uint32_t reserved[2];      // 保留字段
    uint32_t offset[1];        // 资源偏移量数组
    struct fw_rsc_hdr hdr;     // 资源头部
    struct fw_rsc_vdev vdev;   // virtio 设备配置（RPMSG）
    struct fw_rsc_vdev_vring vring0;  // TX vring 配置
    struct fw_rsc_vdev_vring vring1;  // RX vring 配置
};

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* QUARD_STAR_RESOURCE_TABLE_H */
