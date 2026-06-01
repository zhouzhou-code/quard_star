/*
 * libmetal device.h - 最小化存根
 *
 * Copyright (c) 2025 Quard Star Project
 */

#ifndef __METAL_DEVICE_H__
#define __METAL_DEVICE_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct metal_device;

/* 简单的设备结构 */
struct metal_device {
    const char *name;
    void *priv;
};

#ifdef __cplusplus
}
#endif

#endif /* __METAL_DEVICE_H__ */
