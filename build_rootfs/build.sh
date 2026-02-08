#!/bin/bash

#该脚本将传入的rootfs.img镜像文件进行挂载，以便将bootfs和rootfs内容写入对应分区

# $1 = 包含bootfs和rootfs文件夹的输出目录路径

# 1.通过loop70访问镜像文件,-P扫描镜像文件分好的区
losetup -o 0 --sizelimit 1073741824 /dev/loop70 $1/rootfs.img -P

# 2. 在arget下创建临时挂载目录
mkdir -p $1/target/bootfs $1/target/rootfs

# 3. 将镜像文件系统的p1,p2分别挂载到宿主机目录的bootfs和rootfs目录下
mount /dev/loop70p1 $1/target/bootfs
mount /dev/loop70p2 $1/target/rootfs

# 4.将之前准备好的文件cp到挂载目录下
cp -r $1/bootfs/* $1/target/bootfs/  # 拷贝内核、dtb、boot.scr
cp -r $1/rootfs/* $1/target/rootfs/  # 拷贝 BusyBox 根文件系统内容

# 5.强制同步与撤销：sync 确保数据从缓存写进磁盘文件，然后卸载
sync

echo "please wait 5s"
sleep 5

umount $1/target/bootfs 
umount $1/target/rootfs
losetup -d /dev/loop70