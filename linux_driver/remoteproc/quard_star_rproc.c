/*
 * Quard Star Remoteproc Driver Implementation
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件实现 Quard Star SoC 的 remoteproc 平台驱动
 * 用于管理 FreeRTOS (Hart 0) 作为远程处理器
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/remoteproc.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_reserved_mem.h>
#include <linux/io.h>
#include <linux/slab.h>

#include "quard_star_rproc.h"

/* ============================================================================
 * Module 信息
 * ============================================================================ */

#define DRIVER_NAME		"quard_star-rproc"
#define DRIVER_VERSION		"1.0"
#define DRIVER_AUTHOR		"Quard Star Project"
#define DRIVER_DESC		"Quard Star Remoteproc Driver for FreeRTOS Domain"

/* ============================================================================
 * 设备私有数据
 * ============================================================================ */

/**
 * struct quard_star_rproc - Quard Star remoteproc 私有数据
 *
 * @dev: 平台设备
 * @rproc: remoteproc 实例
 * @mailbox_base: Mailbox 寄存器映射基址
 * @vq_irq: virtqueue kick IRQ 编号
 * @rsc_table: Resource Table 映射地址
 * @rsc_table_size: Resource Table 大小
 */
struct quard_star_rproc {
	struct device *dev;
	struct rproc *rproc;
	void __iomem *mailbox_base;
	int vq_irq;
	void __iomem *rsc_table;
	size_t rsc_table_size;
};

/* ============================================================================
 * Remoteproc 操作实现
 * ============================================================================ */

/**
 * quard_star_rproc_start() - 启动远程处理器
 *
 * 在 "Attach-Only" 模式下，FreeRTOS 已经运行
 * 此函数不应该被调用（使用 attach 代替）
 */
static int quard_star_rproc_start(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;

	dev_info(priv->dev, "Attach-Only mode: start() called, but should use attach\n");

	/* "Attach-Only" 模式：不应调用 start，返回成功即可 */
	return 0;
}

/**
 * quard_star_rproc_stop() - 停止远程处理器
 *
 * 在 "Attach-Only" 模式下，不实际停止 FreeRTOS
 * 仅清理资源
 */
static int quard_star_rproc_stop(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;

	dev_info(priv->dev, "Attach-Only mode: not stopping FreeRTOS\n");

	/* "Attach-Only" 模式：不停止 FreeRTOS */
	return 0;
}

/**
 * quard_star_rproc_kick() - 通知远程处理器
 *
 * 通过 Mailbox 发送 IRQ 51 到 FreeRTOS
 */
static void quard_star_rproc_kick(struct rproc *rproc, int vqid)
{
	struct quard_star_rproc *priv = rproc->priv;

	if (!priv->mailbox_base) {
		dev_err(priv->dev, "Mailbox not mapped\n");
		return;
	}

	/* 写 Mailbox 寄存器触发中断到 FreeRTOS (IRQ 51) */
	writel(1, priv->mailbox_base + MAILBOX_REG_LINUX_TRIG);

	/* 内存屏障，确保写操作完成 */
	mmiowb();

	dev_dbg(priv->dev, "Kicked FreeRTOS via Mailbox (vq=%d)\n", vqid);
}

/**
 * quard_star_rproc_attach() - 附加到远程处理器
 *
 * 注册 Mailbox IRQ 处理器 (IRQ 50)
 */
static int quard_star_rproc_attach(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;

	dev_info(priv->dev, "Attaching to FreeRTOS remoteproc\n");

	/* Mailbox IRQ 在 probe 时已注册 */
	/* 此函数仅用于确认附加操作 */

	return 0;
}

/**
 * quard_star_rproc_detach() - 从远程处理器分离
 *
 * 清理资源
 */
static int quard_star_rproc_detach(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;

	dev_info(priv->dev, "Detaching from FreeRTOS remoteproc\n");

	return 0;
}

/**
 * quard_star_rproc_get_loaded_rsc_table() - 获取 Resource Table
 *
 * 从共享内存 (0xbf70c000) 读取 Resource Table
 */
static struct resource_table *quard_star_rproc_get_loaded_rsc_table(struct rproc *rproc,
								   size_t *rsc_table_size)
{
	struct quard_star_rproc *priv = rproc->priv;
	struct resource_table *rsc_table;
	void __iomem *rsc_base;

	if (!rsc_table_size) {
		dev_err(priv->dev, "Invalid rsc_table_size pointer\n");
		return ERR_PTR(-EINVAL);
	}

	/* 映射 Resource Table 物理内存 */
	rsc_base = ioremap_wc(QUARD_STAR_RPROC_RSC_PA, QUARD_STAR_RPROC_RSC_SIZE);
	if (!rsc_base) {
		dev_err(priv->dev, "Failed to map Resource Table memory\n");
		return ERR_PTR(-ENOMEM);
	}

	/* 验证 Resource Table 版本 */
	rsc_table = (struct resource_table *)rsc_base;
	if (rsc_table->ver != 1) {
		dev_err(priv->dev, "Invalid Resource Table version: %u\n",
			rsc_table->ver);
		iounmap(rsc_base);
		return ERR_PTR(-EINVAL);
	}

	/* 保存 Resource Table 信息 */
	priv->rsc_table = rsc_table;
	priv->rsc_table_size = QUARD_STAR_RPROC_RSC_SIZE;
	*rsc_table_size = priv->rsc_table_size;

	dev_info(priv->dev, "Resource Table found: ver=%u, num=%u entries\n",
		 rsc_table->ver, rsc_table->num);

	/* 返回指针（注意：此内存已映射，应在 detach 时解除映射） */
	return rsc_table;
}

/* ============================================================================
 * Mailbox IRQ 处理器
 * ============================================================================ */

/**
 * quard_star_vq_irq_handler() - virtqueue kick IRQ 处理器
 *
 * 当 FreeRTOS 通过 Mailbox 发送 IRQ 50 到 Linux 时调用
 * 通知 RPMsg 有新消息可用
 */
static irqreturn_t quard_star_vq_irq_handler(int irq, void *dev_id)
{
	struct quard_star_rproc *priv = dev_id;
	struct rproc *rproc = priv->rproc;

	/* 清除 Mailbox 中断状态 */
	writel(1, priv->mailbox_base + MAILBOX_REG_LINUX_ACK);

	/* 通知 remoteproc 有数据可用 */
	if (rproc) {
		/* 在新内核中，remoteproc 框架会自动处理 virtqueue 中断 */
		/* 我们只需要清除中断状态 */
	}

	dev_dbg(priv->dev, "VQ IRQ %d handled\n", irq);

	return IRQ_HANDLED;
}

/* ============================================================================
 * Remoteproc 操作表
 * ============================================================================ */

static const struct rproc_ops quard_star_rproc_ops = {
	.start = quard_star_rproc_start,
	.stop = quard_star_rproc_stop,
	.kick = quard_star_rproc_kick,
	.attach = quard_star_rproc_attach,
	.detach = quard_star_rproc_detach,
	.get_loaded_rsc_table = quard_star_rproc_get_loaded_rsc_table,
};

/* ============================================================================
 * 平台设备操作
 * ============================================================================ */

/**
 * quard_star_rproc_probe() - 平台设备 probe
 */
static int quard_star_rproc_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct quard_star_rproc *priv;
	int ret, irq;

	dev_info(dev, "Probing Quard Star remoteproc driver\n");

	/* 分配私有数据 */
	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->dev = dev;
	platform_set_drvdata(pdev, priv);

	/* 映射 Mailbox 寄存器 */
	{
		struct resource res = DEFINE_RES_MEM(QUARD_STAR_MAILBOX_PA,
						   QUARD_STAR_MAILBOX_SIZE);
		priv->mailbox_base = devm_ioremap_resource(dev, &res);
		if (IS_ERR(priv->mailbox_base)) {
			ret = PTR_ERR(priv->mailbox_base);
			dev_err(dev, "Failed to map Mailbox registers: %d\n", ret);
			return ret;
		}
	}

	/* 获取 Mailbox IRQ */
	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		/* 如果设备树没有指定，使用硬编码的 IRQ 50 */
		irq = QUARD_STAR_RPROC_VQ_IRQ;
		dev_info(dev, "Using default VQ IRQ: %d\n", irq);
	}

	priv->vq_irq = irq;

	/* 注册 Mailbox IRQ 处理器 */
	ret = devm_request_irq(dev, irq, quard_star_vq_irq_handler,
			       IRQF_TRIGGER_RISING, dev_name(dev), priv);
	if (ret) {
		dev_err(dev, "Failed to request Mailbox IRQ %d: %d\n", irq, ret);
		return ret;
	}

	dev_info(dev, "Mailbox IRQ %d registered\n", irq);

	/* 分配并注册 remoteproc 实例 (使用 managed API) */
	dev_info(dev, "Calling devm_rproc_alloc...\n");
	priv->rproc = devm_rproc_alloc(dev, "quard-star-rproc",
				       &quard_star_rproc_ops, NULL,
				       sizeof(*priv));
	if (!priv->rproc) {
		dev_err(dev, "Failed to allocate remoteproc instance\n");
		return -ENOMEM;
	}
	dev_info(dev, "devm_rproc_alloc succeeded\n");

	priv->rproc->priv = priv;
	priv->rproc->has_iommu = false; /* 无需 IOMMU */
	priv->rproc->auto_boot = false; /* "Attach-Only" 模式，不自动启动 */

	/* "Attach-Only" 模式：FreeRTOS 已运行，设置状态为 DETACHED */
	/* RPROC_DETACHED = 6，跳过固件加载，使用 attach 路径 */
	priv->rproc->state = 6;  /* RPROC_DETACHED */

	/* 注册 remoteproc 设备 */
	dev_info(dev, "Calling devm_rproc_add...\n");
	ret = devm_rproc_add(dev, priv->rproc);
	if (ret) {
		dev_err(dev, "Failed to register remoteproc: %d\n", ret);
		printk(KERN_ERR "quard_star_rproc: devm_rproc_add failed: %d\n", ret);
		return ret;
	}

	dev_info(dev, "Remoteproc device registered: /sys/class/remoteproc/remoteproc0/\n");
	dev_info(dev, "Remoteproc driver initialized successfully\n");
	printk(KERN_INFO "quard_star_rproc: Driver init complete\n");

	return 0;
}

/**
 * quard_star_rproc_remove() - 平台设备 remove
 */
static int quard_star_rproc_remove(struct platform_device *pdev)
{
	struct quard_star_rproc *priv = platform_get_drvdata(pdev);

	dev_info(priv->dev, "Removing Quard Star remoteproc driver\n");

	/* 清理 Resource Table 映射 */
	if (priv->rsc_table) {
		iounmap(priv->rsc_table);
		priv->rsc_table = NULL;
	}

	/* remoteproc 实例由 devm_* 自动管理，无需手动释放 */

	return 0;
}

/* ============================================================================
 * 设备树匹配表
 * ============================================================================ */

static const struct of_device_id quard_star_rproc_match[] = {
	{ .compatible = "quard-star,rproc", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, quard_star_rproc_match);

static struct platform_driver quard_star_rproc_driver = {
	.probe = quard_star_rproc_probe,
	.remove = quard_star_rproc_remove,
	.driver = {
		.name = DRIVER_NAME,
		.of_match_table = quard_star_rproc_match,
	},
};
module_platform_driver(quard_star_rproc_driver);

/* ============================================================================
 * Module 元数据
 * ============================================================================ */

MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
