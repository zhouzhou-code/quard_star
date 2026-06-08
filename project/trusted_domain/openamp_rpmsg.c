/*
 * OpenAMP-based RPMsg for FreeRTOS trusted domain (业界标准复用件)
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 用 OpenAMP(open-amp + libmetal) 标准库实现 FreeRTOS 侧的 rpmsg：
 *   - remoteproc(DEVICE 角色) + 静态资源表(resource_table.c, @0xbf70c000)
 *   - remoteproc_create_virtio -> rpmsg_init_vdev -> rpmsg_create_ept
 *   - notify 经 ARM MHU：FreeRTOS->Linux 写 CPU0 SET；poll 时清 CPU1 并处理收包
 *
 * 替代手写的 simple_rpmsg.c。内存契约(vring/buffer/资源表地址)与原来一致，
 * 故 Linux 侧 remoteproc 驱动 + virtio_rpmsg_bus 无需任何改动。
 */

#include <openamp/open_amp.h>
#include <openamp/remoteproc.h>
#include <openamp/remoteproc_virtio.h>
#include <openamp/rpmsg_virtio.h>
#include <metal/io.h>
#include <metal/device.h>
#include <metal/alloc.h>
#include <metal/sys.h>

#include <string.h>

#include "hwspecs.h"
#include "openamp_rpmsg.h"
/* 不能 include resource_table.h：其 fw_rsc_* 结构与 open-amp 的同名结构冲突。
 * 这里只需资源表的地址/大小(来自 hwspecs.h)，表内容由 resource_table.c 静态提供。 */

/* UART 打印（与原 simple_rpmsg 行为一致，供 test_openamp2.py 匹配）*/
extern void uart8250_puts(const char *s);

#define EPT_NAME        "freertos-test"
#define EPT_LOCAL_ADDR  0x30

#define SHBUF_OFFSET    0x4000                 /* buffers @ SHM_BASE+0x4000 */

static struct remoteproc          g_rproc;
static struct rpmsg_virtio_device g_rvdev;
static struct rpmsg_virtio_shm_pool g_shpool;
static struct rpmsg_device        *g_rdev;
static struct rpmsg_endpoint       g_ept;
static struct virtio_device       *g_vdev;
static int                         g_inited;

/* ---- remoteproc ops：mmap(恒等映射) + notify(戳 MHU) ---- */

static struct remoteproc *rproc_init_op(struct remoteproc *rproc,
                                        const struct remoteproc_ops *ops, void *arg)
{
    (void)arg;
    rproc->ops = ops;
    rproc->priv = NULL;
    return rproc;
}

static void rproc_remove_op(struct remoteproc *rproc) { (void)rproc; }

/* va == pa 的恒等映射（QEMU 物理地址直访），仿 open-amp 通用实现 */
static void *rproc_mmap_op(struct remoteproc *rproc, metal_phys_addr_t *pa,
                           metal_phys_addr_t *da, size_t size,
                           unsigned int attribute, struct metal_io_region **io)
{
    struct remoteproc_mem *mem;
    metal_phys_addr_t lpa, lda;
    struct metal_io_region *tmpio;

    (void)attribute;
    lpa = *pa;
    lda = *da;
    if (lpa == METAL_BAD_PHYS && lda == METAL_BAD_PHYS)
        return NULL;
    if (lpa == METAL_BAD_PHYS)
        lpa = lda;
    if (lda == METAL_BAD_PHYS)
        lda = lpa;

    mem = metal_allocate_memory(sizeof(*mem));
    if (!mem)
        return NULL;
    tmpio = metal_allocate_memory(sizeof(*tmpio));
    if (!tmpio) {
        metal_free_memory(mem);
        return NULL;
    }
    remoteproc_init_mem(mem, NULL, lpa, lda, size, tmpio);
    metal_io_init(tmpio, (void *)(uintptr_t)lpa, &mem->pa, size,
                  sizeof(metal_phys_addr_t) << 3, 0, NULL);
    remoteproc_add_mem(rproc, mem);
    *pa = lpa;
    *da = lda;
    if (io)
        *io = tmpio;
    return metal_io_phys_to_virt(tmpio, mem->pa);
}

static int rproc_notify_op(struct remoteproc *rproc, uint32_t id)
{
    (void)rproc;
    (void)id;
    /* FreeRTOS -> Linux: 写 ARM MHU CPU0 SET 触发 IRQ50 */
    *(volatile uint32_t *)(MAILBOX_BASE_ADDR + MHU_CPU0_SET) = MHU_RPMSG_DBELL;
    __asm__ volatile("fence ow,ow" ::: "memory");
    return 0;
}

static const struct remoteproc_ops g_rproc_ops = {
    .init   = rproc_init_op,
    .remove = rproc_remove_op,
    .mmap   = rproc_mmap_op,
    .notify = rproc_notify_op,
    .start  = NULL,
    .stop   = NULL,
    .shutdown = NULL,
};

/* ---- rpmsg 端点回调：回显 ---- */

static int ept_cb(struct rpmsg_endpoint *ept, void *data, size_t len,
                  uint32_t src, void *priv)
{
    char buf[256];
    char echo[300];
    size_t n = (len < sizeof(buf) - 1) ? len : sizeof(buf) - 1;

    (void)src;
    (void)priv;

    memcpy(buf, data, n);
    buf[n] = '\0';

    uart8250_puts("[RPMsg] Received: ");
    uart8250_puts(buf);
    uart8250_puts("\r\n");

    /* 回显 "Echo from FreeRTOS: <msg>" 给 Linux */
    strcpy(echo, "Echo from FreeRTOS: ");
    strncat(echo, buf, sizeof(echo) - strlen(echo) - 1);
    rpmsg_send(ept, echo, strlen(echo));

    uart8250_puts("[RPMsg] Sent: ");
    uart8250_puts(echo);
    uart8250_puts("\r\n");

    return RPMSG_SUCCESS;
}

/* ---- 对外 API ---- */

int openamp_rpmsg_init(void)
{
    struct metal_init_params mp = METAL_INIT_DEFAULTS;
    metal_phys_addr_t pa;
    void *rsc = (void *)(uintptr_t)RESOURCE_TABLE_ADDR;
    int rsc_size = (int)RESOURCE_TABLE_SIZE;
    struct metal_io_region *shbuf_io;
    void *shbuf;
    int ret;

    if (g_inited)
        return 0;

    if (metal_init(&mp)) {
        uart8250_puts("[OpenAMP] metal_init failed\r\n");
        return -1;
    }

    if (!remoteproc_init(&g_rproc, &g_rproc_ops, NULL)) {
        uart8250_puts("[OpenAMP] remoteproc_init failed\r\n");
        return -1;
    }

    /* 恒等映射资源表区 + 共享内存区 */
    pa = RESOURCE_TABLE_ADDR;
    (void)rproc_mmap_op(&g_rproc, &pa, NULL, RESOURCE_TABLE_SIZE, 0, &g_rproc.rsc_io);
    pa = SHM_BASE_ADDR;
    (void)rproc_mmap_op(&g_rproc, &pa, NULL, SHM_SIZE, 0, NULL);

    if (remoteproc_set_rsc_table(&g_rproc, rsc, rsc_size)) {
        uart8250_puts("[OpenAMP] set_rsc_table failed\r\n");
        return -1;
    }

    /* DEVICE 角色：Linux 是 driver/master，FreeRTOS 是 device */
    g_vdev = remoteproc_create_virtio(&g_rproc, 0, VIRTIO_DEV_DEVICE, NULL);
    if (!g_vdev) {
        uart8250_puts("[OpenAMP] create_virtio failed\r\n");
        return -1;
    }

    shbuf_io = remoteproc_get_io_with_pa(&g_rproc, SHM_BASE_ADDR);
    shbuf = metal_io_phys_to_virt(shbuf_io, SHM_BASE_ADDR + SHBUF_OFFSET);
    rpmsg_virtio_init_shm_pool(&g_shpool, shbuf, RPMSG_BUF_SIZE);

    ret = rpmsg_init_vdev(&g_rvdev, g_vdev, NULL, shbuf_io, &g_shpool);
    if (ret) {
        uart8250_puts("[OpenAMP] rpmsg_init_vdev failed\r\n");
        return -1;
    }
    g_rdev = rpmsg_virtio_get_rpmsg_device(&g_rvdev);

    /* 创建命名端点(DEVICE 侧创建即向 Linux 发 NS 宣告) */
    ret = rpmsg_create_ept(&g_ept, g_rdev, EPT_NAME,
                           EPT_LOCAL_ADDR, RPMSG_ADDR_ANY, ept_cb, NULL);
    if (ret) {
        uart8250_puts("[OpenAMP] create_ept failed\r\n");
        return -1;
    }

    g_inited = 1;
    uart8250_puts("[OpenAMP] rpmsg ready (ept=freertos-test)\r\n");
    return 0;
}

void openamp_rpmsg_poll(void)
{
    if (!g_inited)
        return;
    /* 清 Linux 的 CPU1 门铃(应答)，再处理 RX virtqueue */
    *(volatile uint32_t *)(MAILBOX_BASE_ADDR + MHU_CPU1_CLR) = MHU_RPMSG_DBELL;
    __asm__ volatile("fence ow,ow" ::: "memory");
    remoteproc_get_notification(&g_rproc, RSC_NOTIFY_ID_ANY);
}
