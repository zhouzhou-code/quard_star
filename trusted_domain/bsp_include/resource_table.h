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

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * OpenAMP Resource Table 标准定义
 * ============================================================================ */

/**
 * struct resource_table - Resource Table 主结构
 *
 * Resource Table 格式版本 1
 */
struct resource_table {
    uint32_t ver;           /* 版本号 (1) */
    uint32_t num;           /* 资源数量 */
    uint32_t reserved[2];   /* 保留字段 */
    uint32_t offset[1];     /* 资源偏移数组（变长）*/
};

/* 资源类型定义 */
#define RSC_CARVEOUT   0     /* 物理内存区域 */
#define RSC_DEVMEM     1     /* 设备内存 */
#define RSC_TRACE      2     /* 跟踪缓冲区 */
#define RSC_VDEV       3     /* virtio 设备 */

/**
 * struct fw_rsc_carveout - 物理内存区域
 */
struct fw_rsc_carveout {
    uint32_t type;          /* RSC_CARVEOUT */
    uint32_t da;            /* 设备地址 */
    uint32_t pa;            /* 物理地址 */
    uint32_t len;           /* 长度 */
    uint32_t flags;         /* 标志位 */
    uint64_t reserved;      /* 保留 */
    char name[32];          /* 区域名称 */
};

/**
 * struct fw_rsc_vdev - virtio 设备
 */
struct fw_rsc_vdev {
    uint32_t type;          /* RSC_VDEV */
    uint32_t id;            /* virtio 设备 ID */
    uint32_t notifyid;      /* 通知 ID */
    uint32_t dfeatures;     /* 设备特性 */
    uint32_t gfeatures;     /* 主机特性 */
    uint32_t config_len;    /* 配置长度 */
    uint8_t status;         /* 状态 */
    uint8_t num_of_vrings;  /* vring 数量 */
    uint16_t reserved[15];  /* 保留 */
    uint32_t config[1];     /* 配置数据（变长）*/
};

/**
 * struct fw_rsc_vdev_vring - virtio vring
 */
struct fw_rsc_vdev_vring {
    uint32_t da;            /* 设备地址 */
    uint32_t align;         /* 对齐要求 */
    uint32_t num;           /* 描述符数量 */
    uint32_t notifyid;      /* 通知 ID */
};

/* virtio 设备 ID */
#define VIRTIO_ID_RPMSG  7   /* RPMsg 设备 */

/* ============================================================================
 * Quard Star 特定的 Resource Table 配置
 * ============================================================================ */

/**
 * Resource Table 实例
 *
 * 位于共享内存 0xbf70c000
 * 由 FreeRTOS 初始化，Linux 通过 remoteproc 驱动读取
 */
extern struct resource_table resource_table;

/**
 * 初始化 Resource Table
 *
 * 此函数填充 resource_table 结构，设置：
 * - vdev (virtio 设备)
 * - vring0 (RX: FreeRTOS → Linux)
 * - vring1 (TX: Linux → FreeRTOS)
 */
void resource_table_init(void);

#ifdef __cplusplus
}
#endif

#endif /* QUARD_STAR_RESOURCE_TABLE_H */
