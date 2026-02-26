# Quard Mailbox Test Driver

## 简介
Mailbox 中断测试驱动，用于验证硬件中断链路是否正常工作。

## 功能
- 实现 Mailbox 平台驱动
- 注册中断处理函数
- 中断发生时打印日志

## 设备树绑定
```dts
mailbox: mailbox@10004000 {
    compatible = "quard-star,mailbox";
    reg = <0x0 0x10004000 0x0 0x1000>;
    interrupts = <50>;
    interrupt-parent = <&plic>;
};
```

## 测试方法
```bash
# 加载驱动
insmod mailbox_test.ko

# 触发中断 (FreeRTOS 端或手动触发)
devmem 0x10004000 32 1

# 查看日志
dmesg | tail
```

## 编译
```bash
# 独立编译
make

# 通过 build.sh 编译
./build.sh driver
```

## 源码说明
- `src/mailbox_test.c` - 驱动源码
- `build/mailbox_test.ko` - 编译输出
