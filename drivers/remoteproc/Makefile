# Quard Star Remoteproc Driver Makefile
#
# Copyright (c) 2025 Quard Star Project

# 模块名称
obj-m := quard_star_rproc.o

# 内核构建系统
ifneq ($(KERNELRELEASE),)

else

# 获取项目根目录的绝对路径（使用 CURDIR，避免 make -C 时 PWD 不可靠）
PROJECT_ROOT := $(shell cd $(CURDIR)/../.. && pwd)

# Linux 内核目录
KERNELDIR := $(PROJECT_ROOT)/linux

# 交叉编译工具链
CROSS_COMPILE := /opt/gcc15-riscv64-unknown-linux-gnu/bin/riscv64-unknown-linux-gnu-

all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) \
		ARCH=riscv \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) \
		ARCH=riscv \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		clean

help:
	@echo "make          - 编译驱动模块 (使用项目内的 Linux 内核)"
	@echo "make clean    - 清理编译产物"

.PHONY: all clean help
endif
