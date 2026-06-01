/*
 * libmetal sys.h - FreeRTOS 最小化实现
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件提供 libmetal 系统接口的最小化实现
 * 避免依赖完整的 libmetal 库
 */

#ifndef METAL_SYS_H
#define METAL_SYS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * metal_sys_init - 初始化 libmetal 系统
 */
int metal_sys_init(void);

/**
 * metal_sys_finish - 反初始化 libmetal 系统
 */
void metal_sys_finish(void);

#ifdef __cplusplus
}
#endif

#endif /* METAL_SYS_H */
