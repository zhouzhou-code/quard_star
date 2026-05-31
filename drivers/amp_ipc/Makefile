# Makefile for AMP IPC Poll Driver

# Default Kernel Directory (Can be overridden by environment variable)
# Example: make KERNELDIR=/path/to/linux-kernel CROSS_COMPILE=riscv64-unknown-linux-gnu-
KERNELDIR := /home/zms/quard_star/linux

# Cross Compiler Prefix (Can be overridden)
CROSS_COMPILE := /opt/gcc15-riscv64-unknown-linux-gnu/bin/riscv64-unknown-linux-gnu-

# Module Name
obj-m := amp_ipc_poll.o
amp_ipc_poll-y := src/amp_ipc_poll.o


# Build Rules
all:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) clean

# Installation Rule (Optional)
install:
	$(MAKE) -C $(KERNELDIR) M=$(CURDIR) ARCH=riscv CROSS_COMPILE=$(CROSS_COMPILE) modules_install

.PHONY: all clean install
