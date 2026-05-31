/*
 * UART 16550 驱动实现 - 移植自 OpenSBI
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件从 OpenSBI 移植标准 16550 UART 驱动
 * 参考：opensbi-1.2/lib/utils/serial/uart8250.c
 */

#include "uart8250.h"
#include <stdint.h>
#include <stdbool.h>

/* ============================================================================
 * 全局变量
 * ============================================================================ */

static volatile uint8_t *uart8250_base;
static unsigned int uart8250_reg_shift = 0;
static unsigned int uart8250_reg_width = 1;  /* 8位寄存器 */

/* ============================================================================
 * 寄存器访问辅助函数
 * ============================================================================ */

/**
 * 读取 UART 寄存器
 */
static inline uint32_t get_reg(uint32_t num)
{
    uint32_t offset = num << uart8250_reg_shift;

    if (uart8250_reg_width == 1)
        return uart8250_base[offset];
    else if (uart8250_reg_width == 2)
        return *(volatile uint16_t *)(uart8250_base + offset);
    else
        return *(volatile uint32_t *)(uart8250_base + offset);
}

/**
 * 写入 UART 寄存器
 */
static inline void set_reg(uint32_t num, uint32_t val)
{
    uint32_t offset = num << uart8250_reg_shift;

    if (uart8250_reg_width == 1)
        uart8250_base[offset] = (uint8_t)val;
    else if (uart8250_reg_width == 2)
        *(volatile uint16_t *)(uart8250_base + offset) = (uint16_t)val;
    else
        *(volatile uint32_t *)(uart8250_base + offset) = val;
}

/* ============================================================================
 * API 实现
 * ============================================================================ */

/**
 * uart8250_init - 初始化 16550 UART
 */
int uart8250_init(unsigned long base, unsigned int in_freq, unsigned int baudrate)
{
    uint16_t bdiv = 0;

    uart8250_base = (volatile uint8_t *)base;

    /* 计算波特率分频值
     * 公式：bdiv = in_freq / (16 * baudrate)
     */
    if (baudrate) {
        bdiv = (in_freq + 8 * baudrate) / (16 * baudrate);
    }

    /* 禁用所有中断 */
    set_reg(UART_IER_OFFSET, 0x00);

    /* 设置 DLAB (Divisor Latch Access Bit) 以访问波特率分频寄存器 */
    set_reg(UART_LCR_OFFSET, UART_LCR_DLAB);

    if (bdiv) {
        /* 设置分频值低字节 */
        set_reg(UART_DLL_OFFSET, bdiv & 0xff);
        /* 设置分频值高字节 */
        set_reg(UART_DLM_OFFSET, (bdiv >> 8) & 0xff);
    }

    /* 8位数据，无校验，1位停止位 (8N1) */
    set_reg(UART_LCR_OFFSET, UART_LCR_WLEN8);

    /* 启用 FIFO，清空发送和接收 FIFO */
    set_reg(UART_FCR_OFFSET,
            UART_FCR_ENABLE_FIFO |
            UART_FCR_CLEAR_RCVR |
            UART_FCR_CLEAR_XMIT);

    /* 无调制解调器控制 (DTR, RTS) */
    set_reg(UART_MCR_OFFSET, 0x00);

    /* 清除线路状态 */
    (void)get_reg(UART_LSR_OFFSET);

    /* 读取接收缓冲区以清除 */
    (void)get_reg(UART_RBR_OFFSET);

    /* 设置暂存寄存器 */
    set_reg(UART_SCR_OFFSET, 0x00);

    return 0;
}

/**
 * uart8250_putc - 发送一个字符
 */
void uart8250_putc(char ch)
{
    /* 等待发送保持寄存器为空 */
    while ((get_reg(UART_LSR_OFFSET) & UART_LSR_THRE) == 0)
        ;

    /* 写入字符到发送保持寄存器 */
    set_reg(UART_THR_OFFSET, (uint8_t)ch);
}

/**
 * uart8250_getc - 接收一个字符
 */
int uart8250_getc(void)
{
    /* 检查是否有数据可读 */
    if (get_reg(UART_LSR_OFFSET) & UART_LSR_DR)
        return (int)get_reg(UART_RBR_OFFSET);

    return -1;
}

/**
 * uart8250_puts - 发送字符串
 */
void uart8250_puts(const char *str)
{
    while (*str) {
        if (*str == '\n') {
            /* 自动添加回车 */
            uart8250_putc('\r');
        }
        uart8250_putc(*str++);
    }
}

/**
 * uart8250_try_getc - 非阻塞接收字符
 */
bool uart8250_try_getc(char *ch)
{
    int c = uart8250_getc();
    if (c >= 0) {
        *ch = (char)c;
        return true;
    }
    return false;
}
