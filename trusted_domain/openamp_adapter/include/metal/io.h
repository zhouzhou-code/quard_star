/*
 * libmetal io.h - FreeRTOS 最小化实现
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件提供 libmetal I/O 区域的最小化实现
 * 避免依赖完整的 libmetal 库
 */

#ifndef METAL_IO_H
#define METAL_IO_H

#include <stdint.h>
#include <stddef.h>
#include "metal/sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 前向声明 */
struct metal_io_region;

/* 物理地址类型 */
typedef unsigned long metal_phys_addr_t;
#define METAL_BAD_PHYS ((metal_phys_addr_t)-1)

/**
 * struct metal_io_ops - I/O 操作函数表
 */
struct metal_io_ops {
    void (*close)(struct metal_io_region *io);
    metal_phys_addr_t (*offset_to_phys)(struct metal_io_region *io,
                                        unsigned long offset);
    unsigned long (*phys_to_offset)(struct metal_io_region *io,
                                     metal_phys_addr_t phys);
    void (*read)(struct metal_io_region *io, unsigned long offset,
                 void *dst, int len);
    void (*write)(struct metal_io_region *io, unsigned long offset,
                  const void *src, int len);
    void (*block_read)(struct metal_io_region *io, unsigned long offset,
                      void *dst, int len);
    void (*block_write)(struct metal_io_region *io, unsigned long offset,
                       const void *src, int len);
};

static inline void *metal_io_phys_to_virt(struct metal_io_region *io,
                                          unsigned long phys)
{
    (void)io;
    return (void *)phys;
}

static inline unsigned long metal_io_virt_to_phys(struct metal_io_region *io,
                                                  void *virt)
{
    (void)io;
    return (unsigned long)virt;
}

void metal_io_block_read(struct metal_io_region *io,
                        unsigned long offset,
                        void *dst,
                        int len);

void metal_io_block_write(struct metal_io_region *io,
                         unsigned long offset,
                         const void *src,
                         int len);

#ifdef __cplusplus
}
#endif

#endif /* METAL_IO_H */
