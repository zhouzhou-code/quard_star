/*
 * libmetal log.h - 最小化存根
 *
 * Copyright (c) 2025 Quard Star Project
 */

#ifndef __METAL_LOG_H__
#define __METAL_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 空的日志宏 */
#define metal_log(level, fmt, ...)
#define metal_err(fmt, ...)
#define metal_warn(fmt, ...)
#define metal_info(fmt, ...)
#define metal_debug(fmt, ...)

#ifdef __cplusplus
}
#endif

#endif /* __METAL_LOG_H__ */
