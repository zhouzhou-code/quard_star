# RPMsg 双向通信测试

## 📋 概述

这个驱动用于测试 Linux (Hart 1-7) 与 FreeRTOS (Hart 0) 之间的 RPMsg 双向通信。

## 🔧 编译驱动

```bash
cd /home/zms/quard_star/linux_driver/rpmsg_test
make clean && make
```

## 🚀 使用方法

### 1. 在 QEMU Linux 中加载驱动

```bash
# 确保已经加载了 remoteproc 驱动
insmod /home/zms/quard_star/linux_driver/remoteproc/quard_star_rproc.ko
echo start > /sys/class/remoteproc/remoteproc0/state

# 加载 rpmsg_test 驱动
insmod /home/zms/quard_star/linux_driver/rpmsg_test/rpmsg_test.ko
```

### 2. 检查驱动是否成功 probe

```bash
dmesg | tail -20 | grep rpmsg_test
```

应该看到类似输出：
```
rpmsg_test: probing new channel: freertos-test (src=0x30)
rpmsg_test: endpoint created successfully
```

### 3. 发送消息到 FreeRTOS

```bash
echo "Hello from Linux!" > /sys/kernel/debug/rpmsg_test/send
```

### 4. 接收来自 FreeRTOS 的消息

```bash
cat /sys/kernel/debug/rpmsg_test/received
```

### 5. 查看 dmesg 日志

```bash
dmesg | grep rpmsg_test
```

## 📊 预期结果

### Linux 侧 dmesg 输出：
```
rpmsg_test: sending message: Hello from Linux!
rpmsg_test: message sent successfully
rpmsg_test: received message: Hello from FreeRTOS! (len=19, src=0x30)
```

### FreeRTOS 侧 UART 输出：
```
[RPMsg] Received: Hello from Linux!
[RPMsg] Sent: Hello from FreeRTOS!
```

## 🧪 自动化测试

使用提供的测试脚本：

```bash
sh /home/zms/quard_star/linux_driver/rpmsg_test/test_rpmsg.sh
```

## 🔍 故障排查

### 问题 1: 驱动加载失败
```bash
# 检查 debugfs 是否挂载
mount | grep debugfs
# 如果没有，挂载它
mount -t debugfs none /sys/kernel/debug
```

### 问题 2: 没有创建 /sys/kernel/debug/rpmsg_test/
```bash
# 检查驱动是否 probe 成功
dmesg | grep rpmsg_test
# 检查 remoteproc 是否 attach
cat /sys/class/remoteproc/remoteproc0/state
```

### 问题 3: 发送消息失败
```bash
# 检查 FreeRTOS 是否还在运行
# 查看 FreeRTOS 的 UART 输出
# 检查 dmesg 是否有错误
dmesg | tail -50
```

## 📝 卸载驱动

```bash
rmmod rpmsg_test
```

## 🎯 测试流程

1. ✅ Linux remoteproc attach 到 FreeRTOS
2. ✅ Linux virtio_rpmsg_bus 创建 channel: `creating channel freertos-test`
3. ✅ FreeRTOS 发送 NS CREATE 消息 (flags=0)
4. ✅ Linux rpmsg_test 驱动 probe 成功
5. ✅ Linux 发送 "Hello from Linux!" 到 FreeRTOS
6. ✅ FreeRTOS 接收并打印消息
7. ✅ FreeRTOS 发送 "Hello from FreeRTOS!" 回复
8. ✅ Linux 接收并打印消息

## 🏆 成功标志

- Linux dmesg 显示 "creating channel freertos-test"（不是 destroying）
- Linux dmesg 显示 "endpoint created successfully"
- Linux 能够发送消息到 FreeRTOS
- FreeRTOS 能够接收并打印消息
- FreeRTOS 能够发送回复消息
- Linux 能够接收并打印回复消息
