/*
 * libmetal cache.h - FreeRTOS 最小化实现
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件提供 libmetal cache 操作的最小化实现
 * 避免依赖完整的 libmetal 库
 */

#ifndef METAL_CACHE_H
#define METAL_CACHE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * metal_cache_flush - 刷新缓存
 * @addr: 起始地址
 * @size: 大小
 */
static inline void metal_cache_flush(void *addr, size_t size)
{
    /* 使用 fence 指令刷新缓存 */
    __asm__ volatile("fence ow, ow" ::: "memory");
}

/**
 * metal_cache_invalidate - 使缓存无效
 * @addr: 起始地址
 * @size: 大小
 */
static inline void metal_cache_invalidate(void *addr, size_t size)
{
    /* 使用 fence 指令使缓存无效 */
    __asm__ volatile("fence iorw,iow" ::: "memory");
}

/**
 * metal_cache_clean - 清除缓存
 * @addr: 起始地址
 * @size: 大小
 */
static inline void metal_cache_clean(void *addr, size_t size)
{
    /* 使用 fence 指令清除缓存 */
    __asm__ volatile("fence ow,ow" ::: "memory");
}

#ifdef __cplusplus
}
#endif

#endif /* METAL_CACHE_H */
