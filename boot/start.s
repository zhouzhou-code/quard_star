.macro loop,cunt                   /* 定义一个简单loop宏,cunt是loop参数 */
    li      t1, 0xffff             /* 加载立即数到t1 */
    li      t2, \cunt              /* 加载立即数到t2 */
1:
    nop                             /* 空指令nop */
    addi    t1, t1, -1              /* t1-- */
    bne     t1, x0, 1b              /* 判断t1是否等于0，不是就跳转前以符号1处（b是向前的意思） */
    li      t1, 0xffff              /* 加载立即数到t1 */
    addi    t2, t2, -1              /* t2-- */
    bne     t2, x0, 1b              /* 判断t2是否等于0，不是就跳转前一个符号1处（b是向前的意思） */     
    .endm        

/* 定义一个简单load_data宏（这里我们按word拷贝数据，实际上64位可以按double word来拷贝，效率更高）
    _src_start为源地址，_dst_start为目标地址，_dst_end为目标结束地址 */
    .macro load_data,_src_start,_dst_start,_dst_end
    bgeu    \_dst_start, \_dst_end, 2f    /* 判断目标结束地址大于起始地址，即是否合法 */
1:
    lw      t0, (\_src_start)             /* 加载源地址内数据到t0 */
    sw      t0, (\_dst_start)             /* 写入t0到目标地址内 */
    addi    \_src_start, \_src_start, 4   /* 源地址+4 */
    addi    \_dst_start, \_dst_start, 4   /* 目标地址+4 */
    bltu    \_dst_start, \_dst_end, 1b    /* 判断是否已到达结束地址，未到达则循环到上前一个符号1 */
2:
    .endm


    .section .data
    .globl  _pen
    .type   _pen,%object
_pen:
    .word   1

    .section .text
    .globl _start
    .type _start,@function

_start:
    csrr    a0, mhartid                   /* 获取当前HART的ID */
    beq     a0, zero, _no_wait            /* 只有核心0执行加载 */
    
_loop:
    loop    0x1000                        /* 其他核心忙等待循环 */
    la      t0, _pen
    lw      t0, 0(t0)
    beq     t0, zero, _run                /* 等待_pen标志被清除 */
    j       _loop

_no_wait:
    la      t0, _pen
    la      t1, 1
    sw      t1, 0(t0)                     /* 设置_pen=1，表示加载中 */
    
    /* 加载OpenSBI固件 opensbi_fw.bin */
    /* [0x20200000:0x20400000] --> [0xBFF80000:0xC0000000] */
    li      a0, 0x202
    slli    a0, a0, 20                    /* 源地址: 0x20200000 */
    li      a1, 0xbff
    slli    a1, a1, 20                    /* 目标地址: 0xBFF00000 */
    li      a2, 0x800
    slli    a2, a2, 8                     /* 偏移: 0x80000 */
    add     a1, a1, a2                    /* 目标起始地址: 0xBFF80000 */
    add     a2, a1, a2                    /* 目标结束地址: 0xC0000000 */
    load_data a0, a1, a2                  /* 拷贝0x20200000到0xBFF80000 */
    
    /* 加载OpenSBI设备树 qemu_sbi.dtb */
    /* [0x20080000:0x20100000] --> [0xBFF00000:0xBFF80000] */
    li      a0, 0x2008
    slli    a0, a0, 16                    /* 源地址: 0x20080000 */
    li      a1, 0xbff
    slli    a1, a1, 20                    /* 目标地址: 0xBFF00000 */
    li      a2, 0x800
    slli    a2, a2, 4                     /* 大小: 0x8000 */
    add     a2, a1, a2                    /* 结束地址: 0xBFF08000 */
    load_data a0, a1, a2                  /* 加载OpenSBI设备树 */
    
    /* 加载安全域固件 trusted_fw.bin */
    /* [0x20400000:0x20800000] --> [0xBF800000:0xBFC00000] */
    li      a0, 0x204
    slli    a0, a0, 20                    /* 源地址: 0x20400000 */
    li      a1, 0xbf8
    slli    a1, a1, 20                    /* 目标地址: 0xBF800000 */
    li      a2, 0xbfc
    slli    a2, a2, 20                    /* 结束地址: 0xBFC00000 */
    load_data a0, a1, a2                  /* 加载可信固件 */
    
    /* 加载U-Boot设备树 u-boot.dtb */
    /* [0x20100000:0x20180000] --> [0xB0000000:0xB0080000] */
    li      a0, 0x201
    slli    a0, a0, 20                    /* 源地址: 0x20100000 */
    li      a1, 0xb00
    slli    a1, a1, 20                    /* 目标地址: 0xB0000000 */
    li      a2, 0x800
    slli    a2, a2, 8                     /* 大小: 0x80000 */
    add     a2, a1, a2                    /* 结束地址: 0xB0080000 */
    load_data a0, a1, a2                  /* 加载U-Boot设备树 */
    
    /* 加载U-Boot固件 u-boot.bin */
    /* [0x20800000:0x20C00000] --> [0xB0200000:0xB0600000] */
    li      a0, 0x208
    slli    a0, a0, 20                    /* 源地址: 0x20800000 */
    li      a1, 0xb02
    slli    a1, a1, 20                    /* 目标地址: 0xB0200000 */
    li      a2, 0xb06
    slli    a2, a2, 20                    /* 结束地址: 0xB0600000 */
    load_data a0, a1, a2                  /* 加载U-Boot固件 */

_run:
    csrr    a0, mhartid                   /* 再次获取核心ID */
    li      a1, 0xbff
    slli    a1, a1, 20                    /* 基地址: 0xBFF00000 */
    li      t0, 0x800
    slli    t0, t0, 8                     /* 偏移: 0x80000 */
    add     t0, a1, t0                    /* 跳转地址 = 0xBFF80000 即opensbi.bin的地址 */
    la      t1, _pen
    sw      zero, 0(t1)                   /* 清除_pen标志，释放所有核心 */
    jr      t0                            /* 所有核心跳转到OpenSBI */