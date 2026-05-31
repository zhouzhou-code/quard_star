/*
 * Simple RPMsg Implementation for FreeRTOS
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 这是一个极度简化的 RPMsg 实现，用于快速验证通信功能
 * 不依赖完整的 open-amp/libmetal 库
 */

#ifndef SIMPLE_RPMSG_H
#define SIMPLE_RPMSG_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RPMsg 消息头结构 */
struct rpmsg_hdr {
    uint32_t src;
    uint32_t dst;
    uint32_t reserved;
    uint16_t len;
    uint16_t flags;
    uint8_t  data[];
} __attribute__((packed));

/* RPMsg 端点 */
struct rpmsg_endpoint {
    uint32_t addr;
    char name[32];
    void (*cb)(struct rpmsg_endpoint *ept, void *data, size_t len, uint32_t src);
    void *priv;
};

/* RPMsg 设备 */
struct rpmsg_device {
    struct {
        volatile uint16_t flags;
        volatile uint16_t index;
    } vring0;
    struct {
        volatile uint16_t flags;
        volatile uint16_t index;
    } vring1;
    struct metal_io_region *io;
};

/**
 * 初始化简化版 RPMsg
 */
int simple_rpmsg_init(void);

/**
 * 创建 RPMsg 端点
 */
int simple_rpmsg_create_ept(struct rpmsg_endpoint *ept,
                            const char *name,
                            void (*cb)(struct rpmsg_endpoint *ept,
                                      void *data, size_t len, uint32_t src),
                            void *priv);

/**
 * 发送 NS 宣告包（在 Linux attach 之后调用）
 */
int simple_rpmsg_announce_endpoint(void);

/**
 * 发送 RPMsg 消息
 */
int simple_rpmsg_send(struct rpmsg_endpoint *ept, const void *data, size_t len);

/**
 * 轮询处理 RPMsg 消息
 */
void simple_rpmsg_poll(void);

#ifdef __cplusplus
}
#endif

#endif /* SIMPLE_RPMSG_H */
