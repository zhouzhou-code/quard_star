/*
 * libmetal FreeRTOS 系统配置
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件定义 libmetal 在 FreeRTOS 上的编译时配置
 */

#ifndef METAL_FREERTOS_H
#define METAL_FREERTOS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * libmetal 系统配置宏
 * ============================================================================ */

/**
 * 使用自定义内存分配器
 * 映射到 FreeRTOS 的 pvPortMalloc/vPortFree
 */
#define METAL_CUSTOM_ALLOCATOR    1

/**
 * 启用日志功能
 * 映射到 FreeRTOS 的 printf
 */
#define METAL_LOG_ENABLE          1
#define METAL_LOG_LEVEL           METAL_LOG_INFO

/**
 * 禁用原子操作（FreeRTOS 单核 Hart 0 不需要）
 */
#define METAL_DISABLE_ATOMICS     0

/**
 * 最大 IRQ 数量
 * PLIC 支持最多 1024 个中断源
 */
#define METAL_MAX_DEVICE_IRQS     1024

/**
 * 共享内存物理地址
 */
#define METAL_SHM_BASE_ADDR       SHM_BASE_ADDR
#define METAL_SHM_SIZE            SHM_SIZE

/**
 * Mailbox IRQ 配置
 */
#define METAL_MAILBOX_IRQ         MAILBOX_IRQ_TO_RTOS

#ifdef __cplusplus
}
#endif

#endif /* METAL_FREERTOS_H */
