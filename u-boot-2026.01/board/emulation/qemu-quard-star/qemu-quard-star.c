// SPDX-License-Identifier: GPL-2.0+

#include <dm.h>
#include <dm/ofnode.h>
#include <env.h>
#include <fdtdec.h>
#include <image.h>
#include <log.h>
#include <spl.h>
#include <init.h>
#include <usb.h>
#include <virtio_types.h>
#include <virtio.h>
#include <linux/sizes.h>

int board_init(void)
{
	return 0;
}
/* 虚函数 */
int board_late_init(void)
{
	return 0;
}

// phys_size_t get_effective_memsize(void)
// {
// 	// return SZ_32M;

// 	// 强制告诉 U-Boot：内存最高只能用到 0xBF800000
//     // 即使物理内存有 1GB，我们也只认到这里
//     return SZ_1G - SZ_8M;
// }

phys_size_t get_effective_memsize(void)
{
    // 强制告诉 U-Boot：内存最高只能用到 0xBF700000
    // 物理总内存 1GB，我们从顶部“砍掉” 9MB 空间：
    // - 顶部的 8MB：留给 OpenSBI 和 FreeRTOS (0xBF800000 - 0xC0000000)
    // - 紧接着的 1MB：留给 OpenAMP 共享内存和缓冲 (0xBF700000 - 0xBF800000)
    return SZ_1G - (9 * SZ_1M);
}

//或者:
// phys_addr_t board_get_usable_ram_top(phys_size_t total_size)
// {
// 	// 强制告诉 U-Boot：内存最高只能用到 0xBF800000
// 	return gd->base+SZ_1G - SZ_8M;
// }