/*
 * Quard Star Remoteproc Driver Implementation
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件实现 Quard Star SoC 的 remoteproc 平台驱动，用于管理 FreeRTOS (Hart 0)
 * 作为远程处理器，实现 AMP 异构多处理器通信。
 *
 * ============================================================================
 * 架构背景
 * ============================================================================
 *
 * 本系统基于 RISC-V OpenSBI Domain 机制实现资源隔离：
 * - Hart 7: FreeRTOS 实时域 (由 OpenSBI Domains 独立启动)
 * - Hart 0-6: Linux 计算域 (由 OpenSBI Domains 独立启动)
 *
 * 两个域同步启动，Linux 不负责 FreeRTOS 的固件加载与复位。
 * 因此本驱动工作在 "Attach-Only" (纯附加) 模式，仅负责：
 * 1. 读取 FreeRTOS 在共享内存中准备好的 Resource Table
 * 2. 解析 VirtIO/vring 配置
 * 3. 注册 Mailbox 中断处理 (IRQ 50)
 * 4. 提供 VirtIO notify 机制
 *
 * ============================================================================
 * 通信机制
 * ============================================================================
 *
 * 【共享内存布局】(与 FreeRTOS 侧 trusted_domain/bsp_include/hwspecs.h 保持一致)
 * 0xbf700000 - 0xbf701fff: vring0 (TX, 8KB)
 * 0xbf702000 - 0xbf703fff: vring1 (RX, 8KB)
 * 0xbf704000 - 0xbf70bfff: rpmsg buffers (32KB)
 * 0xbf70c000 - 0xbf70cfff: Resource Table (4KB) ← 静态初始化，双核同步启动时序安全
 *
 * 【Mailbox 中断机制】
 * - FreeRTOS → Linux: 写 MAILBOX_REG_LINUX_TRIG → 触发 IRQ 50 → Linux ISR 处理
 * - Linux → FreeRTOS: 写 MAILBOX_REG_RTOS_TRIG → 触发 IRQ 51 → FreeRTOS ISR 处理
 *
 * 【VirtIO/RPMsg 协议栈】
 * Linux                     FreeRTOS
 *   |                          |
 *   |-- RPMsg Driver          |-- RPMsg Endpoint
 *   |                          |
 *   |-- Virtio RPMsg          |-- Virtio Device
 *   |        |                 |       |
 *   |-- vring0 (TX) ---------> vring1 (RX)
 *   |                          |
 *   |-- vring1 (RX) <--------- vring0 (TX)
 *            \                 /
 *             \               /
 *            remoteproc.kick  virtio_notify()
 *                 |              |
 *           Mailbox IRQ 50   Mailbox IRQ 51
 *
 * ============================================================================
 * 使用方法
 * ============================================================================
 *
 * 1. 编译驱动：
 *    cd linux_driver/remoteproc
 *    make
 *
 * 2. 加载驱动：
 *    insmod quard_star_rproc.ko
 *
 * 3. 附加到 FreeRTOS：
 *    echo start > /sys/class/remoteproc/remoteproc0/state
 *
 * 4. 检查 RPMsg 设备：
 *    ls -l /dev/rpmsg*
 *
 * 5. 测试通信：
 *    # 发送消息到 FreeRTOS
 *    echo "Hello FreeRTOS" > /dev/rpmsg0
 *
 * ============================================================================
 * 参考资料
 * ============================================================================
 *
 * - Linux remoteproc 框架: Documentation/remoteproc.txt
 * - Resource Table 格式: include/linux/remoteproc.h
 * - VirtIO 规范: https://docs.oasis-open.org/virtio/
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

/*
 * remoteproc core exports rproc_vq_interrupt(), but the declaration is in
 * internal headers not installed for OOT modules.
 *
 * rproc_vq_interrupt() 用于通知 remoteproc 框架：某个 virtqueue 有新消息可用
 *
 * 参数：
 *   @rproc: remoteproc 实例指针
 *   @notifyid: 通知的 virtqueue 索引 (0 或 1)
 *
 * 返回：
 *   IRQ_HANDLED: 如果有 pending 消息被处理
 *   IRQ_NONE: 如果没有 pending 消息
 */
extern irqreturn_t rproc_vq_interrupt(struct rproc *rproc, int notifyid);

/* ============================================================================
 * Module 信息
 * ============================================================================ */

#define DRIVER_NAME		"quard_star-rproc"
#define DRIVER_VERSION	"1.0"
#define DRIVER_AUTHOR	"Quard Star Project"
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

static int quard_star_carveout_map(struct rproc *rproc, struct rproc_mem_entry *mem)
{
	void *va;
	(void)rproc;

	/*
	 * 我们使用固定物理地址契约，避免remoteproc默认的dma_alloc_coherent
	 * 在attach模式下把vring/buffer分配到不可预测地址
	 */
	va = memremap(mem->dma, mem->len, MEMREMAP_WB);
	if (!va)
		return -ENOMEM;

	mem->va = va;
	return 0;
}

static int quard_star_carveout_unmap(struct rproc *rproc, struct rproc_mem_entry *mem)
{
	(void)rproc;
	if (mem->va)
		memunmap(mem->va);
	mem->va = NULL;
	return 0;
}

/* ============================================================================
 * Remoteproc ops 操作实现
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
 * quard_star_rproc_stop()-停止远程处理器
 * 在"Attach-Only"模式下，不实际停止FreeRTOS
 * 仅清理资源
 */
static int quard_star_rproc_stop(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;
	dev_info(priv->dev, "Attach-Only mode: not stopping FreeRTOS\n");
	/* "Attach-Only"模式, 不停止FreeRTOS */
	return 0;
}

/**
 * quard_star_rproc_kick()-通知远程处理器
 *
 * 通过mailbox发送IRQ 51到FreeRTOS
 */
static void quard_star_rproc_kick(struct rproc *rproc, int vqid)
{
	struct quard_star_rproc *priv = rproc->priv;

	if (!priv->mailbox_base) {
		dev_err(priv->dev, "Mailbox not mapped\n");
		return;
	}

	/* Linux -> FreeRTOS: 写 RTOS_TRIG 触发 IRQ 51 */
	writel(1, priv->mailbox_base + MAILBOX_REG_RTOS_TRIG);

	/* 内存屏障，确保写操作完成 */
	mmiowb();

	dev_dbg(priv->dev, "Kicked FreeRTOS via Mailbox (vq=%d)\n", vqid);
}

/**
 * quard_star_rproc_attach() - 附加到远程处理器
 *
 * 注册 Mailbox IRQ 处理器 (IRQ 50)，并通知 FreeRTOS Linux 已就绪
 *
 * 【关键操作】设置 vdev.status = VIRTIO_CONFIG_S_DRIVER_OK (0x04)
 *
 * 这会让 FreeRTOS 知道 Linux 驱动已准备好，可以开始 RPMsg 通信。
 * 此步骤是 VirtIO 握手的关键部分，缺少它会导致 FreeRTOS 超时等待。
 */
static int quard_star_rproc_attach(struct rproc *rproc)
{
	struct quard_star_rproc *priv = rproc->priv;
	void __iomem *rsc_base;
	u32 *vdev_status;

	dev_info(priv->dev, "Attaching to FreeRTOS remoteproc\n");

	/*
	 * 映射 Resource Table 以访问 vdev.status 字段
	 *
	 * Resource Table 布局 (trusted_domain/bsp_include/resource_table.h):
	 *   0x00: ver
	 *   0x04: num
	 *   0x08-0x0F: reserved
	 *   0x10: offset[0]
	 *   0x14: hdr.type
	 *   0x18: vdev.id
	 *   0x1C: vdev.notifyid
	 *   0x20: vdev.dfeatures
	 *   0x24: vdev.gfeatures
	 *   0x28: vdev.config_len
	 *   0x29: vdev.status  ← 我们需要修改这个字段
	 *   0x2A: vdev.num_of_vrings
	 */
	rsc_base = ioremap_wc(QUARD_STAR_RPROC_RSC_PA, QUARD_STAR_RPROC_RSC_SIZE);
	if (!rsc_base) {
		dev_err(priv->dev, "Failed to map Resource Table for status update\n");
		return -ENOMEM;
	}

	/* 指向 vdev.status 字段 (偏移 0x29) */
	vdev_status = (u32 *)(rsc_base + 0x29);

	dev_info(priv->dev, "Current vdev.status = 0x%x\n", readb(vdev_status));

	/*
	 * 设置 DRIVER_OK 状态，通知 FreeRTOS:
	 * "Linux VirtIO 驱动已就绪，可以开始通信"
	 */
	writeb(VIRTIO_CONFIG_S_DRIVER_OK, vdev_status);
	mmiowb();  /* 内存屏障，确保写操作完成 */

	dev_info(priv->dev, "Set vdev.status = 0x%x (DRIVER_OK)\n", VIRTIO_CONFIG_S_DRIVER_OK);

	/*
	 * 保持映射，detach 时清理
	 * 注意：priv->rsc_table 在 get_loaded_rsc_table() 中已经映射
	 * 这里我们再次映射是为了单独处理 status 更新
	 */
	iounmap(rsc_base);

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
 * quard_star_rproc_prepare() - 为远程处理器准备内存
 *
 * 注册 vring 内存给 remoteproc 框架（固定物理地址）
 * RPMsg buffer 由 DTS 的 shared-dma-pool 自动分配
 */
static int quard_star_rproc_prepare(struct rproc *rproc)
{
	struct device *dev = rproc->dev.parent;
	struct rproc_mem_entry *mem;

	dev_info(dev, "Preparing remoteproc resources\n");

	/*
	 * 注册 vring0 的静态物理映射 (TX: FreeRTOS -> Linux)
	 * 必须显式注册，否则 remoteproc 会动态分配错误的地址
	 */
	mem = rproc_mem_entry_init(dev, NULL,
				   (dma_addr_t)QUARD_STAR_VRING0_PA,
				   QUARD_STAR_VRING_SIZE,
				   QUARD_STAR_VRING0_PA,
				   quard_star_carveout_map,
				   quard_star_carveout_unmap,
				   "vdev0vring0");
	if (!mem) {
		dev_err(dev, "Failed to create vring0 carveout\n");
		return -ENOMEM;
	}
	rproc_add_carveout(rproc, mem);
	dev_info(dev, "Registered vring0 carveout: PA=0x%pa, size=0x%x\n",
		 &(dma_addr_t){QUARD_STAR_VRING0_PA}, QUARD_STAR_VRING_SIZE);

	/*
	 * 注册 vring1 的静态物理映射 (RX: Linux -> FreeRTOS)
	 */
	mem = rproc_mem_entry_init(dev, NULL,
				   (dma_addr_t)QUARD_STAR_VRING1_PA,
				   QUARD_STAR_VRING_SIZE,
				   QUARD_STAR_VRING1_PA,
				   quard_star_carveout_map,
				   quard_star_carveout_unmap,
				   "vdev0vring1");
	if (!mem) {
		dev_err(dev, "Failed to create vring1 carveout\n");
		return -ENOMEM;
	}
	rproc_add_carveout(rproc, mem);
	dev_info(dev, "Registered vring1 carveout: PA=0x%pa, size=0x%x\n",
		 &(dma_addr_t){QUARD_STAR_VRING1_PA}, QUARD_STAR_VRING_SIZE);

	/*
	 * vdev0buffer (RPMsg 数据 Buffer):
	 * 由 DTS 的 shared-dma-pool 自动分配
	 * remoteproc_virtio.c 会通过 rproc_find_carveout_by_name("vdev0buffer")
	 * 查找并绑定给 virtio_rpmsg_bus 使用
	 */
	mem = rproc_of_resm_mem_entry_init(dev, 0,
					   QUARD_STAR_RPMSG_BUF_SIZE,
					   QUARD_STAR_RPMSG_BUF_PA,
					   "vdev0buffer");
	if (!mem) {
		dev_err(dev, "Failed to create vdev0buffer carveout\n");
		return -ENOMEM;
	}
	rproc_add_carveout(rproc, mem);
	dev_info(dev, "Registered vdev0buffer carveout: PA=0x%pa, size=0x%zx\n",
		 &(dma_addr_t){QUARD_STAR_RPMSG_BUF_PA}, QUARD_STAR_RPMSG_BUF_SIZE);

	return 0;
}

/**
 * quard_star_rproc_get_loaded_rsc_table() -获取Resource Table
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
 * quard_star_vq_irq_handler() - virtqueue kick IRQ 硬中断处理器
 *
 * 只做最快速的中断确认，然后唤醒 threaded handler
 * 当 FreeRTOS 通过 Mailbox 发送 IRQ 到 Linux 时调用
 */
static irqreturn_t quard_star_vq_irq_handler(int irq, void *dev_id)
{
	struct quard_star_rproc *priv = dev_id;

	/* 清除 Mailbox 中断状态 */
	writel(1, priv->mailbox_base + MAILBOX_REG_LINUX_ACK);

	/* 唤醒 threaded handler */
	return IRQ_WAKE_THREAD;
}

/**
 * quard_star_vq_irq_thread() - virtqueue kick IRQ 线程处理器
 *
 * 在进程上下文中处理 virtqueue 中断，可以睡眠
 */
static irqreturn_t quard_star_vq_irq_thread(int irq, void *dev_id)
{
	struct quard_star_rproc *priv = dev_id;
	struct rproc *rproc = priv->rproc;
	irqreturn_t handled = IRQ_NONE;

	/*
	 * 该 mailbox IRQ 复用为双队列 doorbell，无法直接区分 vq id，
	 * 因此依次探测 vq0/vq1。
	 */
	if (rproc) {
		if (rproc_vq_interrupt(rproc, 0) == IRQ_HANDLED)
			handled = IRQ_HANDLED;
		if (rproc_vq_interrupt(rproc, 1) == IRQ_HANDLED)
			handled = IRQ_HANDLED;
	}

	dev_dbg(priv->dev, "VQ IRQ %d handled\n", irq);

	return handled;
}

/* ============================================================================
 * Remoteproc 操作表
 * ============================================================================ */

static const struct rproc_ops quard_star_rproc_ops = {
	.prepare = quard_star_rproc_prepare,
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
 * quard_star_rproc_probe() - 平台设备probe
 */
static int quard_star_rproc_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct quard_star_rproc *priv;
	int ret, irq;

	dev_info(dev, "Probing Quard Star remoteproc driver\n");

	/*
	 * 1) 分配驱动私有数据（driver context）
	 * devm_kzalloc: 与设备生命周期绑定，probe 失败或 remove 时自动释放。
	 */
	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->dev = dev;
	/* 将 priv 绑定到 pdev，后续 remove/ISR 可通过 platform_get_drvdata 获取 */
	platform_set_drvdata(pdev, priv);

	/*
	 * 2) 初始化 remoteproc 设备的 DMA 内存池
	 * of_reserved_mem_device_init(): 将 DTS 中 reserved-memory 绑定到该设备，
	 * 使 dma_alloc_coherent() 优先从固定保留内存分配（用于 rpmsg buffer 池）。
	 */
	ret = of_reserved_mem_device_init(dev);
	if (ret) {
		dev_warn(dev, "No DMA memory pool configured: %d\n", ret);
		dev_warn(dev, "virtio_rpmsg_bus may fail to allocate buffers\n");
	} else {
		dev_info(dev, "DMA memory pool initialized\n");
	}

	/*
	 * 3) 映射 Mailbox MMIO
	 * devm_ioremap(): 仅做映射，不申请独占 mem region，
	 * 允许 mailbox_test 并行加载；若用 devm_ioremap_resource 会独占资源。
	 */
	priv->mailbox_base = devm_ioremap(dev, QUARD_STAR_MAILBOX_PA,
					  QUARD_STAR_MAILBOX_SIZE);
	if (!priv->mailbox_base) {
		dev_err(dev, "Failed to map Mailbox registers\n");
		ret = -ENOMEM;
		goto err_dma_mem;
	}

	/*
	 * 4) 获取 IRQ
	 * platform_get_irq() 返回的是 Linux 虚拟 IRQ 号，
	 * 硬件 PLIC 中断号来自 DTS 的 interrupts 字段。
	 */
	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		/* 如果设备树没有指定，使用硬编码的 IRQ 50 */
		irq = QUARD_STAR_RPROC_VQ_IRQ;
		dev_info(dev, "Using default VQ IRQ: %d\n", irq);
	}

	priv->vq_irq = irq;

	/*
	 * 5) 注册 Mailbox IRQ 处理器（threaded IRQ）
	 * 原因：rproc_vq_interrupt -> virtqueue 回调路径可能睡眠（例如 mutex_lock），
	 * 硬 IRQ 上下文禁止睡眠，因此需要 threaded IRQ。
	 *
	 * hardirq: quard_star_vq_irq_handler  (只 ACK 中断并返回 IRQ_WAKE_THREAD)
	 * thread:  quard_star_vq_irq_thread   (调用 rproc_vq_interrupt，可睡眠)
	 */
	ret = devm_request_threaded_irq(dev, irq,
						 quard_star_vq_irq_handler,
						 quard_star_vq_irq_thread,
					 IRQF_TRIGGER_RISING | IRQF_SHARED,
					 dev_name(dev), priv);
	if (ret) {
		dev_err(dev, "Failed to request Mailbox IRQ %d: %d\n", irq, ret);
		goto err_dma_mem;
	}

	dev_info(dev, "Mailbox IRQ %d registered\n", irq);

	/* 使能 Linux 侧 mailbox 中断通道，并清理历史 pending */
	writel(1, priv->mailbox_base + MAILBOX_REG_LINUX_ACK);
	writel(1, priv->mailbox_base + MAILBOX_REG_LINUX_IE);

	/* 分配并注册 remoteproc 实例 (使用 managed API) */
	dev_info(dev, "Calling devm_rproc_alloc...\n");
	priv->rproc = devm_rproc_alloc(dev, "quard-star-rproc",
				       &quard_star_rproc_ops, NULL,
				       sizeof(*priv));
	if (!priv->rproc) {
		dev_err(dev, "Failed to allocate remoteproc instance\n");
		ret = -ENOMEM;
		goto err_dma_mem;
	}
	dev_info(dev, "devm_rproc_alloc succeeded\n");

	priv->rproc->priv = priv;
	priv->rproc->has_iommu = false; /* 无需 IOMMU */
	priv->rproc->auto_boot = false; /* "Attach-Only" 模式，不自动启动 */

	/* "Attach-Only" 模式：FreeRTOS 已运行，走 detached -> attach 路径 */
	priv->rproc->state = RPROC_DETACHED;

	/* 注册 remoteproc 设备 */
	dev_info(dev, "Calling devm_rproc_add...\n");
	ret = devm_rproc_add(dev, priv->rproc);
	if (ret) {
		dev_err(dev, "Failed to register remoteproc: %d\n", ret);
		printk(KERN_ERR "quard_star_rproc: devm_rproc_add failed: %d\n", ret);
		goto err_dma_mem;
	}

	dev_info(dev, "Remoteproc device registered: /sys/class/remoteproc/remoteproc0/\n");
	dev_info(dev, "Remoteproc driver initialized successfully\n");
	printk(KERN_INFO "quard_star_rproc: Driver init complete\n");

	return 0;

err_dma_mem:
	of_reserved_mem_device_release(dev);
	return ret;
}

/**
 * quard_star_rproc_remove() - 平台设备remove
 */
static int quard_star_rproc_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;

	dev_info(dev, "Removing Quard Star remoteproc driver\n");

	/* 清理 DMA 内存池 */
	of_reserved_mem_device_release(dev);

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
