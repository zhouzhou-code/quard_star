#!/bin/bash

# 传入参数：$1 = 空白镜像文件路径, $2 = sfdisk分区配置文件路径

# 1. 将空白镜像文件挂载到 loop70(/dev/loop70 块设备)，以便使用fdisk进行分区
losetup -o 0 --sizelimit 1073741824 /dev/loop70 $1 -P
# 2.读配置:使用fdisk工具读取sfdisk分区配置文件，对镜像文件进行分区
echo -e "I\n$2\nw\n" | fdisk /dev/loop70
losetup -d /dev/loop70  
sync

echo "please wait 5s" #等待设备创建好
sleep 5

losetup -o 0 --sizelimit 1073741824 /dev/loop70 $1 -P
# 3.在划好的两个分区上分别安装文件系统
mkfs.vfat /dev/loop70p1   # 分区 1 设为 FAT32，存放引导文件
mkfs.ext4 /dev/loop70p2   # 分区 2 设为 ext4，存放系统根目录

# 4.释放设备
losetup -d /dev/loop70
sync
