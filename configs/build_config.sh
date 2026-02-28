#!/bin/bash
# Quard Star Build Configuration
# Copyright (c) 2025 Quard Star Project

# 本文件被 build.sh source，定义所有构建相关的配置常量

# ============================================================================
# 工具链配置
# ============================================================================

# RISC-V 交叉编译工具链
export RISCV64_UNKNOWN_ELF_PREFIX=riscv64-unknown-elf-
export RISCV64_LINUX_GNU_PREFIX=riscv64-linux-gnu-

# 编译器
export CC_RV64=${RISCV64_UNKNOWN_ELF_PREFIX}gcc
export AR_RV64=${RISCV64_UNKNOWN_ELF_PREFIX}ar
export LD_RV64=${RISCV64_UNKNOWN_ELF_PREFIX}ld

export CC_LINUX=${RISCV64_LINUX_GNU_PREFIX}gcc
export AR_LINUX=${RISCV64_LINUX_GNU_PREFIX}ar

# ============================================================================
# 路径配置
# ============================================================================

# 第三方库
export LIBMETAL_DIR=third_party/libmetal
export OPENAMP_DIR=third_party/open-amp

# FreeRTOS 应用
export FREERTOS_APP_DIR=freertos_app
export OPENAMP_ADAPTER_DIR=freertos_app/openamp_adapter

# Linux 内核
export LINUX_DIR=linux

# U-Boot
export UBOOT_DIR=u-boot-2026.01

# BusyBox
export BUSYBOX_DIR=busybox-1.36.1

# 输出目录
export OUTPUT_DIR=output

# ============================================================================
# 编译选项
# ============================================================================

# FreeRTOS/裸机编译选项
export CFLAGS_FREERTOS="-march=rv64imafdcv -mabi=lp64d -mcmodel=medany \
                        -ffunction-sections -fdata-sections \
                        -Wall -Wextra \
                        -I${OPENAMP_ADAPTER_DIR}/include \
                        -Ibsp_include \
                        -DFREERTOS -O2 -g"

# Linux 内核编译选项
export CFLAGS_LINUX="-march=rv64imafdcv -mabi=lp64d \
                    -ffunction-sections -fdata-sections"

# ============================================================================
# 硬件规格（从 hwspecs.h 导出）
# ============================================================================

export SHM_BASE_ADDR=0xbf700000
export RESOURCE_TABLE_OFFSET=0xc000
export VRING_SIZE=256
export MAILBOX_IRQ_TO_LINUX=50
export MAILBOX_IRQ_TO_RTOS=51

# ============================================================================
# QEMU 配置
# ============================================================================

export QEMU_DIR=qemu-8.0.2
export QEMU_BIN=${QEMU_DIR}/build/qemu-system-riscv64

# ============================================================================
# 日志配置
# ============================================================================

# 颜色输出
export RED='\033[0;31m'
export GREEN='\033[0;32m'
export YELLOW='\033[1;33m'
export NC='\033[0m'

# 日志函数
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# ============================================================================
# 构建目标列表
# ============================================================================

# 可用目标：all, freertos, linux, uboot, busybox, libmetal, openamp, clean
