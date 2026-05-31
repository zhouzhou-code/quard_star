/*
 * libmetal FreeRTOS 缓存操作实现
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件实现 libmetal 在 FreeRTOS 上的缓存操作
 * RISC-V 使用 Zicbom 扩展或手工管理缓存
 * 参考：libmetal/lib/system/generic/cache.c
 */

#include "metal/cache.h"
#include "metal/sys.h"

#include "hwspecs.h"

#include <stdint.h>

/* ============================================================================
 * RISC-V 缓存管理指令
 * ============================================================================ */

/**
 * RISC-V 缓存管理指令编码
 *
 * 使用 .insn 伪指令生成 Zicbom 指令：
 * - cbo.clean: 清除缓存行 (写回)
 * - cbo.inval: 使缓存行无效
 * - cbo.flush: 刷新缓存行 (写回 + 无效)
 *
 * 对于不支持 Zicbom 的实现，使用 fence 指令作为替代
 */

#if (__riscv_zicbom)
#define CBO_CLEAN(offset)    asm volatile(".insn i 0x0F, 0, x0, %0" :: "r"(offset) : "memory")
#define CBO_INVAL(offset)    asm volatile(".insn i 0x0F, 1, x0, %0" :: "r"(offset) : "memory")
#else
/* 不支持 Zicbom，使用 fence 作为替代 */
#define CBO_CLEAN(offset)    asm volatile("fence ow,ow" ::: "memory")
#define CBO_INVAL(offset)    asm volatile("fence iorw,iow" ::: "memory")
#endif

#define CACHE_LINE_SIZE      64  /* RISC-V 缓存行大小通常为 64 字节 */

/* ============================================================================
 * 缓存操作 API (使用 libmetal 要求的函数名)
 * ============================================================================ */

/**
 * 使缓存无效
 *
 * @param addr  虚拟地址
 * @param len   大小 (unsigned int 以匹配 libmetal API)
 */
void metal_machine_cache_invalidate(void *addr, unsigned int len)
{
    uintptr_t start = (uintptr_t)addr;
    uintptr_t end = start + len;
    uintptr_t line;

    if (!addr && len == 0) {
        /* 使整个缓存无效 */
        /* 使用 fence 代替 fence.i */
        asm volatile("fence iorw,iow" ::: "memory");
        return;
    }

    /* 对齐到缓存行边界 */
    start = start & ~(CACHE_LINE_SIZE - 1);

    /* 逐缓存行操作 */
    for (line = start; line < end; line += CACHE_LINE_SIZE) {
        CBO_INVAL(line);
    }

    /* 内存屏障 */
    asm volatile("fence iorw,iow" ::: "memory");
}

/**
 * 清除缓存（写回）
 *
 * @param addr  虚拟地址
 * @param len   大小 (unsigned int 以匹配 libmetal API)
 */
void metal_machine_cache_flush(void *addr, unsigned int len)
{
    uintptr_t start = (uintptr_t)addr;
    uintptr_t end = start + len;
    uintptr_t line;

    if (!addr && len == 0) {
        /* 刷新整个缓存 */
        asm volatile("fence ow,ow" ::: "memory");
        /* 使用 fence 代替 fence.i */
        asm volatile("fence iorw,iorw" ::: "memory");
        return;
    }

    /* 对齐到缓存行边界 */
    start = start & ~(CACHE_LINE_SIZE - 1);

    /* 逐缓存行操作：先写回，再无效 */
    for (line = start; line < end; line += CACHE_LINE_SIZE) {
        CBO_CLEAN(line);
        CBO_INVAL(line);
    }

    /* 内存屏障 */
    asm volatile("fence iorw,iorw" ::: "memory");
}
