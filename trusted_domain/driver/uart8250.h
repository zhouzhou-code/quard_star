/*
 * UART 16550 驱动 - 移植自 OpenSBI
 *
 * Copyright (c) 2025 Quard Star Project
 *
 * 本文件从 OpenSBI 移植标准 16550 UART 驱动
 * 参考：opensbi-1.2/lib/utils/serial/uart8250.c
 *
 * 资料参考：
 * - 16550 UART Datasheet: https://byterunner.com/16550.html
 * - QEMU UART 实现: qemu-8.0.2/hw/char/serial.c
 */

#ifndef UART8250_H
#define UART8250_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * 16550 UART 寄存器偏移定义
 * ============================================================================ */

#define UART_RBR_OFFSET     0   /* In:  Receive Buffer Register */
#define UART_THR_OFFSET     0   /* Out: Transmitter Holding Register */
#define UART_DLL_OFFSET     0   /* Out: Divisor Latch Low */
#define UART_IER_OFFSET     1   /* I/O: Interrupt Enable Register */
#define UART_DLM_OFFSET     1   /* Out: Divisor Latch High */
#define UART_FCR_OFFSET     2   /* Out: FIFO Control Register */
#define UART_IIR_OFFSET     2   /* I/O: Interrupt Identification Register */
#define UART_LCR_OFFSET     3   /* Out: Line Control Register */
#define UART_MCR_OFFSET     4   /* Out: Modem Control Register */
#define UART_LSR_OFFSET     5   /* In:  Line Status Register */
#define UART_MSR_OFFSET     6   /* In:  Modem Status Register */
#define UART_SCR_OFFSET     7   /* I/O: Scratch Register */

/* ============================================================================
 * Line Status Register (LSR) 位定义
 * ============================================================================ */

#define UART_LSR_FIFOE      0x80    /* FIFO error */
#define UART_LSR_TEMT       0x40    /* Transmitter empty */
#define UART_LSR_THRE       0x20    /* Transmit-hold-register empty */
#define UART_LSR_BI         0x10    /* Break interrupt indicator */
#define UART_LSR_FE         0x08    /* Frame error indicator */
#define UART_LSR_PE         0x04    /* Parity error indicator */
#define UART_LSR_OE         0x02    /* Overrun error indicator */
#define UART_LSR_DR         0x01    /* Receiver data ready */
#define UART_LSR_BRK_ERROR_BITS 0x1E  /* BI, FE, PE, OE bits */

/* ============================================================================
 * Line Control Register (LCR) 位定义
 * ============================================================================ */

#define UART_LCR_DLAB       0x80    /* Divisor Latch Access Bit */
#define UART_LCR_SBC        0x40    /* Set Break Control */
#define UART_LCR_SPAR       0x20    /* Stick Parity */
#define UART_LCR_EPAR       0x10    /* Even Parity Select */
#define UART_LCR_PARITY     0x08    /* Parity Enable */
#define UART_LCR_STOP       0x04    /* Stop bits: 0=1 bit, 1=2 bits */
#define UART_LCR_WLEN8      0x03    /* 8 bits data */
#define UART_LCR_WLEN7      0x02    /* 7 bits data */
#define UART_LCR_WLEN6      0x01    /* 6 bits data */
#define UART_LCR_WLEN5      0x00    /* 5 bits data */

/* ============================================================================
 * FIFO Control Register (FCR) 位定义
 * ============================================================================ */

#define UART_FCR_ENABLE_FIFO    0x01    /* Enable FIFO */
#define UART_FCR_CLEAR_RCVR     0x02    /* Clear receiver FIFO */
#define UART_FCR_CLEAR_XMIT     0x04    /* Clear transmitter FIFO */

/* ============================================================================
 * Interrupt Enable Register (IER) 位定义
 * ============================================================================ */

#define UART_IER_MSI        0x08    /* Enable Modem status interrupt */
#define UART_IER_RLSI       0x04    /* Enable receiver line status interrupt */
#define UART_IER_THRI       0x02    /* Enable Transmitter holding register interrupt */
#define UART_IER_RDI        0x01    /* Enable receiver data interrupt */

/* ============================================================================
 * API 函数声明
 * ============================================================================ */

/**
 * uart8250_init - 初始化 16550 UART
 * @base: UART 寄存器基地址（物理地址）
 * @in_freq: UART 时钟频率（Hz），QEMU 中通常为 1843200 或系统时钟
 * @baudrate: 目标波特率（如 115200）
 *
 * 返回：0 成功，负数失败
 *
 * 配置：8N1（8位数据，无校验，1位停止位）
 */
int uart8250_init(unsigned long base, unsigned int in_freq, unsigned int baudrate);

/**
 * uart8250_putc - 发送一个字符
 * @ch: 要发送的字符
 *
 * 此函数会阻塞直到发送完成
 */
void uart8250_putc(char ch);

/**
 * uart8250_getc - 接收一个字符
 *
 * 返回：接收到的字符，如果没有数据则返回 -1
 */
int uart8250_getc(void);

/**
 * uart8250_puts - 发送字符串
 * @str: 以 null 结尾的字符串
 */
void uart8250_puts(const char *str);

/**
 * uart8250_try_getc - 非阻塞接收字符
 *
 * 返回：true 如果有字符可读，false 否则
 */
bool uart8250_try_getc(char *ch);

#ifdef __cplusplus
}
#endif

#endif /* UART8250_H */
