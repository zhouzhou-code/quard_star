/*
 * libmetal atomic.h - 最小化存根
 *
 * Copyright (c) 2025 Quard Star Project
 */

#ifndef __METAL_ATOMIC_H__
#define __METAL_ATOMIC_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 简单的原子操作（RISC-V 使用 fence 指令）*/
static inline uint32_t metal_atomic_uint32(volatile uint32_t *ptr, uint32_t val)
{
    uint32_t old;
    __asm__ volatile("amoswap.w %0, %2, %1"
                     : "=r"(old), "+A"(*ptr)
                     : "r"(val)
                     : "memory");
    return old;
}

#ifdef __cplusplus
}
#endif

#endif /* __METAL_ATOMIC_H__ */
