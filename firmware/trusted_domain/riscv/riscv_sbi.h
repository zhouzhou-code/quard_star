/*
 * 轻量级 RISC-V SBI 接口层 (适配 FreeRTOS 裸机环境)
 * 专为现代 OpenSBI (SBI v0.2+) 设计，去除老旧 v0.1 兼容层。
 */

#ifndef _ASM_RISCV_SBI_H
#define _ASM_RISCV_SBI_H

#include <stdint.h>

/* SBI Extension IDs (v0.2+) */
#define SBI_EXT_BASE    0x10
#define SBI_EXT_TIME    0x54494D45
#define SBI_EXT_IPI     0x735049
#define SBI_EXT_RFENCE  0x52464E43
#define SBI_EXT_HSM     0x48534D

/* Function IDs */
#define SBI_EXT_TIME_SET_TIMER 0
#define SBI_EXT_IPI_SEND_IPI   0

/* 老版本 v0.1 遗留的 Console 接口 (OpenSBI 仍支持，调试极其好用) */
#define SBI_EXT_0_1_CONSOLE_PUTCHAR 0x1
#define SBI_EXT_0_1_CONSOLE_GETCHAR 0x2
#define SBI_EXT_0_1_SHUTDOWN        0x8

/* SBI 返回值结构体 */
struct sbiret {
    long error;
    long value;
};

struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
                        unsigned long arg1, unsigned long arg2,
                        unsigned long arg3, unsigned long arg4,
                        unsigned long arg5);

/* 串口打印 (直接调用底层的 UART，早期 Printf 必备) */
void sbi_console_putchar(int ch);
int sbi_console_getchar(void);

/* 定时器 (FreeRTOS 的 SysTick 来源) */
void sbi_set_timer(uint64_t stime_value);

/* 核间中断 IPI (给多核发信号) */
void sbi_send_ipi(const unsigned long *hart_mask);

/* 暴力关机 (测试跑完后退出 QEMU 用) */
void sbi_shutdown(void);

#endif /* _ASM_RISCV_SBI_H */