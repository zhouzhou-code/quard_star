# Quard Star Linux 驱动开发框架

> 本目录全部是**树外（out-of-tree）内核模块**：独立于内核源码树编译，由 `build.sh build_driver`
> 用 `-C $(KERNELDIR) M=$(模块目录) modules` 编出 `.ko`，再注入 rootfs 的 `/driver/`。
> 每个子目录(remoteproc/rpmsg_test/mailbox_test/amp_ipc)是一个独立模块，含自己的 Makefile。

## 📁 目录结构

```
linux_driver/                 # Linux 驱动源码（out-of-tree）
├── amp_ipc/                  # AMP IPC 通讯驱动
│   ├── src/                  #    源码文件（驱动 + 测试 app）
│   ├── build/                #    编译产物（.ko, .o 等）
│   ├── Makefile              #    编译脚本
│   └── README.md             #    驱动说明文档
├── mailbox_test/             # Mailbox 中断测试驱动
│   ├── src/                  #    源码文件
│   ├── build/                #    编译产物
│   ├── Makefile
│   └── README.md
└── quard_mailbox/            # Quard Mailbox 驱动（待添加）
    ├── src/
    ├── build/
    ├── Makefile
    └── README.md

busybox_root_script/          # BusyBox 相关配置
├── etc/init.d/rcS            #    启动脚本
└── usr/bin/

sysroot/                      # 🎯 最终完整的 rootfs（应用层开发用）
├── bin/, sbin/               #    来自 busybox
├── etc/                      #    来自 busybox_root_script
└── driver/                   #    所有 .ko 驱动模块
    ├── amp_ipc_poll.ko
    └── mailbox_test.ko

output/
├── linux_driver/             # 驱动编译汇总输出
│   ├── amp_ipc_poll.ko
│   └── mailbox_test.ko
└── rootfs/
    ├── rootfs.img            # QEMU 使用的镜像文件
    └── rootfs/               # 临时组装目录
```

## 📦 驱动目录结构说明

每个驱动目录都是独立的，包含：
```
your_driver/
├── src/                      # 源码目录
│   ├── your_driver.c         #    驱动源码
│   └── test_app.c            #    应用层测试程序（可选）
├── build/                    # 编译产物目录
│   ├── your_driver.ko        #    最终的驱动模块
│   └── *.o, *.mod.o          #    中间编译产物
├── Makefile                  # 编译脚本
└── README.md                 # 驱动说明文档
```

**设计理念**：
- `src/` - 包含驱动源码和测试程序，便于管理
- `build/` - 所有编译产物统一存放，保持目录整洁
- `Makefile` - 支持独立编译和 `build.sh` 集成编译

## 🔨 编译使用

### 编译所有驱动
```bash
./build.sh driver
```

### 编译单个驱动
```bash
cd linux_driver/amp_ipc
make
```

### 查看编译产物
```bash
ls linux_driver/amp_ipc/build/*.ko
```

### 完整编译流程（包括驱动和 rootfs）
```bash
./build.sh all
```

### 清理编译产物
```bash
# 清理单个驱动
cd linux_driver/amp_ipc
make clean

# 清理所有驱动
./build.sh driver clean
```

## 📦 RootFS 生成流程

```
┌─────────────────────────┐
│  busybox/               │ 基础命令
├─────────────────────────┤
│  busybox_root_script/   │ 启动脚本
├─────────────────────────┤
│  linux_driver/*.ko      │ 驱动模块
└─────────────────────────┘
           ↓
┌─────────────────────────┐
│  output/rootfs/rootfs/  │ 临时组装
└─────────────────────────┘
           ↓
┌─────────────────────────┐     ┌─────────────────┐
│  rootfs.img             │ ←→  │  sysroot/       │
│  (QEMU 用)              │     │  (应用层开发用)  │
└─────────────────────────┘     └─────────────────┘
```

### 驱动集成
1. 编译输出：`output/linux_driver/*.ko`
2. rootfs 生成时自动拷贝到：`sysroot/driver/`
3. 系统启动后可手动加载：
   ```bash
   insmod /driver/amp_ipc_poll.ko
   insmod /driver/mailbox_test.ko
   ```

### 应用层开发
应用层开发者可以直接使用 `sysroot/` 作为根文件系统环境：
```bash
# 查看可用的驱动模块
ls sysroot/driver/

# 查看启动脚本
cat sysroot/etc/init.d/rcS
```

## 🚀 添加新驱动

### 1. 创建驱动目录
```bash
mkdir -p linux_driver/your_driver/src
```

### 2. 编写 Makefile
```makefile
KERNELDIR ?= $(SHELL_FOLDER)/linux
CROSS_COMPILE ?= /opt/gcc15-riscv64-unknown-linux-gnu/bin/riscv64-unknown-linux-gnu-

obj-m := your_module.o
your_module-y := src/your_source.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) clean
```

### 3. 编译测试
```bash
./build.sh driver
```

## 📝 设计原则

1. **源码与编译分离**：源码在 `linux_driver/`，编译产物在 `output/linux_driver/`
2. **独立 Makefile**：每个驱动有独立的 Makefile，支持单独编译
3. **自动集成**：编译的 .ko 自动集成到 rootfs
4. **清晰的目录职责**：
   - `linux_driver/` - 驱动源码
   - `busybox_root_script/` - BusyBox 配置
   - `sysroot/` - 最终完整 rootfs（应用层用）
   - `output/linux_driver/` - 编译输出
   - `output/rootfs/rootfs.img` - QEMU 镜像

## 🔧 故障排查

### 驱动编译失败
- 确认内核已编译：`./build.sh kernel`
- 检查内核版本：`./build.sh kernel rebuild`

### .ko 未集成到 rootfs
- 检查编译输出：`ls output/linux_driver/*.ko`
- 检查 sysroot：`ls sysroot/driver/*.ko`
- 重新生成 rootfs：`./build.sh rootfs`

### 清理所有产物
```bash
./build.sh clean
```

