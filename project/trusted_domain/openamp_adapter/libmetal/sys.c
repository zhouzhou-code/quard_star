/*
 * libmetal FreeRTOS 系统接口实现
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件实现 libmetal 在 FreeRTOS 上的系统接口扩展
 * 参考：libmetal/lib/system/generic/sys.c
 */

#include "metal/io.h"
#include "metal/device.h"
#include "metal/sys.h"
#include "metal/log.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "metal_freertos.h"
#include "hwspecs.h"

#include <stdio.h>

/* ============================================================================
 * libmetal 系统初始化
 * ============================================================================ */

/**
 * 初始化 libmetal 系统
 * 使用 libmetal v1.9.0 的标准 metal_init API
 */
int metal_freertos_sys_init(void)
{
    struct metal_init_params params = METAL_INIT_DEFAULTS;

    /* 使用 libmetal 标准初始化 */
    return metal_init(&params);
}

/**
 * 反初始化 libmetal 系统
 */
void metal_freertos_sys_finish(void)
{
    metal_finish();
}
