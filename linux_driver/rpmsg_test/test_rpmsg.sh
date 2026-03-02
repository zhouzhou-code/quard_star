#!/bin/sh
# RPMsg 双向通信测试脚本

echo "==============================================="
echo "RPMsg 双向通信测试"
echo "==============================================="
echo ""

echo "[步骤 1] 加载 rpmsg_test 驱动..."
if [ -f /home/zms/quard_star/linux_driver/rpmsg_test/rpmsg_test.ko ]; then
    insmod /home/zms/quard_star/linux_driver/rpmsg_test/rpmsg_test.ko
    echo "✅ rpmsg_test 驱动加载成功"
else
    echo "❌ 找不到 rpmsg_test.ko"
    exit 1
fi
echo ""

sleep 1

echo "[步骤 2] 检查驱动是否 probe 到 freertos-test channel..."
dmesg | tail -20 | grep -i "rpmsg_test.*probing"
echo ""

echo "[步骤 3] 发送测试消息到 FreeRTOS..."
echo "Hello from Linux!" > /sys/kernel/debug/rpmsg_test/send
echo "✅ 消息已发送"
echo ""

sleep 1

echo "[步骤 4] 检查 FreeRTOS 的接收日志..."
echo "（查看 FreeRTOS 的 UART 输出）"
echo ""

echo "[步骤 5] 从 FreeRTOS 接收消息..."
echo "（FreeRTOS 应该会发送回复消息）"
echo ""

echo "[步骤 6] 读取接收到的消息..."
if [ -f /sys/kernel/debug/rpmsg_test/received ]; then
    RECEIVED=$(cat /sys/kernel/debug/rpmsg_test/received)
    if [ -n "$RECEIVED" ]; then
        echo "✅ 收到消息: $RECEIVED"
    else
        echo "⚠️  还没有收到消息"
    fi
else
    echo "❌ /sys/kernel/debug/rpmsg_test/received 不存在"
fi
echo ""

echo "[步骤 7] 查看 dmesg 日志..."
dmesg | tail -30 | grep rpmsg
echo ""

echo "==============================================="
echo "测试完成"
echo "==============================================="
