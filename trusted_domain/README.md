# FreeRTOS Trusted Domain 目录结构说明

## 📁 目录结构

```
trusted_domain/
├── 📄 应用层
│   ├── main.c                    # 主入口，UART 16550 驱动测试
│   ├── FreeRTOSConfig.h          # FreeRTOS 配置
│   ├── link.lds                  # 链接脚本
│   ├── startup.S                 # 启动汇编
│   └── Makefile                  # 编译脚本
│
├── 📁 driver/                    # 驱动程序
│   ├── uart8250.c                # UART 16550 驱动实现（从 OpenSBI 移植）
│   └── uart8250.h                # UART 16550 驱动头文件
│
├── 📁 riscv/                     # RISC-V 架构相关
│   ├── riscv_sbi.c               # OpenSBI 调用接口实现
│   ├── riscv_sbi.h               # OpenSBI 调用接口声明
│   ├── riscv_encoding.h          # RISC-V 指令编码
│   ├── riscv_asm.h               # RISC-V 汇编宏
│   └── sbi_const.h              # SBI 常量定义
│
├── 📁 bsp_include/               # BSP 共享头文件（只放 .h）
│   ├── hwspecs.h                 # 硬件规格定义（内存地址、常量）
│   └── resource_table.h          # Resource Table 结构定义
│
├── 📁 openamp_adapter/           # OpenAMP 适配层（暂时禁用）
│   ├── include/                  # 导出头文件
│   ├── libmetal/                 # libmetal FreeRTOS 移植（依赖复杂）
│   └── openamp/                  # open-amp FreeRTOS 移植（依赖复杂）
│
└── 📁 FreeRTOS-Kernel-v10.4.3/   # FreeRTOS 内核
```

## 📝 重要说明

### 编译状态
- ✅ **基本功能编译通过** - UART 驱动、Resource Table、FreeRTOS 内核
- ⏸️ **OpenAMP 适配层暂时禁用** - libmetal 依赖太复杂，需要后续重构

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

### 下一步工作
1. 🔧 **修复 OpenAMP 适配层** - 重构 libmetal 依赖
2. 🧪 **测试 Resource Table** - 验证共享内存固定地址可读
3. 📡 **启用 RPMsg 通信** - 在 UART 测试通过后

### 已知问题
- ⚠️ OpenAMP 适配层依赖大量 libmetal 头文件，需要重构

---
更新时间: 2026-03-01
