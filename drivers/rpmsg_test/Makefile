# Makefile for RPMsg Test Driver

obj-m += rpmsg_test.o

# 内核与工具链目录：build.sh 用命令行 KERNELDIR=/CROSS_COMPILE= 覆盖；
# 独立 make 时用相对工程根的回退路径（?= 保证可被覆盖）
PROJECT_ROOT := $(shell cd $(CURDIR)/../.. && pwd)
KERNELDIR ?= $(PROJECT_ROOT)/source/linux

# Cross compilation settings
ARCH := riscv
CROSS_COMPILE ?= $(PROJECT_ROOT)/toolchain/gcc15-riscv64-unknown-linux-gnu/bin/riscv64-unknown-linux-gnu-

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
