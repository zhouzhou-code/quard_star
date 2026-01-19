/* 指定目标体系结构为 RISC-V */
OUTPUT_ARCH("riscv") 

/* 指定程序入口点为 _start 符号 */
ENTRY(_start) 

/* 定义物理内存区域 */
MEMORY
{ 
   /* 定义一个名为"ddr"的内存区域，属性：可读(r)、可执行(x)、可分配(a)、已初始化(i)，但不可写(!w) */
   ddr (rxai!w) : ORIGIN = 0xBF800000, LENGTH = 7M  /* 起始地址0xBF800000，长度7M字节 */
}

/* 开始定义输出文件的段(section)布局 */
SECTIONS
{
  /* 定义.text段 - 存放所有程序代码 */
  .text :
  {
    /* 使用KEEP确保即使代码未被引用也不会被链接器优化掉，收集所有输入文件中的.text段 */
    KEEP(*(.text))  /* 通配符*表示所有输入文件，.text表示代码段 */
  } >ddr  /* 指定此段放置在ddr内存区域中 */
}  /* 链接脚本结束 */