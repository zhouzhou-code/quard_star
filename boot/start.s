/* ================= 修改点1：定义绝对地址和魔数 ================= */
#define LOCK_ADDR       0x8000       /* 使用验证过可写的 SRAM 地址 */
#define MAGIC_GO_VAL    0x88888888   /* 魔法数，读到它才准走 */

/* ================= 修改点2：减小循环次数，防止睡太久 ================= */
.macro loop,cunt
    li      t1, 0xff        /* [修改] 从 0xffff 改为 0xff，大大缩短延时 */
    li      t2, \cunt
1:
    nop
    addi    t1, t1, -1
    bne     t1, x0, 1b
    li      t1, 0xff      
    addi    t2, t2, -1
    bne     t2, x0, 1b
    .endm

    .macro load_data,_src_start,_dst_start,_dst_end
    bgeu    \_dst_start, \_dst_end, 2f
1:
    lw      t0, (\_src_start)
    sw      t0, (\_dst_start)
    addi    \_src_start, \_src_start, 4
    addi    \_dst_start, \_dst_start, 4
    bltu    \_dst_start, \_dst_end, 1b
2:
    .endm

    .section .text
    .globl _start
    .type _start,@function

_start:
    csrr    a0, mhartid
    beq     a0, zero, _no_wait

/* Hart 1-7 从核等待逻辑 */
_loop: 
    /* 加读屏障，防止读到缓存旧值 */
    fence   r, r
    
    /* 直接读绝对地址 0x8000 */
    li      t0, LOCK_ADDR //将立即数(锁地址)加载到寄存器t0
    lwu     t0, 0(t0)     //从t0+0源地址处加载无符号数到寄存器t0
                          //一定要按照无符号读:lwu，寄存器高32位清零，把值加载到低32位
    
    /* 比较魔法数，相等才跳 */
    li      t1, MAGIC_GO_VAL
    beq     t0, t1, _run_slave //如果t0 equal t1,跳转到_run_slave标签处
    
    loop    0x10
    j       _loop

/* Hart 0 启动逻辑 */
_no_wait: 
    /* 先清零锁，防止意外撞上魔数 */
    li      t0, LOCK_ADDR
    sw      zero, 0(t0)

    /* 加上写屏障，确保内存清零生效 */
    fence   w, w  
    
    /* 加载OpenSBI固件到内存; 原地址:0x20200000-->目标地址0xBFF80000 */
    li      a0, 0x202  //加载立即数0x202到寄存器a0
    slli    a0, a0, 20 //将寄存器a0左移20位，a0 = 0x20200000
    li      a1, 0xbff  //加载立即数0xbff到寄存器a1
    slli    a1, a1, 20 //将寄存器a1左移20位，a1 = 0xbff00000
    li      a2, 0x800  //加载立即数0x800到寄存器a2
    slli    a2, a2, 8  //将寄存器a2左移8位，a2 = 0x80000
    add     a1, a1, a2 //将寄存器a1和a2相加，a1 = 0xbff00000+0x80000=0xbff80000 ,目标地址起始地址
    add     a2, a1, a2 //将寄存器a1和a2相加，a2 = 0xbff80000 + 0x80000=0xc00000，目标地址结束地址
    load_data a0, a1, a2 //调用load_data宏，将数据从源地址a0复制到目标地址a1，直到目标地址达到a2
    
    /* 加载OpenSBI设备树到内存; 原地址:0x20080000-->目标地址0xBFF00000 */
    li      a0, 0x2008 //加载立即数0x2008到寄存器a0
    slli    a0, a0, 16 //将寄存器a0左移16位，a0 = 0x20080000
    li      a1, 0xbff  //加载立即数0xbff到寄存器a1
    slli    a1, a1, 20 //将寄存器a1左移20位，a1 = 0xbff00000
    li      a2, 0x800  //加载立即数0x800到寄存器a2
    slli    a2, a2, 4  //将寄存器a2左移4位，a2 = 0x8000
    add     a2, a1, a2 //将寄存器a1和a2相加，a2 = 0xbff00000+0x8000=0xbff08000，目标地址结束地址
    load_data a0, a1, a2 //调用load_data宏，将数据从源地址a0复制到目标地址a1，直到目标地址达到a2
    
    /* 加载安全域固件；原地址:0x20400000-->目标地址0xBF800000 */
    li      a0, 0x204  //加载立即数0x204到寄存器a0
    slli    a0, a0, 20 //将寄存器a0左移20位，a0 = 0x20400000 原地址
    li      a1, 0xbf8  //加载立即数0xbf8到寄存器a1
    slli    a1, a1, 20 //将寄存器a1左移20位，a1 = 0xbf800000 目标地址起始地址
    li      a2, 0xbfc  //加载立即数0xbfc到寄存器a2
    slli    a2, a2, 20 //将寄存器a2左移20位，a2 = 0xbfc00000 目标地址结束地址
    load_data a0, a1, a2
    
    /* 加载U-Boot设备树；原地址:0x20100000-->目标地址0xB0000000 */
    li      a0, 0x201  //加载立即数0x201到寄存器a0
    slli    a0, a0, 20 //将寄存器a0左移20位，a0 = 0x20100000
    li      a1, 0xb00  //加载立即数0xb00到寄存器a1
    slli    a1, a1, 20 //将寄存器a1左移20位，a1 = 0xb0000000 目标地址起始地址
    li      a2, 0x800  //加载立即数0x800到寄存器a2
    slli    a2, a2, 8  //将寄存器a2左移8位，a2 = 0x80000
    add     a2, a1, a2 //将寄存器a1和a2相加，a2 = 0xb0000000 + 0x80000=0xb0080000 目标地址结束地址
    load_data a0, a1, a2 //调用load_data宏，将数据从源地址a0复制到目标地址a1，直到目标地址达到a2
    
    /* 加载U-Boot固件；原地址:0x20800000-->目标地址0xB0200000 */
    li      a0, 0x208  //加载立即数0x208到寄存器a0
    slli    a0, a0, 20 //将寄存器a0左移20位，a0 = 0x20800000
    li      a1, 0xb02  //加载立即数0xb02到寄存器a1
    slli    a1, a1, 20 //将寄存器a1左移20位，a1 = 0xb0200000 目标地址起始地址
    li      a2, 0xb06  //加载立即数0xb06到寄存器a2
    slli    a2, a2, 20 //将寄存器a2左移20位，a2 = 0xb0600000 目标地址结束地址
    load_data a0, a1, a2 //调用load_data宏，将数据从源地址a0复制到目标地址a1，直到目标地址达到a2
    
    /* 写屏障，保证数据搬完才能开锁 */
    fence   w, w
    
    /* 写入魔法数 0x88888888 */
    li      t0, LOCK_ADDR
    li      t1, MAGIC_GO_VAL
    sw      t1, 0(t0)
    
    /* Hart 0 不需要等，直接去计算地址 */
    j       _calc_addr

_run_slave:
    /* 从核跳到这里，不需要再做任何事，直接去_calc_addr */

_calc_addr:
    /* 计算 OpenSBI 入口地址 */
    li      a1, 0xbff
    slli    a1, a1, 20
    li      t0, 0x800
    slli    t0, t0, 8
    add     t0, a1, t0  /* 0xBFF80000 */
    
    /* Hart 0 这里的 loop 其实可以去掉了，因为用了魔法数锁 */
    /* 但你想保留也可以，改小一点 */
    /* loop 0x10 */ 
    
    jr      t0