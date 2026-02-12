#ifndef RISCV_SBI_H
#define RISCV_SBI_H

#include <stdint.h>

/* Legacy SBI Extension IDs */
#define SBI_EXT_0_1_SET_TIMER           0x0
#define SBI_EXT_0_1_CONSOLE_PUTCHAR     0x1
#define SBI_EXT_0_1_CONSOLE_GETCHAR     0x2
#define SBI_EXT_0_1_CLEAR_IPI           0x3
#define SBI_EXT_0_1_SEND_IPI            0x4
#define SBI_EXT_0_1_REMOTE_FENCE_I      0x5
#define SBI_EXT_0_1_REMOTE_SFENCE_VMA   0x6
#define SBI_EXT_0_1_REMOTE_SFENCE_VMA_ASID 0x7
#define SBI_EXT_0_1_SHUTDOWN            0x8

/* SBI Return Error Codes */
#define SBI_SUCCESS                 0
#define SBI_ERR_FAILED              -1
#define SBI_ERR_NOT_SUPPORTED       -2
#define SBI_ERR_INVALID_PARAM       -3
#define SBI_ERR_DENIED              -4
#define SBI_ERR_INVALID_ADDRESS     -5
#define SBI_ERR_ALREADY_AVAILABLE   -6

typedef struct sbiret {
    long error;
    long value;
} sbiret_t;

void sbi_set_timer(uint64_t stime_value);
void sbi_console_putchar(int ch);
int sbi_console_getchar(void);

#endif /* RISCV_SBI_H */
