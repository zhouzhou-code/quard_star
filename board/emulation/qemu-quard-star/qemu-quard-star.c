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

phys_size_t get_effective_memsize(void)
{
	return SZ_32M;
}
