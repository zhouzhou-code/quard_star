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

/* 资源类型定义 */
#define RSC_VDEV       3
#define VIRTIO_ID_RPMSG 7
#define VIRTIO_RPMSG_F_NS 0

/* Virtio 设备状态定义（用于同步）*/
#define VIRTIO_CONFIG_S_DRIVER_OK   4  /* 驱动已就绪 */

#pragma pack(push, 1)

struct fw_rsc_vdev_vring {
    uint32_t da;
    uint32_t align;
    uint32_t num;
    uint32_t notifyid;
};

struct fw_rsc_vdev {
    uint32_t type;
    uint32_t id;
    uint32_t notifyid;
    uint32_t dfeatures;
    uint32_t gfeatures;
    uint32_t config_len;
    uint8_t status;
    uint8_t num_of_vrings;
    uint8_t reserved[2];  // 绝对只能是 2 个字节！
};

struct shared_resource_table {
    uint32_t ver;
    uint32_t num;
    uint32_t reserved[2];
    uint32_t offset[1];  // 必须要有目录偏移
    struct fw_rsc_vdev vdev;
    struct fw_rsc_vdev_vring vring0;
    struct fw_rsc_vdev_vring vring1;
};

#pragma pack(pop)

void resource_table_init(void);

#ifdef __cplusplus
}
#endif

#endif /* QUARD_STAR_RESOURCE_TABLE_H */