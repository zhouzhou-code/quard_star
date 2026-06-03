# Makefile for RV-Mailbox self-test driver (QEMU virt demo)

PROJECT_ROOT := $(shell cd $(CURDIR)/../../.. && pwd)
KERNELDIR ?= $(PROJECT_ROOT)/source/linux
CROSS_COMPILE ?= $(PROJECT_ROOT)/toolchain/gcc15-riscv64-unknown-linux-gnu/bin/riscv64-unknown-linux-gnu-

BUILD_DIR := $(CURDIR)/build

obj-m := rv_mailbox_selftest.o
rv_mailbox_selftest-y := src/rv_mailbox_selftest.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) modules
	@mkdir -p $(BUILD_DIR)
	@cp -f *.ko $(BUILD_DIR)/ 2>/dev/null || true

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) clean
	rm -rf $(BUILD_DIR)

.PHONY: all clean
