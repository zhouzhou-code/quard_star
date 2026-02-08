SHELL_FOLDER=$(cd "$(dirname "$0")"; pwd)
DEFAULT_VC="1280x720"

$SHELL_FOLDER/output/qemu/bin/qemu-system-riscv64 \
-M quard-star  \
-m 1G \
-smp 8 \
-bios none \
-drive if=pflash,bus=0,unit=0,format=raw,file=$SHELL_FOLDER/output/fw/fw.bin \
-drive file=$SHELL_FOLDER/output/rootfs/rootfs.img,format=raw,id=hd0 \
-device virtio-blk-device,drive=hd0 \
-fw_cfg name="opt/qemu_cmdline",string="qemu_vc="$DEFAULT_VC"" \
-d in_asm -D qemu.log \
-serial mon:stdio \
-nographic --parallel none 
# -s -S

#将三个串口以TCP服务器方式输出，方便使用telnet连接调试 
# -serial tcp::4444,server,nowait \   连接：telnet localhost 4444
# -serial tcp::4445,server,nowait \
# -serial tcp::4446,server,nowait \
# -s -S


# -serial mon:stdio \

# --serial vc:$DEFAULT_VC --serial vc:$DEFAULT_VC --serial vc:$DEFAULT_VC --monitor vc:$DEFAULT_VC --parallel none

#-monitor stdio

# 终端使用telnet localhost 4444监听串口

