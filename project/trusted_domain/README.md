# FreeRTOS Trusted Domain 目录结构说明

FreeRTOS 可信域运行在 Hart 7，通过自研 **RV-Mailbox** 设备 + 业界标准 **OpenAMP**
(open-amp + libmetal) 与 Linux 应用域做 rpmsg 双向通信。

## 📁 目录结构

```
trusted_domain/
├── 📄 应用层
│   ├── main.c                    # 主入口：起 FreeRTOS，跑 OpenAMP rpmsg 回显任务
│   ├── openamp_rpmsg.c/.h        # OpenAMP rpmsg：remoteproc + 静态资源表 → virtio
│   │                             #   → rpmsg_init_vdev → 端点 freertos-test 回显
│   ├── resource_table.c          # 静态 resource table（vdev/vring，固定共享内存地址）
│   ├── FreeRTOSConfig.h          # FreeRTOS 配置
│   ├── link.lds                  # 链接脚本
│   ├── startup.S                 # 启动汇编
│   └── Makefile                  # 编译脚本（链接 open-amp / libmetal freertos system）
│
├── 📁 driver/                    # 驱动程序
│   ├── uart8250.c                # UART 16550 驱动实现（从 OpenSBI 移植）
│   └── uart8250.h
│
├── 📁 riscv/                     # RISC-V 架构相关
│   ├── riscv_sbi.c/.h            # OpenSBI 调用接口
│   ├── riscv_encoding.h          # RISC-V 指令编码
│   ├── riscv_asm.h               # RISC-V 汇编宏
│   └── sbi_const.h               # SBI 常量定义
│
├── 📁 bsp_include/               # BSP 共享头文件（只放 .h）
│   ├── hwspecs.h                 # 硬件规格（内存地址、RV-Mailbox 寄存器偏移/掩码）
│   └── resource_table.h          # Resource Table 结构定义
│
└── 📁 FreeRTOS-Kernel-v10.4.3/   # FreeRTOS 内核
```

> open-amp / libmetal 上游源码不在本目录，统一放 `source/third_party/`，由 build.sh 链接。

## 📝 重要说明

### 状态
- ✅ **OpenAMP rpmsg 双向通信跑通** —— QEMU(RV-Mailbox) + OpenAMP，自动化测试
  `test_openamp2.py` 双向 BIDIRECTIONAL PASS（7/7）。
- ✅ FreeRTOS 内核、UART 驱动、静态 resource table 均正常。

### 通知机制（RV-Mailbox）
- **FreeRTOS → Linux**：写 RV-Mailbox to-Linux SET（`RVMB_TL_SET=0x04`）触发 Linux 侧 IRQ。
- **Linux → FreeRTOS**：当前由 `openamp_rpmsg_poll()` 轮询 to-RTOS 寄存器并 CLEAR
  （尚未接 PLIC 中断，是后续可优化点）。

### UART 分配
- **UART0** (0x10000000) - OpenSBI/U-Boot/Linux
- **UART1** (0x10001000) - 未使用
- **UART2** (0x10002000) - **FreeRTOS (Hart 7) 专用** ✅

### 调试方式
```bash
# 终端 1: 启动 QEMU（显示 OpenSBI/U-Boot/Linux）
./run.sh

# 终端 2: 连接 FreeRTOS 输出
telnet localhost 4446
```

---
更新时间: 2026-06-08
