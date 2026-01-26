# 获取当前脚本文件所在的目录
SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)

if [ ! -d "$SHELL_FOLDER/output" ]; then  
mkdir $SHELL_FOLDER/output
fi  

echo "------------------------- 编译qemu---------------------------------------"
cd qemu-8.0.2
if [ ! -d "$SHELL_FOLDER/output/qemu" ]; then  
./configure --prefix=$SHELL_FOLDER/output/qemu  --target-list=riscv64-softmmu --enable-gtk  --enable-virtfs --disable-gio
fi  
make -j16
sudo make install


# # 编译 lowlevelboot
echo "------------------------- 编译lowlevelboot-----------------------------"
export PATH="/home/zms/riscv64-lp64d--glibc--stable-2025.08-1/bin:$PATH"
CROSS_PREFIX=riscv64-linux


if [ ! -d "$SHELL_FOLDER/output/lowlevelboot" ]; then  
mkdir $SHELL_FOLDER/output/lowlevelboot
fi  
cd  $SHELL_FOLDER/boot
$CROSS_PREFIX-gcc -x assembler-with-cpp -c start.s -o $SHELL_FOLDER/output/lowlevelboot/start.o
$CROSS_PREFIX-gcc -nostartfiles -T./boot.lds -Wl,-Map=$SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.map -Wl,--gc-sections $SHELL_FOLDER/output/lowlevelboot/start.o -o $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.elf
# 使用gnu工具生成原始的程序bin文件
$CROSS_PREFIX-objcopy -O binary -S $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.elf $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.bin
# 使用gnu工具生成反汇编文件，方便调试分析（当然我们这个代码太简单，不是很需要）
$CROSS_PREFIX-objdump --source --demangle --disassemble --reloc --wide $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.elf > $SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.lst

#编译 opensbi
echo "------------------------- 编译opensbi --------------------------------"
if [ ! -d "$SHELL_FOLDER/output/opensbi" ]; then  
mkdir $SHELL_FOLDER/output/opensbi
fi  
cd $SHELL_FOLDER/opensbi-1.2  
make distclean #必须要重新编译，否则改的链接地址不会生效
make CROSS_COMPILE=$CROSS_PREFIX- PLATFORM=quard_star FW_TEXT_START=0xBFF80000 FW_JUMP=y FW_JUMP_ADDR=0xB0200000
# make CROSS_COMPILE=$CROSS_PREFIX- PLATFORM=quard_star
cp -r $SHELL_FOLDER/opensbi-1.2/build/platform/quard_star/firmware/fw_jump.bin $SHELL_FOLDER/output/opensbi/fw_jump.bin
cp -r $SHELL_FOLDER/opensbi-1.2/build/platform/quard_star/firmware/fw_jump.elf $SHELL_FOLDER/output/opensbi/fw_jump.elf
$CROSS_PREFIX-objdump --source --demangle --disassemble --reloc --wide $SHELL_FOLDER/output/opensbi/fw_jump.elf > $SHELL_FOLDER/output/opensbi/fw_jump.lst

# 生成sbi.dtb
cd $SHELL_FOLDER/dts
dtc -I dts -O dtb -o $SHELL_FOLDER/output/opensbi/quard_star_sbi.dtb quard_star_sbi.dts
# 生成uboot.dtb
cd $SHELL_FOLDER/dts
dtc -I dts -O dtb -o $SHELL_FOLDER/output/uboot/quard_star_uboot.dtb quard_star_uboot.dts

#编译trusted_domain
if [ ! -d "$SHELL_FOLDER/output/trusted_domain" ]; then  
mkdir $SHELL_FOLDER/output/trusted_domain
fi  
cd $SHELL_FOLDER/trusted_domain
$CROSS_PREFIX-gcc -x assembler-with-cpp -c startup.s -o $SHELL_FOLDER/output/trusted_domain/startup.o
$CROSS_PREFIX-gcc -nostartfiles -T./link.lds -Wl,-Map=$SHELL_FOLDER/output/trusted_domain/trusted_fw.map -Wl,--gc-sections $SHELL_FOLDER/output/trusted_domain/startup.o -o $SHELL_FOLDER/output/trusted_domain/trusted_fw.elf
$CROSS_PREFIX-objcopy -O binary -S $SHELL_FOLDER/output/trusted_domain/trusted_fw.elf $SHELL_FOLDER/output/trusted_domain/trusted_fw.bin
$CROSS_PREFIX-objdump --source --demangle --disassemble --reloc --wide $SHELL_FOLDER/output/trusted_domain/trusted_fw.elf > $SHELL_FOLDER/output/trusted_domain/trusted_fw.lst

#编译uboot
echo "------------------------- 编译uboot --------------------------------"
if [ ! -d "$SHELL_FOLDER/output/uboot" ]; then  
mkdir $SHELL_FOLDER/output/uboot
fi  
cd $SHELL_FOLDER/u-boot-2026.01
make CROSS_COMPILE=$CROSS_PREFIX- qemu-quard-star_defconfig
make CROSS_COMPILE=$CROSS_PREFIX- -j16 
cp $SHELL_FOLDER/u-boot-2026.01/u-boot $SHELL_FOLDER/output/uboot/u-boot.elf
cp $SHELL_FOLDER/u-boot-2026.01/u-boot.map $SHELL_FOLDER/output/uboot/u-boot.map
cp $SHELL_FOLDER/u-boot-2026.01/u-boot-nodtb.bin $SHELL_FOLDER/output/uboot/u-boot.bin
$CROSS_PREFIX-objdump --source --demangle --disassemble --reloc --wide $SHELL_FOLDER/output/uboot/u-boot.elf > $SHELL_FOLDER/output/uboot/u-boot.lst


# 合成firmware固件
if [ ! -d "$SHELL_FOLDER/output/fw" ]; then  
mkdir $SHELL_FOLDER/output/fwc
fi  
cd $SHELL_FOLDER/output/fw
rm -rf fw.bin
#填充32K的0
dd of=fw.bin bs=1k count=32k if=/dev/zero

#写入lowlevelboot程序 偏移量0k
dd of=fw.bin bs=1k conv=notrunc seek=0 if=$SHELL_FOLDER/output/lowlevelboot/lowlevel_fw.bin
#写入quard_star_sbi.dtb 偏移量512,因此fdt的地址偏移量为0x80000
dd of=fw.bin bs=1k conv=notrunc seek=512 if=$SHELL_FOLDER/output/opensbi/quard_star_sbi.dtb
#写入quard_star_uboot.dtb 偏移量1024,因此fdt的地址偏移量为0x100000
dd of=fw.bin bs=1k conv=notrunc seek=1k if=$SHELL_FOLDER/output/uboot/quard_star_uboot.dtb
#写入opensbi程序 偏移量2k*1k=2048k=0x200000
dd of=fw.bin bs=1k conv=notrunc seek=2k if=$SHELL_FOLDER/output/opensbi/fw_jump.bin
#写入trusted_domain.bin 偏移量1k*4k=0x400000
dd of=fw.bin bs=1k conv=notrunc seek=4k if=$SHELL_FOLDER/output/trusted_domain/trusted_fw.bin
#写入uboot 偏移量8k*1k=0x800000
dd of=fw.bin bs=1k conv=notrunc seek=8k if=$SHELL_FOLDER/output/uboot/u-boot.bin


#写入固件的偏移地址改成如下:
# load opensbi_fw.bin 
# [0x00200000

# load qemu_sbi.dtb
# [0x00080000

# load trusted_fw.bin
# [0x00400000

# load qemu_uboot.dtb
# [0x00100000

# load u-boot.bin
# [0x00800000