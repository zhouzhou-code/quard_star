#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/string.h>

#define DRIVER_NAME "amp_ipc"

/* 物理共享内存配置 (注意：确保这里与你的设备树 no-map 地址完全一致) */
#define AMP_SHM_BASE_ADDR 0xbf700000 
#define AMP_SHM_SIZE      0x1000     // 改成了 4KB，对于 Mailbox 足够了

/* 共享内存数据结构 */
struct amp_mailbox {
    volatile u32 f2l_flag;  // Offset 0x00
    char payload[256];      // Offset 0x04
} __packed;

/* 全局变量 */
static void __iomem *shm_base;       
static struct amp_mailbox *mailbox;  
static struct timer_list poll_timer; 
static DECLARE_WAIT_QUEUE_HEAD(read_wq); // 优化：直接使用宏静态初始化等待队列
static int data_ready = 0;           
static char kernel_buffer[256];      
static DEFINE_SPINLOCK(buffer_lock); 

/* 定时器回调函数 */
static void amp_poll_timer_callback(struct timer_list *t)
{
    unsigned long flags;

    // 读取标志位
    if (ioread32(&mailbox->f2l_flag) == 1) {
        
        spin_lock_irqsave(&buffer_lock, flags);
        // 拷贝数据到本地缓冲区
        memcpy_fromio(kernel_buffer, (void __iomem *)mailbox->payload, sizeof(mailbox->payload));
        kernel_buffer[sizeof(mailbox->payload) - 1] = '\0';
        data_ready = 1;
        spin_unlock_irqrestore(&buffer_lock, flags);

        // 清除标志位，通知 FreeRTOS 可以发下一条了
        iowrite32(0, &mailbox->f2l_flag);

        // 唤醒用户态进程
        wake_up_interruptible(&read_wq);
        
        pr_info(DRIVER_NAME ": Received message: %s\n", kernel_buffer);
    }

    // 重新设置定时器
    mod_timer(&poll_timer, jiffies + msecs_to_jiffies(10));
}

static int amp_open(struct inode *inode, struct file *file) { return 0; }
static int amp_release(struct inode *inode, struct file *file) { return 0; }

/* 字符设备 read 函数 */
static ssize_t amp_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    unsigned long flags;
    int ret;
    size_t copy_len;
    char tmp_buf[256]; // 临时缓冲区，用于避开 Spinlock 内 copy_to_user

    // 阻塞等待
    if (wait_event_interruptible(read_wq, data_ready != 0))
        return -ERESTARTSYS;

    // --- 加锁：只做最快的内存拷贝 ---
    spin_lock_irqsave(&buffer_lock, flags);
    
    copy_len = strnlen(kernel_buffer, sizeof(kernel_buffer));
    if (count < copy_len) {
        copy_len = count;
    }
    memcpy(tmp_buf, kernel_buffer, copy_len); // 拷贝到局部变量
    data_ready = 0; // 重置标志位
    
    spin_unlock_irqrestore(&buffer_lock, flags);
    // --- 解锁完毕 ---

    // 在锁外安全地拷贝到用户空间！
    ret = copy_to_user(buf, tmp_buf, copy_len);
    if (ret)
        return -EFAULT;

    return copy_len;
}

static const struct file_operations amp_fops = {
    .owner   = THIS_MODULE,
    .open    = amp_open,
    .release = amp_release,
    .read    = amp_read,
};

static struct miscdevice amp_misc_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = "amp_ipc",
    .fops  = &amp_fops,
};

static int __init amp_init(void)
{
    int ret;
    pr_info(DRIVER_NAME ": Initializing AMP IPC Poll Driver\n");

    shm_base = ioremap(AMP_SHM_BASE_ADDR, AMP_SHM_SIZE);
    if (!shm_base) {
        pr_err(DRIVER_NAME ": Failed to ioremap\n");
        return -ENOMEM;
    }
    
    mailbox = (struct amp_mailbox *)shm_base;

    ret = misc_register(&amp_misc_dev);
    if (ret) {
        pr_err(DRIVER_NAME ": Failed to register misc device\n");
        iounmap(shm_base);
        return ret;
    }

    // 初始化时确保标志位为 0
    iowrite32(0, &mailbox->f2l_flag);

    // 启动定时器(去掉了错误的返回值检查)
    timer_setup(&poll_timer, amp_poll_timer_callback, 0);
    mod_timer(&poll_timer, jiffies + msecs_to_jiffies(10));

    pr_info(DRIVER_NAME ": Module loaded. Virt: %px\n", shm_base);
    return 0;
}

static void __exit amp_exit(void)
{
    del_timer_sync(&poll_timer);
    misc_deregister(&amp_misc_dev);
    iounmap(shm_base);
    pr_info(DRIVER_NAME ": Module unloaded\n");
}

module_init(amp_init);
module_exit(amp_exit);

MODULE_LICENSE("GPL");