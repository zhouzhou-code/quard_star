# Makefile for Mailbox Test Driver

# Kernel Directory (build.sh 命令行覆盖；独立 make 用相对回退)
PROJECT_ROOT := $(shell cd $(CURDIR)/../../.. && pwd)
KERNELDIR ?= $(PROJECT_ROOT)/source/linux
CROSS_COMPILE ?= $(PROJECT_ROOT)/toolchain/gcc15-riscv64-unknown-linux-gnu/bin/riscv64-unknown-linux-gnu-

# Build directory
BUILD_DIR := $(CURDIR)/build

# Module objects (source files are relative to driver dir, not build dir)
obj-m := mailbox_test.o
mailbox_test-y := src/mailbox_test.o

# Build Rules
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) modules
	@mkdir -p $(BUILD_DIR)
	@cp -f *.ko $(BUILD_DIR)/ 2>/dev/null || true

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) clean
	rm -rf $(BUILD_DIR)

.PHONY: all clean
