// SPDX-License-Identifier: GPL-2.0
/*
 * RV-Mailbox 自测驱动（QEMU virt 通用平台演示）
 *
 * 目的：在 QEMU 标准 `virt` 机器上证明 RV-Mailbox 通用邮箱设备工作正常——
 * 一个 Linux 平台驱动即可在 virt 上完成 doorbell 的"自环"测试：
 *   probe 时 unmask bank0 ch0 bit0 → 写 SET 敲自己门铃 → 触发 PLIC 中断 →
 *   ISR 读 STAT、写 CLEAR 应答 → 完成量唤醒 → 判定 PASS。
 *
 * 该驱动与具体机器无关，仅依赖设备树节点 compatible = "qemu,riscv-mailbox"。
 *
 * 编译（随 ./build.sh driver 自动编译）：
 *   make KERNELDIR=<linux> CROSS_COMPILE=riscv64-unknown-linux-gnu-
 * 使用：
 *   insmod rv_mailbox_selftest.ko   # probe 自动跑自测，dmesg 看 PASS/FAIL
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/io.h>
#include <linux/completion.h>

/* bank0（在 virt 上 bank0 的中断线 IRQ0 接到 PLIC→本 Linux） */
#define RVMB_STAT          0x000   /* RO  doorbell 状态 */
#define RVMB_SET           0x004   /* W1S 置位（敲门铃） */
#define RVMB_CLEAR         0x008   /* W1C 清位（应答） */
#define RVMB_MASK_STAT     0x00C   /* RO  屏蔽状态 */
#define RVMB_MASK_SET      0x010   /* W1S 置屏蔽（关中断） */
#define RVMB_MASK_CLEAR    0x014   /* W1C 清屏蔽（开中断） */

#define RVMB_REVISION      0x0F0
#define RVMB_NUM_CHANNELS  0x0F4
#define RVMB_NUM_BANKS     0x0F8

#define RVMB_DBELL_BIT     0x1     /* ch0 bit0 */
#define RVMB_EXPECT_REV    0x0100  /* v1.0 */

struct rvmb_selftest {
	void __iomem *base;
	int irq;
	int irq_count;
	u32 isr_stat;
	struct completion done;
};

static irqreturn_t rvmb_isr(int irq, void *dev_id)
{
	struct rvmb_selftest *st = dev_id;

	/* 读门铃状态 */
	st->isr_stat = readl(st->base + RVMB_STAT);
	/* W1C 清门铃位，电平随之落下 */
	writel(RVMB_DBELL_BIT, st->base + RVMB_CLEAR);
	st->irq_count++;

	complete(&st->done);
	return IRQ_HANDLED;
}

static int rvmb_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct rvmb_selftest *st;
	struct resource *res;
	u32 rev, nch, nbk, stat_after;
	long ret;

	st = devm_kzalloc(dev, sizeof(*st), GFP_KERNEL);
	if (!st)
		return -ENOMEM;
	init_completion(&st->done);
	platform_set_drvdata(pdev, st);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	st->base = devm_ioremap_resource(dev, res);
	if (IS_ERR(st->base))
		return PTR_ERR(st->base);

	/* 1) 读自描述寄存器，确认设备身份 */
	rev = readl(st->base + RVMB_REVISION);
	nch = readl(st->base + RVMB_NUM_CHANNELS);
	nbk = readl(st->base + RVMB_NUM_BANKS);
	dev_info(dev, "RV-Mailbox @ %pa  REVISION=0x%04x  NUM_CHANNELS=%u  NUM_BANKS=%u\n",
		 &res->start, rev, nch, nbk);
	if (rev != RVMB_EXPECT_REV)
		dev_warn(dev, "unexpected REVISION (expect 0x%04x)\n", RVMB_EXPECT_REV);

	/* 2) 申请 bank0 中断 */
	st->irq = platform_get_irq(pdev, 0);
	if (st->irq < 0)
		return st->irq;
	ret = devm_request_irq(dev, st->irq, rvmb_isr, 0, "rv_mailbox_selftest", st);
	if (ret) {
		dev_err(dev, "request_irq(%d) failed: %ld\n", st->irq, ret);
		return ret;
	}
	dev_info(dev, "registered IRQ %d\n", st->irq);

	/* 3) 自环测试：先确保清零并使能 bit0，再敲自己门铃 */
	writel(RVMB_DBELL_BIT, st->base + RVMB_CLEAR);       /* 清残留 */
	writel(RVMB_DBELL_BIT, st->base + RVMB_MASK_CLEAR);  /* unmask bit0 */
	dev_info(dev, "self-test: ringing doorbell (write SET bit0)...\n");
	writel(RVMB_DBELL_BIT, st->base + RVMB_SET);         /* 敲门铃 → 触发中断 */

	/* 4) 等中断回来 */
	if (!wait_for_completion_timeout(&st->done, msecs_to_jiffies(1000))) {
		dev_err(dev, "self-test FAIL: no interrupt within 1s\n");
		writel(RVMB_DBELL_BIT, st->base + RVMB_MASK_SET); /* 复屏蔽 */
		return -ETIMEDOUT;
	}

	stat_after = readl(st->base + RVMB_STAT);
	dev_info(dev,
		 "self-test PASS: irq_count=%d, STAT-in-ISR=0x%x, STAT-after-clear=0x%x\n",
		 st->irq_count, st->isr_stat, stat_after);

	/* 收尾：重新屏蔽，保持设备空闲 */
	writel(RVMB_DBELL_BIT, st->base + RVMB_MASK_SET);
	return 0;
}

static int rvmb_remove(struct platform_device *pdev)
{
	struct rvmb_selftest *st = platform_get_drvdata(pdev);

	if (st && st->base)
		writel(RVMB_DBELL_BIT, st->base + RVMB_MASK_SET);
	return 0;
}

static const struct of_device_id rvmb_of_match[] = {
	{ .compatible = "qemu,riscv-mailbox", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, rvmb_of_match);

static struct platform_driver rvmb_driver = {
	.probe = rvmb_probe,
	.remove = rvmb_remove,
	.driver = {
		.name = "rv_mailbox_selftest",
		.of_match_table = rvmb_of_match,
	},
};
module_platform_driver(rvmb_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quard Star Project");
MODULE_DESCRIPTION("RV-Mailbox self-test driver (QEMU virt demo)");
MODULE_VERSION("1.0");
