/*
 * Quard Star Mailbox Test Driver
 *
 * 这是一个极简的中断测试驱动，用于验证 Mailbox IRQ 50 是否正确工作。
 *
 * 编译：
 *   make ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- -C ~/linux M=$(pwd) modules
 *
 * 使用：
 *   insmod mailbox_test.ko
 *   # 触发中断：devmem 0x10004000 32 1
 *   # 查看日志：dmesg | tail
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/io.h>

/* QS-Mailbox v2: to-Linux bank channel0（本驱动是接收方） */
#define QSMB_TL_STAT         0x00   /* doorbell 状态 (RO) */
#define QSMB_TL_SET          0x04   /* 置位 (W1S)，测试时手写它模拟对端 kick */
#define QSMB_TL_CLEAR        0x08   /* 清位 (W1C) */
#define QSMB_TL_MASK_CLEAR   0x14   /* 清屏蔽=使能该位中断 (W1C) */
#define QSMB_DBELL_BIT       0x1    /* channel0 bit0 */

static void __iomem *mailbox_base;
static int mailbox_irq;
static int irq_count = 0;

/*
 * 中断服务程序 (ISR)
 *
 * 当 Mailbox 拉高 IRQ 线时，PLIC 会调用这个函数。
 */
static irqreturn_t mailbox_isr(int irq, void *dev_id)
{
    uint32_t stat;

    /* 读取 doorbell 状态寄存器 */
    stat = readl(mailbox_base + QSMB_TL_STAT);

    /* 清除该 doorbell 位 (W1C 语义) */
    writel(QSMB_DBELL_BIT, mailbox_base + QSMB_TL_CLEAR);

    irq_count++;

    pr_info("[MAILBOX TEST] IRQ received! count=%d, stat=0x%x\n", irq_count, stat);

    return IRQ_HANDLED;
}

/*
 * 平台设备 Probe 函数
 */
static int mailbox_probe(struct platform_device *pdev)
{
    struct resource *res;
    int ret;
    uint32_t reg_val;

    dev_info(&pdev->dev, "Quard Star Mailbox Test Driver probing...\n");

    /* 1. 获取并映射 MMIO 资源 */
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (!res) {
        dev_err(&pdev->dev, "Failed to get MMIO resource\n");
        return -ENODEV;
    }

    mailbox_base = devm_ioremap_resource(&pdev->dev, res);
    if (IS_ERR(mailbox_base)) {
        dev_err(&pdev->dev, "Failed to map MMIO\n");
        return PTR_ERR(mailbox_base);
    }

    dev_info(&pdev->dev, "MMIO mapped: 0x%pa - 0x%pa\n", &res->start, &res->end);

    /* 2. 读取 REVISION 寄存器验证设备 (QS-Mailbox v2 在 0xF0，应为 0x0200) */
    reg_val = readl(mailbox_base + 0xF0);
    dev_info(&pdev->dev, "Mailbox REVISION: 0x%x\n", reg_val);

    if (reg_val != 0x0200) {
        dev_warn(&pdev->dev, "Unexpected revision! Expected 0x0200 (v2)\n");
    }

    /* 3. 获取 IRQ 编号 */
    mailbox_irq = platform_get_irq(pdev, 0);
    if (mailbox_irq < 0) {
        dev_err(&pdev->dev, "Failed to get IRQ resource\n");
        return mailbox_irq;
    }

    dev_info(&pdev->dev, "IRQ number: %d\n", mailbox_irq);

    /* 4. 注册中断处理函数 */
    ret = devm_request_irq(&pdev->dev, mailbox_irq, mailbox_isr,
                           IRQF_SHARED | IRQF_TRIGGER_RISING,
                           "mailbox_test", pdev);
    if (ret) {
        dev_err(&pdev->dev, "Failed to request IRQ %d (error: %d)\n", mailbox_irq, ret);
        return ret;
    }

    dev_info(&pdev->dev, "IRQ %d registered successfully\n", mailbox_irq);

    /* 5. unmask to-Linux ch0 bit0（清屏蔽位 = 使能该位中断）*/
    writel(QSMB_DBELL_BIT, mailbox_base + QSMB_TL_MASK_CLEAR);
    dev_info(&pdev->dev, "Mailbox ch0 bit0 unmasked (中断已使能)\n");

    dev_info(&pdev->dev, "Probe complete. Ready to receive interrupts!\n");
    dev_info(&pdev->dev, "Test with: devmem 0x%llx 32 1\n",
             (unsigned long long)res->start + QSMB_TL_SET);

    return 0;
}
 
/*
 * 平台设备 Remove 函数
 */
static int mailbox_remove(struct platform_device *pdev)
{
    dev_info(&pdev->dev, "Removing driver... Total IRQs received: %d\n", irq_count);

    /* 禁用中断：重新屏蔽 to-Linux ch0 bit0（置屏蔽位 W1S，0x10）*/
    if (mailbox_base) {
        writel(QSMB_DBELL_BIT, mailbox_base + 0x10);
    }

    return 0;
}

/*
 * 设备树匹配表
 */
static const struct of_device_id mailbox_of_match[] = {
    { .compatible = "quard-star,mailbox", },
    { /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, mailbox_of_match);

/*
 * 平台驱动结构体
 */
static struct platform_driver mailbox_driver = {
    .probe = mailbox_probe,
    .remove = mailbox_remove,
    .driver = {
        .name = "mailbox_test",
        .of_match_table = mailbox_of_match,
    },
};

module_platform_driver(mailbox_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Quard Star Project");
MODULE_DESCRIPTION("Quard Star Mailbox Interrupt Test Driver");
MODULE_VERSION("1.0");
