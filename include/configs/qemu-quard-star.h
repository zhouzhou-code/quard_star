// /* SPDX-License-Identifier: GPL-2.0+ */
// /*
//  * Copyright (C) 2018, Bin Meng <bmeng.cn@gmail.com>
//  */

// #ifndef __CONFIG_H
// #define __CONFIG_H

// #include <linux/sizes.h>

// #define CONFIG_SYS_SDRAM_BASE		0x80000000
// #define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + SZ_2M)

// //#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + SZ_2M) 重定义了，但是一样

// //#define CONFIG_SYS_MALLOC_LEN		SZ_8M //重定义了，另一个地方4M

// //#define CONFIG_SYS_BOOTM_LEN		SZ_64M //重定义了，另一个地方2G

// // #define CFG_SYS_INIT_RAM_ADDR      CONFIG_SYS_SDRAM_BASE
// // #define CFG_SYS_INIT_RAM_SIZE      0x200000  /* 定义 2MB 大小，足够早期使用 */


// #define CONFIG_STANDALONE_LOAD_ADDR	0x80200000

// #define RISCV_MMODE_TIMERBASE		0x2000000
// #define RISCV_MMODE_TIMER_FREQ		1000000

// #define RISCV_SMODE_TIMER_FREQ		1000000

// /* Environment options */

// #ifndef CONFIG_SPL_BUILD
// #define BOOT_TARGET_DEVICES(func) \
// 	func(QEMU, qemu, na) \

// #include <config_distro_bootcmd.h>

// #define BOOTENV_DEV_QEMU(devtypeu, devtypel, instance) \
// 	"bootcmd_qemu=" \
// 		"if env exists kernel_start; then " \
// 			"bootm ${kernel_start} - ${fdtcontroladdr};" \
// 		"fi;\0"

// #define BOOTENV_DEV_NAME_QEMU(devtypeu, devtypel, instance) \
// 	"qemu "

// #define CONFIG_EXTRA_ENV_SETTINGS \
// 	"fdt_high=0xffffffffffffffff\0" \
// 	"initrd_high=0xffffffffffffffff\0" \
// 	"kernel_addr_r=0x84000000\0" \
// 	"fdt_addr_r=0x88000000\0" \
// 	"scriptaddr=0x88100000\0" \
// 	"pxefile_addr_r=0x88200000\0" \
// 	"ramdisk_addr_r=0x88300000\0" \
// 	BOOTENV
// #endif

// #endif /* __CONFIG_H */