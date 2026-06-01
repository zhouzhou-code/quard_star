# rootfs 内容来源 / 如何往最终 rootfs 加东西

最终 `output/rootfs/rootfs.img` 的组装流程（`build.sh build_rootfs`）：

```
buildroot 基础 rootfs (rootfs.tar)         ← busybox + glibc + 本 overlay
        +  本目录 overlay (BR2_ROOTFS_OVERLAY)  ← 静态文件直接覆盖到 /
        +  注入 project/drivers/*.ko -> /driver/  ← build_driver 编出的内核模块
        =>  打包进 rootfs.img 的 ext4 分区(p2)
boot 分区(p1, vfat): kernel Image + quard_star.dtb + boot.scr
```

## 想加自己的东西，放哪？

| 你要加的 | 放哪 / 怎么做 | 说明 |
|---|---|---|
| 静态文件：配置/脚本/预编译二进制 | **直接放本目录** `project/rootfs_overlay/`，按目标路径建子目录（如 `etc/xxx`、`usr/bin/xxx`） | buildroot 通过 `BR2_ROOTFS_OVERLAY` 原样覆盖到 rootfs 根，无需改 build.sh |
| 内核驱动模块 `.ko` | 放 `project/drivers/<名字>/`（带 Makefile） | `build_driver` 自动编译并注入到 rootfs 的 `/driver/` |
| 需要编译的用户态程序 | ① 作为 buildroot 包（改 `buildrootconfig`）；或 ② 编好后像驱动那样在 `build_rootfs` 里注入；或 ③ 预编译好直接丢进本 overlay | 简单程序用 ②/③，要依赖管理用 ① |
| 改 busybox 启用的 applet | `./build.sh buildrootconfig` -> Target packages -> BusyBox configuration | busybox 现由 buildroot 管 |

## 注意
- 本目录里的文件会**原样覆盖** buildroot 默认 rootfs 里的同名文件（例如这里的 `etc/inittab` 覆盖 buildroot 自带的）。
- 当前是 **2 分区**（boot vfat + rootfs ext4）。如需 RK 那种独立 `userdata`/`oem`/`app` 分区，需扩展 `build_rootfs` 的分区表（暂未做）。

## 当前 overlay 内容
- `etc/inittab` —— 启动后在 console 直接拉起 `/bin/sh`（无登录），`::sysinit` 跑 rcS
- `etc/init.d/rcS` —— `mount -a` + `mdev -s` 生成设备节点
- `etc/fstab`、`etc/profile`
