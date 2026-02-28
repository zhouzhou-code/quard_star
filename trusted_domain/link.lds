OUTPUT_ARCH( "riscv" )
ENTRY( _start )

__stack_size    = 0x1000; /* 栈大小设为 4KB */

MEMORY
{
    /* Fake ROM(其实是ram伪装的): 存放代码、常量、已初始化变量的初始值 */
    rom (rxa) : ORIGIN = 0xBF800000, LENGTH = 1M
    /* RAM: 存放运行时的变量、BSS、堆、栈 */
    ram (wxa) : ORIGIN = 0xBF900000, LENGTH = 6M
    /* 共享内存区域：0xbf700000 - 0xbf800000 (1MB) */
    /* 注意：这个区域是 FreeRTOS 和 Linux 之间的共享内存 */
}

SECTIONS
{
    /* 
    代码段 (.text)
     */
    .text : 
    {
        . = ALIGN(4);
        __text_start = .; 

        /* 把启动汇编强制钉在二进制文件绝对开头 */
        KEEP (*(SORT_NONE(.init))) 

        *(.text.unlikely .text.unlikely.*)
        *(.text.startup .text.startup.*)
        *(.text .text.*)
        *(.gnu.linkonce.t.*)

        KEEP (*(SORT_NONE(.fini)))

        . = ALIGN(4);
        __text_end = .; 
    } >rom AT>rom

    /* 
     * 构造与析构函数指针数组 (.init_array / .fini_array)
     */
    .preinit_array : 
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);
    } >rom AT>rom

    .init_array : 
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
    } >rom AT>rom

    .fini_array : 
    {
        . = ALIGN(4);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >rom AT>rom

    /* 
     * 只读数据段 (.rodata)
     *  */
    .rodata : 
    {
        . = ALIGN(8);
        __rodata_start = .; 
        
        *(.rdata)
        *(.rodata .rodata.*)
        *(.gnu.linkonce.r.*)
        
        . = ALIGN(8);
        __rodata_end = .; 
    } >rom AT>rom

    /* 
     * 已初始化数据段 (.data)
     */

    __data_lma = LOADADDR(.data);

    .data :
    {
        . = ALIGN(8);
        __data_start = .; /* RAM中的目的地起点 */

        *(.data .data.*)
        *(.gnu.linkonce.d.*)
        
        . = ALIGN(8);
        /* RISC-V 专属全局指针 (gp) 优化锚点 */
        PROVIDE( __global_pointer$ = . + 0x800 );
        
        *(.sdata .sdata.*)
        *(.sdata2 .sdata2.*)
        *(.gnu.linkonce.s.*)
        
        . = ALIGN(8);
        *(.srodata.cst16)
        *(.srodata.cst8)
        *(.srodata.cst4)
        *(.srodata.cst2)
        *(.srodata .srodata.*)

        . = ALIGN(8);
        __data_end = .;   /* RAM中的目的地终点 */
    } >ram AT>rom

    /*
     * 共享内存段 - 放置 Resource Table 供 Linux 访问
     * 必须放在 0xbf70c000
     */
    .resource_table (NOLOAD) :
    {
        . = ALIGN(8);
        __resource_table_start = .;

        *(.resource_table)

        . = ALIGN(8);
        __resource_table_end = .;
    } >ram AT>rom

    .bss (NOLOAD) :
    {
        . = ALIGN(8);
        __bss_start = .;  /* 清零循环起点 */

        *(.sbss*)
        *(.gnu.linkonce.sb.*)
        *(.bss .bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)

        . = ALIGN(8);
        __bss_end = .;    /* 清零循环终点 */
        /* 增加下面这两行，给 C 标准库的 _sbrk 函数指路 */
        PROVIDE( end = . );
        PROVIDE( _end = . );
    } >ram AT>rom

   /* 
     * 7. 划定栈空间 (Stack) 
    */
    __stack_top    = ORIGIN(ram) + LENGTH(ram);
    __freertos_irq_stack_top = __stack_top ;
    __stack_bottom = __stack_top - __stack_size;

    /* 
     * 8. 收集剩余空间给堆 (Heap) 
     * 我们不用 malloc，所以把 BSS 结束到栈底之间的所有“废弃空间”定义为堆
     *  */
    __heap_start = .;              /* 堆的起点紧接在 BSS 之后 */
    __heap_limit = __stack_bottom; /* 堆的极限绝对不能越过栈底 */

    /* 
    终极防线：内存溢出编译期拦截
      */
    /* 如果你的全局变量开得太大，导致 BSS 段直接撞到了栈底，链接器会当场报错死给你看 */
    ASSERT(__heap_start <= __heap_limit, "🚨 FATAL ERROR: RAM overflowed! BSS crashed into the Stack region.")
}