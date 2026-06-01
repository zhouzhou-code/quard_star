/*
 * OpenAMP FreeRTOS 配置
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件定义 OpenAMP 在 FreeRTOS 上的编译时配置
 */

#ifndef OPENAMP_CONFIG_H
#define OPENAMP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hwspecs.h"

/* ============================================================================
 * OpenAMP 框架配置
 * ============================================================================ */

/**
 * RPMsg 缓冲区数量
 * 每个 buffer 大小为 RPMSG_BUFFER_SIZE (512B)
 */
#define RPMSG_NUM_BUFFERS        (RPMSG_BUF_SIZE / RPMSG_BUFFER_SIZE)  /* 64 */

/**
 * VirtIO 设备特性
 */
#define VIRTIO_RPMSG_F_NS        0  /* 名称服务（可选） */

/**
 * Resource Table 版本
 */
#define RSC_TBL_VERSION          1

/**
 * 最大 RPMsg 端点数量
 */
#define RPMSG_ENDPOINT_MAX       16

#ifdef __cplusplus
}
#endif

#endif /* OPENAMP_CONFIG_H */
