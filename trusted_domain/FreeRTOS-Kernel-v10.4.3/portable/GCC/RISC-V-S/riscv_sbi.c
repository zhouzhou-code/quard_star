#include "riscv_sbi.h"

/* Legacy SBI call wrapper */
static inline long sbi_call(long ext, long arg0, long arg1, long arg2) {
    register long a0 asm("a0") = arg0;
    register long a1 asm("a1") = arg1;
    register long a2 asm("a2") = arg2;
    register long a7 asm("a7") = ext;

    asm volatile("ecall"
                 : "+r"(a0), "+r"(a1)
                 : "r"(a2), "r"(a7)
                 : "memory");
    return a0;
}

void sbi_set_timer(uint64_t stime_value) {
#if __riscv_xlen == 32
    sbi_call(SBI_EXT_0_1_SET_TIMER, (uint32_t)stime_value, (uint32_t)(stime_value >> 32), 0);
#else
    sbi_call(SBI_EXT_0_1_SET_TIMER, stime_value, 0, 0);
#endif
}

void sbi_console_putchar(int ch) {
    sbi_call(SBI_EXT_0_1_CONSOLE_PUTCHAR, ch, 0, 0);
}

int sbi_console_getchar(void) {
    return sbi_call(SBI_EXT_0_1_CONSOLE_GETCHAR, 0, 0, 0);
}
