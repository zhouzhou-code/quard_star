/* 指定目标体系结构为 RISC-V */
OUTPUT_ARCH("riscv")

/* 指定程序入口点为 _start 符号 */
ENTRY(_start)

/* 定义物理内存区域 */
MEMORY
{ 
    /*
     * flash 区域：存储固件的非易失性存储器
     * 属性：可读(r)、可执行(x)、可分配(a)、已初始化(i)、不可写(!w)
     * 起始地址：0x20000000（pflash的起始地址）
     * 大小：512KB（0x80000字节）
     * 说明：代码将在flash中原地执行（XIP，eXecute In Place）
     */
    flash (rxai!w) : ORIGIN = 0x20000000, LENGTH = 512k
    
    /*
     * sram 区域：快速静态随机存取存储器（通常为芯片内部RAM）
     * 属性：可读(r)、可执行(x)、可分配(a)、已初始化(i)、可写(w)
     * 起始地址：0x00008000（芯片内部SRAM的地址）
     * 大小：32KB（0x8000字节）
     * 说明：用于存放运行时的可写数据，速度比flash快
     */
    sram  (rxaiw)  : ORIGIN = 0x00008000, LENGTH = 32k
}

/* 定义输出文件的段(section)布局 */
SECTIONS
{
    /*
     * .text 段：存放程序代码
     * 放置在flash区域，因为代码通常是只读的
     * 这意味着代码将直接在flash中执行（XIP模式）
     */
    .text :
    {
        /* 收集所有输入文件中的代码段 */
        KEEP(*(.text))
    } >flash  /*段域的地址(LMA和VMA相同)位于名为flash内存域*/

    /*
     * .data 段：存放已初始化的全局和静态变量
     * 运行时地址(VMA)在sram中（因为需要可写）
     * 加载地址(LMA)在flash中（因为初始值需要存储在非易失性存储器中）
     * 语法解释：
     *   >sram     - 运行时地址在sram区域
     *   AT >flash - 加载地址在flash区域
     */
    .data :
    {
        /* 收集所有输入文件中的.data段 */
        KEEP(*(.data))
    } >sram AT >flash
    /* 重要：这意味着.data段的内容编译时会放在flash中，
       但程序运行时需要将这部分数据从flash复制到sram中 */
}