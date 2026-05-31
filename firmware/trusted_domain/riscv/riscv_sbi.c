#include "riscv_sbi.h"

struct sbiret sbi_ecall(int ext, int fid, unsigned long arg0,
                        unsigned long arg1, unsigned long arg2,
                        unsigned long arg3, unsigned long arg4,
                        unsigned long arg5)
{
    struct sbiret ret;

    register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0);
    register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1);
    register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2);
    register uintptr_t a3 asm ("a3") = (uintptr_t)(arg3);
    register uintptr_t a4 asm ("a4") = (uintptr_t)(arg4);
    register uintptr_t a5 asm ("a5") = (uintptr_t)(arg5);
    register uintptr_t a6 asm ("a6") = (uintptr_t)(fid);
    register uintptr_t a7 asm ("a7") = (uintptr_t)(ext);

    asm volatile ("ecall"
                  : "+r" (a0), "+r" (a1)
                  : "r" (a2), "r" (a3), "r" (a4), "r" (a5), "r" (a6), "r" (a7)
                  : "memory");
                  
    ret.error = a0;
    ret.value = a1;

    return ret;
}

/* * 打印单个字符。
 * 这是裸机/早期 Bring-up 最神圣的接口，你的第一个 "Hello World" 全靠它！
 */
void sbi_console_putchar(int ch)
{
    sbi_ecall(SBI_EXT_0_1_CONSOLE_PUTCHAR, 0, ch, 0, 0, 0, 0, 0);
}

/* 读取单个字符 (非阻塞，读不到通常返回 -1) */
int sbi_console_getchar(void)
{
    struct sbiret ret;
    ret = sbi_ecall(SBI_EXT_0_1_CONSOLE_GETCHAR, 0, 0, 0, 0, 0, 0, 0);
    return ret.error;
}

/*
 * 设置下一次定时器中断触发的绝对时间 (MTIME)。
 * FreeRTOS 的 vPortSetupTimerInterrupt() 就是靠调用这个来产生 Tick 的。
 */
void sbi_set_timer(uint64_t stime_value)
{
#if __riscv_xlen == 32
    sbi_ecall(SBI_EXT_TIME, SBI_EXT_TIME_SET_TIMER, stime_value,
              stime_value >> 32, 0, 0, 0, 0);
#else
    sbi_ecall(SBI_EXT_TIME, SBI_EXT_TIME_SET_TIMER, stime_value, 0,
              0, 0, 0, 0);
#endif
}

/*
 * 发送核间中断 (IPI)。
 * 在 8 核 AMP 架构下，核心 A 唤醒或通知核心 B，全靠这个接口。
 * 参数 hart_mask: 一个位图。传地址过去，掩码为 1 的位对应的物理 Hart 将收到中断。
 */
void sbi_send_ipi(const unsigned long *hart_mask)
{
    unsigned long hmask_val = 0;
    unsigned long hbase = 0;

    /* 如果没传掩码，为了安全直接返回，不盲目发全核广播 */
    if (!hart_mask) {
        return; 
    }
    
    hmask_val = *hart_mask;
    
    /* 调用现代 SBI v0.2+ 的 IPI 发送接口 */
    sbi_ecall(SBI_EXT_IPI, SBI_EXT_IPI_SEND_IPI, hmask_val, hbase, 0, 0, 0, 0);
}

/* 让整个 QEMU 机器关机 */
void sbi_shutdown(void)
{
    sbi_ecall(SBI_EXT_0_1_SHUTDOWN, 0, 0, 0, 0, 0, 0, 0);
}