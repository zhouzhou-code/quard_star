/*
 * Simple RPMsg Implementation for FreeRTOS (Fixed - Correct Host/Device Roles)
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * Virtio 角色分配（Gemini 修正）：
 * - Host (Linux): 生产者，写入 avail，读取 used
 * - Device (FreeRTOS): 消费者，读取 avail，写入 used
 *
 * 关键修复：
 * 1. 发送时：从 avail 读取 buffer，写入 used
 * 2. 接收时：从 avail 读取消息，写入 used
 * 3. 维护本地消费索引 (tx_avail_idx, rx_avail_idx)
 * 4. 使用 Resource Table 中的实际 vring 地址
 * 5. 等待 Linux 提供空 buffer（Race Condition 修复）
 */

#include "simple_rpmsg.h"
#include "hwspecs.h"
#include "uart8250.h"
#include "bsp_include/resource_table.h"

#include "FreeRTOS.h"
#include "task.h"

#include <string.h>
#include <stdio.h>



/* ============================================================================
 * VirtQueue 结构定义
 * ============================================================================ */

#define VIRTIO_DESC_F_NEXT     1
#define VIRTIO_DESC_F_WRITE    2

struct vring_desc {
    uint64_t addr;   /* Address (guest-physical) */
    uint32_t len;    /* Length */
    uint16_t flags;
    uint16_t next;
};

struct vring_avail {
    uint16_t flags;
    uint16_t idx;
    uint16_t ring[];
};

struct vring_used {
    uint16_t flags;
    uint16_t idx;
    struct {
        uint32_t id;
        uint32_t len;
    } ring[];
};

struct vring {
    struct vring_desc *desc;
    struct vring_avail *avail;
    struct vring_used *used;
};

/* RPMsg 地址定义 */
#define RPMSG_ADDR_ANY        0xFFFFFFFF
#define RPMSG_NS_ADDR         53

/* NS 消息标志 */
#define RPMSG_NS_DESTROY      1
#define RPMSG_NS_CREATE       0

/* RPMsg 消息头 */
struct rpmsg_hdr {
    uint32_t src;
    uint32_t dst;
    uint32_t reserved;
    uint16_t len;
    uint16_t flags;
    uint8_t  data[];
} __attribute__((packed));

/* RPMsg NS (Name Service) 消息结构 */
struct rpmsg_ns_msg {
    char name[32];
    uint32_t addr;
    uint32_t flags;
} __attribute__((packed));

/* ============================================================================
 * 全局变量
 * ============================================================================ */

static struct vring g_vring0;  /* TX vring (FreeRTOS -> Linux) */
static struct vring g_vring1;  /* RX vring (Linux -> FreeRTOS) */
static struct rpmsg_endpoint g_ept;
static uint32_t g_local_addr = 0x30;  /* 本地端点地址 */
static uint32_t g_remote_addr = RPMSG_NS_ADDR; /* 业务消息目的地址，收到首包后更新 */
static int g_rpmsg_initialized = 0;

/* ⚠️ 关键：本地消费索引（由 Device 维护）*/
static uint16_t tx_avail_idx = 0;  /* TX 环的本地消费索引 */
static uint16_t rx_avail_idx = 0;  /* RX 环的本地消费索引 */

/* Mailbox 寄存器 */
/* 注意: offset 是字节偏移。base 转成字节指针再加，否则 uint32_t* 指针运算会 ×4。
 * (v1 时 offset 恒为 0x00 故未暴露; v2 偏移 0x04 起必须按字节算) */
#define MAILBOX_REG(base, offset)  (*(volatile uint32_t *)((volatile uint8_t *)(base) + (offset)))

/* ============================================================================
 * VirtQueue 初始化
 * ============================================================================ */

/**
 * 初始化 VirtQueue 指针
 */
static void vring_init(struct vring *vr, uintptr_t pa)
{
    vr->desc  = (struct vring_desc *)pa;
    vr->avail = (struct vring_avail *)(pa + VRING_SIZE * sizeof(struct vring_desc));

    /* used 部分需要对齐到页边界 */
    uint32_t used_offset = (VRING_SIZE * sizeof(struct vring_desc) +
                           sizeof(uint16_t) + sizeof(uint16_t) +
                           VRING_SIZE * sizeof(uint16_t));
    used_offset = (used_offset + 0xfff) & ~0xfff;  /* 页对齐 */
    vr->used = (struct vring_used *)(pa + used_offset);
}

/* ============================================================================
 * Mailbox 操作（硬件门铃）
 * ============================================================================ */

/**
 * 触发 Mailbox 中断到 Linux
 */
static void mailbox_kick_to_linux(void)
{
    volatile uint32_t *mailbox = (volatile uint32_t *)MAILBOX_BASE_ADDR;

    /* QS-Mailbox v2：写 to-Linux bank ch0 SET 置 bit0 → 触发 IRQ50 通知 Linux */
    MAILBOX_REG(mailbox, QSMB_TL_SET) = QSMB_RPMSG_DBELL;

    /* RISC-V 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");
}

/* ============================================================================
 * RPMsg 实现
 * ============================================================================ */

/**
 * 初始化 Simple RPMsg
 * ✅ 修复：使用 Resource Table 中的实际 vring 地址
 */
int simple_rpmsg_init(void)
{
    if (g_rpmsg_initialized) {
        return 0;
    }

    /* ⚠️ 关键修复：从 Resource Table 读取 vring 地址 */
    volatile struct shared_resource_table *rsc =
        (volatile struct shared_resource_table *)RESOURCE_TABLE_ADDR;

    uintptr_t vring0_pa = (uintptr_t)rsc->vring0.da;
    uintptr_t vring1_pa = (uintptr_t)rsc->vring1.da;

    /* 使用 Resource Table 中的实际地址初始化 vring */
    vring_init(&g_vring0, vring0_pa);
    vring_init(&g_vring1, vring1_pa);

    /* 初始化本地消费索引 */
    tx_avail_idx = 0;
    rx_avail_idx = 0;
    g_remote_addr = RPMSG_NS_ADDR;

    /* ✅ 修复：正确打印 vring PA */
    char buf[64];
    snprintf(buf, sizeof(buf), "[RPMsg] vring0 PA = 0x%lx\r\n", vring0_pa);
    uart8250_puts(buf);

    snprintf(buf, sizeof(buf), "[RPMsg] vring1 PA = 0x%lx\r\n", vring1_pa);
    uart8250_puts(buf);

    uart8250_puts("[RPMsg] Local consumer indices reset\r\n");

    /* 清空端点 */
    memset(&g_ept, 0, sizeof(g_ept));

    g_rpmsg_initialized = 1;

    uart8250_puts("[RPMsg] Initialized successfully\r\n");

    return 0;
}

/**
 * 发送 NS (Name Service) 宣告包
 * ✅ 修正：从 avail 读取 buffer，写入 used
 * ✅ 修正：等待 Linux 提供空 buffer（Race Condition 修复）
 */
static int send_ns_announce(struct rpmsg_endpoint *ept)
{
    struct vring *vr = &g_vring0;  /* TX 环 */
    struct vring_desc *desc;
    struct rpmsg_hdr *hdr;
    struct rpmsg_ns_msg *ns_msg;
    uint16_t desc_idx;
    uint32_t msg_len;
    uint8_t *buffer;
    uint16_t used_idx;

    if (!g_rpmsg_initialized) {
        return -1;
    }

    msg_len = sizeof(struct rpmsg_hdr) + sizeof(struct rpmsg_ns_msg);

    /* ✅ 终极修复：死等 Linux 提供空 buffer（Race Condition 修复）*/
    uint32_t wait_count = 0;
    const uint32_t MAX_WAIT = 100;  /* 最多等待 5 秒 */

    while (tx_avail_idx == vr->avail->idx) {
        if (wait_count == 0) {
            uart8250_puts("[RPMsg] Waiting for Linux to provide empty buffers...\r\n");
        }

        vTaskDelay(pdMS_TO_TICKS(50));  /* 等 50ms */
        wait_count++;

        if (wait_count >= MAX_WAIT) {
            uart8250_puts("[RPMsg] Timeout waiting for buffers!\r\n");
            return -1;
        }

        /* 每 1 秒打印一次 */
        if (wait_count % 20 == 0) {
            uart8250_puts("[RPMsg] Still waiting... (");
            uart8250_putc('0' + (wait_count / 20));
            uart8250_puts("s)\r\n");
        }
    }

    uart8250_puts("[RPMsg] Empty buffers received from Linux!\r\n");

    /* ✅ 1. 从 avail 环消费一个描述符（Linux 提供的）*/
    desc_idx = vr->avail->ring[tx_avail_idx % VRING_SIZE];
    desc = &vr->desc[desc_idx];

    uart8250_puts("[RPMsg] NS: Got descriptor ");
    /* 简单打印 */
    if (desc_idx < 10) {
        uart8250_putc('0' + desc_idx);
    }
    uart8250_puts(" from avail\r\n");

    /* 使用 descriptor 中提供的 buffer 地址 */
    buffer = (uint8_t *)(uintptr_t)desc->addr;

    if (msg_len > desc->len) {
        uart8250_puts("[RPMsg] NS payload too large for descriptor\r\n");
        return -1;
    }

    /* ✅ 修复：正确打印 buffer 地址 */
    char buf[64];
    snprintf(buf, sizeof(buf), "[RPMsg] NS: buffer addr = 0x%lx\r\n", (uintptr_t)desc->addr);
    uart8250_puts(buf);

    /* 2. 构建 RPMsg + NS 宣告包 */
    hdr = (struct rpmsg_hdr *)buffer;
    hdr->src = g_local_addr;
    hdr->dst = RPMSG_NS_ADDR;
    hdr->reserved = 0;
    hdr->len = sizeof(struct rpmsg_ns_msg);
    hdr->flags = 0;

    ns_msg = (struct rpmsg_ns_msg *)hdr->data;
    memset(ns_msg, 0, sizeof(*ns_msg));
    strncpy(ns_msg->name, ept->name, sizeof(ns_msg->name) - 1);
    ns_msg->addr = g_local_addr;
    ns_msg->flags = RPMSG_NS_CREATE;

    /* 3. ✅ 将描述符生产到 used 环交差 */
    used_idx = vr->used->idx % VRING_SIZE;
    vr->used->ring[used_idx].id = desc_idx;
    vr->used->ring[used_idx].len = msg_len;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    /* 更新 used->idx */
    vr->used->idx++;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    /* 4. 更新本地消费索引 */
    tx_avail_idx++;

    /* 5. 发送硬件门铃（Kick Linux）*/
    mailbox_kick_to_linux();

    uart8250_puts("[RPMsg] NS announce sent: ");
    uart8250_puts(ept->name);
    uart8250_puts(" -> addr 0x30\r\n");

    return 0;
}

/**
 * 创建 RPMsg 端点
 */
int simple_rpmsg_create_ept(struct rpmsg_endpoint *ept,
                            const char *name,
                            void (*cb)(struct rpmsg_endpoint *, void *, size_t, uint32_t),
                            void *priv)
{
    if (!g_rpmsg_initialized) {
        uart8250_puts("[RPMsg] Not initialized!\r\n");
        return -1;
    }

    memset(ept, 0, sizeof(*ept));
    ept->addr = g_local_addr;
    if (name) {
        strncpy(ept->name, name, sizeof(ept->name) - 1);
    }
    ept->cb = cb;
    ept->priv = priv;

    /* 复制到全局端点 */
    memcpy(&g_ept, ept, sizeof(*ept));

    uart8250_puts("[RPMsg] Endpoint created: ");
    uart8250_puts(name ? name : "null");
    uart8250_puts("\r\n");

    return 0;
}

/**
 * 发送 NS 宣告包（在 Linux attach 之后调用）
 */
int simple_rpmsg_announce_endpoint(void)
{
    return send_ns_announce(&g_ept);
}

/**
 * 发送 RPMsg 消息
 * ✅ 修正：从 avail 读取 buffer，写入 used
 * ✅ 修正：等待 Linux 提供空 buffer
 */
int simple_rpmsg_send(struct rpmsg_endpoint *ept, const void *data, size_t len)
{
    struct vring *vr = &g_vring0;  /* TX 环 */
    struct vring_desc *desc;
    struct rpmsg_hdr *hdr;
    uint16_t desc_idx;
    uint32_t msg_len;
    uint8_t *buffer;
    uint16_t used_idx;

    (void)ept;  /* 未使用 */

    if (!g_rpmsg_initialized) {
        return -1;
    }

    msg_len = sizeof(struct rpmsg_hdr) + len;

    /* ✅ 等待 Linux 提供空 buffer（简短等待）*/
    uint32_t wait_count = 0;
    const uint32_t MAX_WAIT = 20;  /* 最多等待 1 秒 */

    while (tx_avail_idx == vr->avail->idx) {
        vTaskDelay(pdMS_TO_TICKS(50));
        wait_count++;

        if (wait_count >= MAX_WAIT) {
            /* 没有可用 buffer，放弃 */
            return -1;
        }
    }

    /* ✅ 1. 从 avail 环消费一个描述符 */
    desc_idx = vr->avail->ring[tx_avail_idx % VRING_SIZE];
    desc = &vr->desc[desc_idx];

    /* 使用 descriptor 中提供的 buffer 地址 */
    buffer = (uint8_t *)(uintptr_t)desc->addr;

    if (msg_len > desc->len) {
        return -1;
    }

    /* 2. 构建 RPMsg 消息头 */
    hdr = (struct rpmsg_hdr *)buffer;
    hdr->src = g_local_addr;
    hdr->dst = g_remote_addr;
    hdr->reserved = 0;
    hdr->len = len;
    hdr->flags = 0;

    /* 拷贝数据 */
    if (data && len > 0) {
        memcpy(hdr->data, data, len);
    }

    /* 3. ✅ 将描述符生产到 used 环 */
    used_idx = vr->used->idx % VRING_SIZE;
    vr->used->ring[used_idx].id = desc_idx;
    vr->used->ring[used_idx].len = msg_len;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    vr->used->idx++;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    /* 4. 更新本地消费索引 */
    tx_avail_idx++;

    /* 5. 发送硬件门铃 */
    mailbox_kick_to_linux();

    uart8250_puts("[RPMsg] Sent: ");
    uart8250_puts((const char *)data);
    uart8250_puts("\r\n");

    return 0;
}

/**
 * 轮询处理 RPMsg 消息
 * ✅ 修正：从 avail 读取消息，写入 used
 */
void simple_rpmsg_poll(void)
{
    struct vring *vr = &g_vring1;  /* RX 环 */
    struct vring_desc *desc;
    struct rpmsg_hdr *hdr;
    uint16_t desc_idx;
    uint8_t *buffer;
    uint16_t used_idx;

    if (!g_rpmsg_initialized) {
        return;
    }

    /* ✅ 检查 Linux 是否有新消息 */
    if (rx_avail_idx == vr->avail->idx) {
        return;  /* 没有新消息 */
    }

    /* ✅ 1. 从 avail 环消费一条新消息 */
    desc_idx = vr->avail->ring[rx_avail_idx % VRING_SIZE];
    desc = &vr->desc[desc_idx];
    buffer = (uint8_t *)(uintptr_t)desc->addr;
    hdr = (struct rpmsg_hdr *)buffer;

    if (desc->len >= sizeof(*hdr) && hdr->len <= (desc->len - sizeof(*hdr))) {
        /* 记录对端业务地址，后续 simple_rpmsg_send 使用 */
        g_remote_addr = hdr->src;

        uart8250_puts("[RPMsg] Received: ");
        uart8250_puts((const char *)hdr->data);
        uart8250_puts("\r\n");

        /* 2. 触发回调处理业务 */
        if (g_ept.cb) {
            g_ept.cb(&g_ept, hdr->data, hdr->len, hdr->src);
        }
    } else {
        uart8250_puts("[RPMsg] Drop malformed packet\r\n");
    }

    /* 3. ✅ 将 Buffer 生产到 used 环，退还给 Linux */
    used_idx = vr->used->idx % VRING_SIZE;
    vr->used->ring[used_idx].id = desc_idx;
    vr->used->ring[used_idx].len = 0;  /* 退还空 Buffer，长度填 0 */

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    vr->used->idx++;

    /* 内存屏障 */
    __asm__ volatile("fence ow,ow" ::: "memory");

    /* 4. 更新本地消费索引 */
    rx_avail_idx++;

    /* 5. 发送硬件门铃通知 Linux 回收 */
    mailbox_kick_to_linux();
}
