# Makefile for RPMsg Test Driver

obj-m += rpmsg_test.o

# Kernel build directory
KDIR := /home/zms/quard_star/linux

# Cross compilation settings
ARCH := riscv
CROSS_COMPILE := /opt/gcc15-riscv64-unknown-linux-gnu/bin/riscv64-unknown-linux-gnu-

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
