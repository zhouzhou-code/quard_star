#! /bin/sh
PATH=/sbin:/bin:/usr/sbin:/usr/bin

#执行fstab里的挂载，把内核虚拟文件系统挂载上到根文件系统
mount -a
#生成设备节点
/sbin/mdev -s
#上一步生成设备节点后东西变了，重新挂载fstab里的东西
mount -a

echo "---------------------------------------------"
echo " Welcome debugging on Qemu Quard Star board! "
echo "---------------------------------------------"