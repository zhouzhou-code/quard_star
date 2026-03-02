/*
 * Simple RPMsg Test Driver
 *
 * 用于测试与 FreeRTOS 的 RPMsg 通信
 *
 * 使用方法：
 * 1. insmod rpmsg_test.ko
 * 2. echo "test" > /sys/kernel/debug/rpmsg_test/send
 * 3. cat /sys/kernel/debug/rpmsg_test/received
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/rpmsg.h>
#include <linux/slab.h>
#include <linux/debugfs.h>

#define DRIVER_NAME	"rpmsg_test"
#define DRIVER_VERSION	"1.0"
#define DRIVER_AUTHOR	"Quard Star Project"
#define DRIVER_DESC	"Simple RPMsg Test Driver"

static struct rpmsg_device *g_rpdev;
static struct rpmsg_endpoint *g_ept;
static struct dentry *debugfs_dir;
static struct dentry *send_file;
static struct dentry *received_file;

static char received_msg[256];
static size_t received_len;

/* 接收消息回调 */
static int rpmsg_test_cb(struct rpmsg_device *rpdev, void *data, int len, void *priv, u32 src)
{
	pr_info("rpmsg_test: received message: %.*s (len=%d, src=0x%x)\n",
		len, (char *)data, len, src);

	/* 保存接收到的消息 */
	received_len = min_t(size_t, len, sizeof(received_msg) - 1);
	memcpy(received_msg, data, received_len);
	received_msg[received_len] = '\0';

	return 0;
}

/* RPMsg 驱动 probe */
static int rpmsg_test_probe(struct rpmsg_device *rpdev)
{
	struct rpmsg_channel_info chinfo;

	dev_info(&rpdev->dev, "rpmsg_test: probing new channel: %s (src=0x%x)\n",
		 rpdev->id.name, rpdev->src);

	/* 保存 rpdev 指针 */
	g_rpdev = rpdev;

	/* 创建 endpoint */
	strncpy(chinfo.name, rpdev->id.name, RPMSG_NAME_SIZE);
	chinfo.src = RPMSG_ADDR_ANY;
	chinfo.dst = RPMSG_ADDR_ANY;

	g_ept = rpmsg_create_ept(rpdev, rpmsg_test_cb, NULL, chinfo);
	if (!g_ept) {
		dev_err(&rpdev->dev, "rpmsg_test: failed to create endpoint\n");
		return -ENOMEM;
	}

	dev_info(&rpdev->dev, "rpmsg_test: endpoint created successfully\n");

	return 0;
}

/* RPMsg 驱动 remove */
static void rpmsg_test_remove(struct rpmsg_device *rpdev)
{
	dev_info(&rpdev->dev, "rpmsg_test: removing channel\n");
	if (g_ept) {
		rpmsg_destroy_ept(g_ept);
		g_ept = NULL;
	}
}

/* RPMsg 驱动 ID 表 - 匹配 FreeRTOS 创建的 channel */
static struct rpmsg_device_id rpmsg_test_id_table[] = {
	{ .name = "freertos-test" },
	{ /* sentinel */ },
};
MODULE_DEVICE_TABLE(rpmsg, rpmsg_test_id_table);

/* RPMsg 驱动结构 */
static struct rpmsg_driver rpmsg_test_driver = {
	.drv.name	= KBUILD_MODNAME,
	.id_table	= rpmsg_test_id_table,
	.probe		= rpmsg_test_probe,
	.callback	= rpmsg_test_cb,
	.remove		= rpmsg_test_remove,
};

/* DebugFS: 发送消息 */
static ssize_t rpmsg_test_send_write(struct file *file, const char __user *buf,
				       size_t count, loff_t *ppos)
{
	char *msg;
	int ret;

	if (!g_ept) {
		pr_err("rpmsg_test: endpoint not ready\n");
		return -ENODEV;
	}

	msg = memdup_user_nul(buf, count);
	if (IS_ERR(msg))
		return PTR_ERR(msg);

	pr_info("rpmsg_test: sending message: %s\n", msg);

	ret = rpmsg_send(g_ept, msg, count);
	kfree(msg);

	if (ret) {
		pr_err("rpmsg_test: failed to send message: %d\n", ret);
		return ret;
	}

	pr_info("rpmsg_test: message sent successfully\n");
	return count;
}

/* DebugFS: 读取接收到的消息 */
static ssize_t rpmsg_test_received_read(struct file *file, char __user *buf,
					 size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, received_msg, received_len);
}

static const struct file_operations send_fops = {
	.write = rpmsg_test_send_write,
};

static const struct file_operations received_fops = {
	.read = rpmsg_test_received_read,
};

/* 模块初始化 */
static int __init rpmsg_test_init(void)
{
	int ret;

	pr_info("rpmsg_test: initializing driver\n");

	/* 注册 RPMsg 驱动 */
	ret = register_rpmsg_driver(&rpmsg_test_driver);
	if (ret) {
		pr_err("rpmsg_test: failed to register driver: %d\n", ret);
		return ret;
	}

	/* 创建 DebugFS 目录 */
	debugfs_dir = debugfs_create_dir(DRIVER_NAME, NULL);
	if (IS_ERR(debugfs_dir)) {
		pr_warn("rpmsg_test: failed to create debugfs directory\n");
		debugfs_dir = NULL;
	} else {
		send_file = debugfs_create_file("send", 0220, debugfs_dir,
						NULL, &send_fops);
		received_file = debugfs_create_file("received", 0444, debugfs_dir,
						    NULL, &received_fops);
	}

	pr_info("rpmsg_test: driver initialized\n");
	return 0;
}

/* 模块退出 */
static void __exit rpmsg_test_exit(void)
{
	pr_info("rpmsg_test: exiting driver\n");

	debugfs_remove_recursive(debugfs_dir);
	unregister_rpmsg_driver(&rpmsg_test_driver);
}

module_init(rpmsg_test_init);
module_exit(rpmsg_test_exit);

MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
