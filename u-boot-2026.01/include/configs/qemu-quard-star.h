/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2018, Bin Meng <bmeng.cn@gmail.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>

/* 
 * 系统主内存（DRAM）的起始物理地址。
 */
#define CFG_SYS_SDRAM_BASE      0x80000000

/* 
 * RISC-V 计时器配置 (CLINT - Core Local Interrupter)
 * 分别定义了 Machine Mode (M-Mode) 和 Supervisor Mode (S-Mode) 的计时器基址、偏移和频率。
 * 这些宏用于 U-Boot 的时钟驱动，以实现 udelay 等延时函数。
 */
#define RISCV_MMODE_TIMERBASE       0x2000000  /* CLINT 基地址 */
#define RISCV_MMODE_TIMEROFF        0xbff8     /* mtime 寄存器偏移量 */
#define RISCV_MMODE_TIMER_FREQ      1000000    /* M模式计时器频率 (1MHz) */
#define RISCV_SMODE_TIMER_FREQ      1000000    /* S模式计时器频率 (1MHz) */

/* Environment options - 环境变量选项 */

/* 
 * 标准输入输出设备的默认设置。
 * 同时启用串口 (serial) 和 视频控制台 (vidconsole/usbkbd)。
 * 这样无论是在无头模式（仅串口）还是图形模式下都能操作 U-Boot。
 */
#define CFG_STD_DEVICES_SETTINGS    "stdin=serial,usbkbd\0" \
                    "stdout=serial,vidconsole\0" \
                    "stderr=serial,vidconsole\0"

/* 
 * 启动目标设备列表 (Distro Boot)。
 * 定义了 U-Boot 扫描启动介质的优先级顺序：
 * 1. QEMU 虚拟设备(fw_cfg)
 * 2. virtio 块设备 (virtio_blk)
 * config_distro_bootcmd.h 会利用这个列表生成 bootcmd 脚本。
 */
#define BOOT_TARGET_DEVICES(func) \
    func(QEMU, qemu, na) \
    func(VIRTIO, virtio, 0)

/* 包含通用的发行版启动命令生成逻辑 */
#include <config_distro_bootcmd.h>
/* 
 * QEMU 设备的启动命令定义，是针对 BOOT_TARGET_DEVICES 中的 QEMU 设备的具体实现
 * 这里定义了 bootcmd_qemu，用于从 QEMU 虚拟设备加载内核和设备树。
 */
#define BOOTENV_DEV_QEMU(devtypeu, devtypel, instance) \
	"bootcmd_qemu=" \
		"if env exists kernel_start; then " \
			"bootm ${kernel_start} - ${fdtcontroladdr};" \
		"fi;\0"

#define BOOTENV_DEV_NAME_QEMU(devtypeu, devtypel, instance) \
	"qemu "


/* 
 * 默认环境变量集 (通过 env print 可以看到的内容)。
 * 这里硬编码了关键组件加载到内存中的物理地址：
 * 
 * - kernel_addr_r: 内核加载地址 (0x84000000, 距 RAM 起始 64MB)
 * - kernel_comp_addr_r: 压缩内核加载地址 (防止解压时覆盖)
 * - fdt_addr_r: 设备树加载地址
 * - ramdisk_addr_r: initrd 加载地址
 * - initrd_high: 设为全 F 表示不移动 initrd，直接在加载位置运行 (避免重定位开销)
 */
#define CFG_EXTRA_ENV_SETTINGS \
    CFG_STD_DEVICES_SETTINGS \
    "initrd_high=0xffffffffffffffff\0" \
    "kernel_addr_r=0x84000000\0" \
    "kernel_comp_addr_r=0x88000000\0" \
    "kernel_comp_size=0x4000000\0" \
    "fdt_addr_r=0x8c000000\0" \
    "scriptaddr=0x8c100000\0" \
    "pxefile_addr_r=0x8c200000\0" \
    "ramdisk_addr_r=0x8c300000\0" \
    BOOTENV  /* 展开为上述 BOOT_TARGET_DEVICES 生成的具体启动脚本 */

#endif /* __CONFIG_H */