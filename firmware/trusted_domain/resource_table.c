/*
 * Resource Table 初始化实现
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件实现 OpenAMP Resource Table 的静态初始化。
 * * 架构背景：
 * 本系统基于 RISC-V OpenSBI Domain 机制实现 AMP 非对称隔离。
 * 实时域 (FreeRTOS) 与计算域 (Linux) 同步启动，Linux 侧的 Remoteproc
 * 工作在纯附加 (Attach / IPC-only) 模式，不负责从核的固件加载与复位。
 */

#include "bsp_include/resource_table.h"

/* ============================================================================
 * Resource Table 实例（紧凑型，使用静态初始化）
 * ============================================================================ 
 * * 【核心设计说明】：
 * 1. 静态链接锁定地址：使用 __attribute__((section(".resource_table"))) 
 * 配合链接脚本，将资源表强行放置在与 Linux 约定的物理共享内存起始地址。
 * 2. 消除启动竞态：由于双核同步启动，若在运行时使用 memcpy 拷贝资源表，
 * 可能引发 Linux 驱动加载时读取到空数据的 Race Condition。采用静态段
 * 编译让数据“上电即存在”，完美解决此时序问题。
 */
static struct shared_resource_table rsc_table_payload
    __attribute__((section(".resource_table"), used)) = {
    .ver = 1,
    .num = 1,
    .reserved = {0, 0},
    /* 计算 vdev 节点在结构体中的相对偏移，Linux 驱动将据此寻址 */
    .offset = { offsetof(struct shared_resource_table, hdr) },
    
    /* * 严格遵循 Linux 6.10 remoteproc.h ABI 布局 
     * hdr 与 vdev 必须在物理内存中紧邻排布
     */
    .hdr = {
        .type = RSC_VDEV,
    },

    .vdev = {
        .id = VIRTIO_ID_RPMSG,   /* 声明这是一个 RPMsg 虚拟设备 */
        /* * 注意：在 Linux 6.10 主线中，框架在 attach 时会使用 IDR 机制
         * 动态分配 notifyid。因此此处保留为 0，由 Linux 驱动后续回填。
         */
        .notifyid = 0,           
        .dfeatures = (1UL << VIRTIO_RPMSG_F_NS), /* 开启名称服务，允许动态协商端点 */
        .gfeatures = 0,
        .config_len = 0,
        .status = 0,             /* 状态字，将由 Linux 端 Virtio 引擎负责驱动和修改 */
        .num_of_vrings = 2,      /* 包含 TX 和 RX 两条传送带 (Vring) */
        .reserved = {0, 0}
    },
    
    /* Vring 0 配置 (通常用作 TX 队列) */
    .vring0 = {
        /* Attach 模式下 Linux 拒绝分配内存，da/pa 必须硬编码为约定的物理绝对地址 */
        .da = VRING0_PA,         
        .align = VRING_ALIGN,    /* 内存对齐控制 (通常为 4096) */
        .num = VRING_SIZE,       /* Vring 描述符环容量 (通常为 256) */
        .notifyid = 0,           /* 等待 Linux 回填 */
        .pa = VRING0_PA
    },
    
    /* Vring 1 配置 (通常用作 RX 队列) */
    .vring1 = {
        .da = VRING1_PA,
        .align = VRING_ALIGN,
        .num = VRING_SIZE,
        .notifyid = 0,           /* 等待 Linux 回填 */
        .pa = VRING1_PA
    }
};

/* 编译期防线：确保 Vring 结构体大小不受编译器 Padding 影响，严格占 20 字节 */
_Static_assert(sizeof(struct fw_rsc_vdev_vring) == 20, "fw_rsc_vdev_vring ABI mismatch");