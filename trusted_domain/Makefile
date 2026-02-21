# 【1. 工具链配置】
# 指定你的 64 位 GCC 工具链前缀
CROSS_COMPILE :=/opt/gcc-riscv64-unknown-elf/bin/riscv64-unknown-elf-
CC      := $(CROSS_COMPILE)gcc
LD      := $(CROSS_COMPILE)ld
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
SIZE    := $(CROSS_COMPILE)size

# 【2. 架构与ABI参数】
# rv64gc: g 代表 imafd (带整数、乘除、原子操作、单/双精度硬件浮点)，c 代表压缩指令。
# lp64d: 告诉编译器，用 64 位硬件寄存器来传 double 浮点数
MARCH ?= rv64gc
MABI  ?= lp64d
# -mcmodel=medany: RISC-V 必备！让代码能寻址到正负 2GB 的绝对物理地址范围，S-Mode 下极其关键。
ARCH_FLAGS := -march=$(MARCH) -mabi=$(MABI) -mcmodel=medany

# 【3. 目录与产物配置】
BUILD_DIR = build
TARGET    = $(BUILD_DIR)/RTOSDemo
# 这里填入咱们前面精心打磨的物理内存映射脚本
LDSCRIPT  = link.lds

# 【4. 精准狙击：源文件目录】
# 只有被加进这里的文件夹，Makefile 才会去里面抓代码。
# 后续写了驱动代码 (如 src/driver)，直接在下面追加即可。
SRC_DIRS  = . \
            FreeRTOS-Kernel-v10.4.3 \
            FreeRTOS-Kernel-v10.4.3/portable/GCC/RISC-V-S \
            riscv

# 【5. 头文件搜索路径 (-I)】
INCLUDES  = $(foreach dir, $(SRC_DIRS), -I$(dir))
INCLUDES += -I FreeRTOS-Kernel-v10.4.3/include
INCLUDES += -I FreeRTOS-Kernel-v10.4.3/portable/MemMang
INCLUDES += -I riscv/


# 【6. 自动抓取食材 (匹配源码)】
# 去白名单目录里，把所有的 .c、.S(手写汇编) 和 .s(编译生成的汇编) 全捞出来
SRCS_C   = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
SRCS_C  += FreeRTOS-Kernel-v10.4.3/portable/MemMang/heap_4.c
SRCS_ASM = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.S))
SRCS_asm = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.s))

# 【7. 魔法替换：生成中间 .o 和 .d 路径】
# 保持原有的目录结构树，把它完整平移到 build/ 目录下
# 例如: src/main.c -> build/src/main.o
OBJS  = $(SRCS_C:%.c=$(BUILD_DIR)/%.o)
OBJS += $(SRCS_ASM:%.S=$(BUILD_DIR)/%.o)
OBJS += $(SRCS_asm:%.s=$(BUILD_DIR)/%.o)

# 生成依赖文件列表 (.d)
DEPS  = $(OBJS:.o=.d)

# 【8. 炒菜配料 (CFLAGS 编译选项)】
CFLAGS  = $(ARCH_FLAGS)
# -O0 -g3: 关掉优化，保留最详尽的调试信息，初期板子 Bring-up 必备，方便单步调试！
CFLAGS += -O0 -g3 -Wall -Wextra
# 裸机双雄：告诉 GCC "这里没有操作系统"，别自作聪明替换我的基础函数
CFLAGS += -ffreestanding -fno-builtin
# 垃圾回收前置准备：给每个函数和变量分配独立的"小单间"
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += $(INCLUDES)

# 【9. 摆盘要求 (LDFLAGS 链接选项)】
LDFLAGS  = $(ARCH_FLAGS)
LDFLAGS += -T $(LDSCRIPT)
# 滚蛋吧默认C启动代码：我们用自己的 startup.S
LDFLAGS += -nostartfiles
# 垃圾回收：把没用到的"小单间"全部扔掉，固件体积瞬间暴瘦
LDFLAGS += -Wl,--gc-sections
# 生成 Map 地址表：排查内存越界、查崩溃地址的神器
LDFLAGS += -Wl,-Map=$(TARGET).map
# LDFLAGS += --specs=nano.specs 
LDFLAGS += --specs=nosys.specs

# 【10. 伪目标定义】
.PHONY: all clean dump size

# 默认一键三连：生成 bin、反汇编 asm、打印内存占用
all: $(TARGET).elf $(TARGET).bin dump size

# 【11. 核心构建规则】

# 最终缝合 (ELF)
$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	@echo "  LD      $@"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# 提取纯二进制烧录文件 (BIN)
$(TARGET).bin: $(TARGET).elf
	@echo "  OBJCOPY $@"
	@$(OBJCOPY) -O binary $< $@

# 逆向反汇编 (ASM) - 底层 debug 必看
dump: $(TARGET).elf
	@echo "  OBJDUMP $(TARGET).asm"
	@$(OBJDUMP) -S -d $< > $(TARGET).asm

# 打印内存账单 (SIZE)
size: $(TARGET).elf
	@echo "  SIZE    $<"
	@$(SIZE) $<

# 万能模具：编译所有的 .c 文件
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)  # 智能建文件夹，保证目录树不散架
	@echo "  CC      $<"
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# 万能模具：编译大写 .S 汇编文件
$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	@echo "  AS      $<"
	@$(CC) $(CFLAGS) -D__ASSEMBLY__ -MMD -MP -c $< -o $@

# 万能模具：编译小写 .s 汇编文件
$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo "  AS      $<"
	@$(CC) $(CFLAGS) -D__ASSEMBLY__ -MMD -MP -c $< -o $@

# 一键清理
clean:
	@echo "  CLEAN   $(BUILD_DIR)"
	@rm -rf $(BUILD_DIR)

# 包含所有的 .d 头文件依赖记录，改了 .h 自动重新编译关联文件！
-include $(DEPS)
