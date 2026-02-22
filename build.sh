#!/bin/bash
set -e

# ==============================================================================
# Configuration
# ==============================================================================
SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
OUTPUT_DIR="${SHELL_FOLDER}/output"
# JOBS=$(nproc)
JOBS=24


# Toolchains
GLIB_ELF_CROSS_COMPILE_DIR=/opt/gcc15-riscv64-unknown-linux-gnu
GLIB_ELF_CROSS_PREFIX="${GLIB_ELF_CROSS_COMPILE_DIR}/bin/riscv64-unknown-linux-gnu"

# 低版本的交叉编译工具链，适配低版本的linux内核,千万不要用
# GLIB_ELF_CROSS_COMPILE_DIR=/opt/riscv64-lp64d--glibc--bleeding-edge-2021.11-1
# GLIB_ELF_CROSS_PREFIX="${GLIB_ELF_CROSS_COMPILE_DIR}/bin/riscv64-linux"

NEWLIB_ELF_CROSS_COMPILE_DIR=/opt/gcc-riscv64-unknown-elf
NEWLIB_ELF_CROSS_PREFIX="${NEWLIB_ELF_CROSS_COMPILE_DIR}/bin/riscv64-unknown-elf"

# Project Paths
QEMU_DIR="${SHELL_FOLDER}/qemu-8.0.2"
BOOT_DIR="${SHELL_FOLDER}/boot"
OPENSBI_DIR="${SHELL_FOLDER}/opensbi-1.2"
UBOOT_DIR="${SHELL_FOLDER}/u-boot-2026.01"
KERNEL_DIR="${SHELL_FOLDER}/linux"
BUSYBOX_DIR="${SHELL_FOLDER}/busybox-1.36.1"
FREERTOS_DIR="${SHELL_FOLDER}/trusted_domain"
DTS_DIR="${SHELL_FOLDER}/dts"
BUSYBOX_ROOT_SCRIPT="${SHELL_FOLDER}/busybox_root_script"

# ==============================================================================
# Helper Functions
# ==============================================================================

log_step() {
    local component=$1
    local mode_str=$(echo "$BUILD_MODE" | tr '[:lower:]' '[:upper:]')
    echo -e "\033[36m\n------------------------- [${mode_str}] ${component} -------------------------\033[0m"
}

log_info() {
    echo -e "\033[32m[INFO] $1\033[0m"
}

log_error() {
    echo -e "\033[31m[ERROR] $1\033[0m"
}

check_dir() {
    if [ ! -d "$1" ]; then
        mkdir -p "$1"
    fi
}

# ==============================================================================
# Build Functions
# ==============================================================================

build_qemu() {
    log_step "Building QEMU"
    check_dir "${OUTPUT_DIR}/qemu"
    
    cd "${QEMU_DIR}"
    if [ "${BUILD_MODE}" == "clean" ]; then
        if [ -d build ]; then rm -rf build; fi 
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        if [ -d build ]; then rm -rf build; fi
    fi

    if [ ! -f "build/config-host.mak" ]; then
        ./configure --prefix="${OUTPUT_DIR}/qemu" --target-list=riscv64-softmmu --enable-gtk --enable-virtfs --disable-gio
    fi
    make -j"${JOBS}"
    sudo make install
}

build_lowlevelboot() {
    log_step "Building LowLevelBoot"
    check_dir "${OUTPUT_DIR}/lowlevelboot"
    
    cd "${BOOT_DIR}"
    if [ "${BUILD_MODE}" == "clean" ]; then
        rm -f *.o *.elf *.bin *.lst *.map
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        rm -f *.o *.elf *.bin *.lst *.map
    fi

    "${GLIB_ELF_CROSS_PREFIX}-gcc" -x assembler-with-cpp -c start.s -o "${OUTPUT_DIR}/lowlevelboot/start.o"
    "${GLIB_ELF_CROSS_PREFIX}-gcc" -nostartfiles -T./boot.lds -Wl,-Map="${OUTPUT_DIR}/lowlevelboot/lowlevel_fw.map" -Wl,--gc-sections "${OUTPUT_DIR}/lowlevelboot/start.o" -o "${OUTPUT_DIR}/lowlevelboot/lowlevel_fw.elf"
    "${GLIB_ELF_CROSS_PREFIX}-objcopy" -O binary -S "${OUTPUT_DIR}/lowlevelboot/lowlevel_fw.elf" "${OUTPUT_DIR}/lowlevelboot/lowlevel_fw.bin"
    "${GLIB_ELF_CROSS_PREFIX}-objdump" --source --demangle --disassemble --reloc --wide "${OUTPUT_DIR}/lowlevelboot/lowlevel_fw.elf" > "${OUTPUT_DIR}/lowlevelboot/lowlevel_fw.lst"
}

build_opensbi() {
    log_step "Building OpenSBI"
    check_dir "${OUTPUT_DIR}/opensbi"
    
    cd "${OPENSBI_DIR}"
    if [ "${BUILD_MODE}" == "clean" ]; then
        make PLATFORM=quard_star distclean
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        make PLATFORM=quard_star distclean
    fi

    make CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" PLATFORM=quard_star FW_TEXT_START=0xBFF80000 FW_JUMP=y FW_JUMP_ADDR=0xB0200000
    
    cp "${OPENSBI_DIR}/build/platform/quard_star/firmware/fw_jump.bin" "${OUTPUT_DIR}/opensbi/fw_jump.bin"
    cp "${OPENSBI_DIR}/build/platform/quard_star/firmware/fw_jump.elf" "${OUTPUT_DIR}/opensbi/fw_jump.elf"
    "${GLIB_ELF_CROSS_PREFIX}-objdump" --source --demangle --disassemble --reloc --wide "${OUTPUT_DIR}/opensbi/fw_jump.elf" > "${OUTPUT_DIR}/opensbi/fw_jump.lst"
    
    # Generate DTB
    cd "${DTS_DIR}"
    dtc -I dts -O dtb -o "${OUTPUT_DIR}/opensbi/quard_star_sbi.dtb" quard_star_sbi.dts
}

build_uboot() {
    log_step "Building U-Boot"
    check_dir "${OUTPUT_DIR}/uboot"
    
    cd "${UBOOT_DIR}"
    if [ "${BUILD_MODE}" == "clean" ]; then
        make distclean
        rm -f u-boot u-boot.map u-boot.bin
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        make distclean
        rm -f u-boot u-boot.map u-boot.bin
    fi
    
    if [ ! -f ".config" ]; then
        make CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" qemu-quard-star_defconfig
    fi
    
    make CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" -j"${JOBS}"
    
    cp u-boot "${OUTPUT_DIR}/uboot/u-boot.elf"
    cp u-boot.map "${OUTPUT_DIR}/uboot/u-boot.map"
    cp u-boot.bin "${OUTPUT_DIR}/uboot/u-boot.bin"
    "${GLIB_ELF_CROSS_PREFIX}-objdump" --source --demangle --disassemble --reloc --wide "${OUTPUT_DIR}/uboot/u-boot.elf" > "${OUTPUT_DIR}/uboot/u-boot.lst"

    # Generate U-Boot DTB
    cd "${DTS_DIR}"
    dtc -I dts -O dtb -o "${OUTPUT_DIR}/uboot/quard_star_uboot.dtb" quard_star_uboot.dts
}

build_kernel() {
    log_step "Building Linux Kernel"
    check_dir "${OUTPUT_DIR}/linux_kernel"
    
    cd "${KERNEL_DIR}"
    if [ "${BUILD_MODE}" == "clean" ]; then
        make distclean
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        make distclean
    fi

    # if [ ! -f ".config" ]; then
    make ARCH=riscv CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" defconfig 
    # fi
    
    make ARCH=riscv CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" -j"${JOBS}"
    
    cp arch/riscv/boot/Image "${OUTPUT_DIR}/linux_kernel/Image"
}

build_busybox() {
    log_step "Building BusyBox"
    check_dir "${OUTPUT_DIR}/busybox"
    
    cd "${BUSYBOX_DIR}"
    if [ "${BUILD_MODE}" == "clean" ]; then
        make distclean
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        make distclean
    fi
    
    unset srctree objtree VPATH
    
    if [ ! -f ".config" ]; then
        make ARCH=riscv CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" quard_star_defconfig
    fi
    
    make ARCH=riscv CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" -j"${JOBS}"
    make ARCH=riscv CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" CONFIG_PREFIX="${OUTPUT_DIR}/busybox" install
    
    mkdir -p "${OUTPUT_DIR}/busybox/proc"
    mkdir -p "${OUTPUT_DIR}/busybox/sys"
    mkdir -p "${OUTPUT_DIR}/busybox/dev"
    mkdir -p "${OUTPUT_DIR}/busybox/tmp"
}

build_freertos() {
    log_step "Building FreeRTOS (Trusted Domain)"
    check_dir "${OUTPUT_DIR}/trusted_domain"
    
    cd "${FREERTOS_DIR}"
    if [ "${BUILD_MODE}" == "clean" ]; then
        make clean
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        make clean
    fi
    
    make CROSS_COMPILE="${NEWLIB_ELF_CROSS_PREFIX}-"
    
    cp "${FREERTOS_DIR}/build/trusted_fw.bin" "${OUTPUT_DIR}/trusted_domain/"
    cp "${FREERTOS_DIR}/build/trusted_fw.elf" "${OUTPUT_DIR}/trusted_domain/"
}

pack_firmware() {
    log_step "Packing Firmware (fw.bin)"
    check_dir "${OUTPUT_DIR}/fw"
    cd "${OUTPUT_DIR}/fw"
    
    rm -rf fw.bin
    dd if=/dev/zero of=fw.bin bs=1k count=32k status=none
    
    dd of=fw.bin bs=1k conv=notrunc seek=0 if="${OUTPUT_DIR}/lowlevelboot/lowlevel_fw.bin" status=none
    dd of=fw.bin bs=1k conv=notrunc seek=512 if="${OUTPUT_DIR}/opensbi/quard_star_sbi.dtb" status=none
    dd of=fw.bin bs=1k conv=notrunc seek=1024 if="${OUTPUT_DIR}/uboot/quard_star_uboot.dtb" status=none
    dd of=fw.bin bs=1k conv=notrunc seek=2048 if="${OUTPUT_DIR}/opensbi/fw_jump.bin" status=none
    dd of=fw.bin bs=1k conv=notrunc seek=4096 if="${OUTPUT_DIR}/trusted_domain/trusted_fw.bin" status=none
    dd of=fw.bin bs=1k conv=notrunc seek=8192 if="${OUTPUT_DIR}/uboot/u-boot.bin" status=none
    
    log_info "Firmware packed at ${OUTPUT_DIR}/fw/fw.bin"
}

build_rootfs() {
    log_step "Building Root Filesystem Image"
    
    if [ "$(id -u)" != "0" ]; then
        if ! sudo -n true 2>/dev/null; then
            log_error "Root privileges required for rootfs generation, but sudo requires password."
            exit 1
        fi
        sudo -v
    fi

    ROOTFS_DIR="${OUTPUT_DIR}/rootfs"
    check_dir "${ROOTFS_DIR}"
    check_dir "${ROOTFS_DIR}/rootfs"
    check_dir "${ROOTFS_DIR}/bootfs"
    
    IMG_FILE="${ROOTFS_DIR}/rootfs.img"
    
    # 执行清除逻辑
    if [ "${BUILD_MODE}" == "clean" ]; then
        rm -f "${IMG_FILE}"
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        rm -f "${IMG_FILE}"
    fi
    
    # 同步最新文件到临时目录
    cp "${OUTPUT_DIR}/linux_kernel/Image" "${ROOTFS_DIR}/bootfs/Image"
    cp "${OUTPUT_DIR}/uboot/quard_star_uboot.dtb" "${ROOTFS_DIR}/bootfs/quard_star.dtb"
    cp -r "${OUTPUT_DIR}/busybox/"* "${ROOTFS_DIR}/rootfs/"
    if [ -d "${BUSYBOX_ROOT_SCRIPT}" ]; then
        cp -r "${BUSYBOX_ROOT_SCRIPT}/"* "${ROOTFS_DIR}/rootfs/"
    fi
    
    "${UBOOT_DIR}/tools/mkimage" -A riscv -O linux -T script -C none -a 0 -e 0 -n "Distro Boot Script" -d "${DTS_DIR}/quard_star_uboot.cmd" "${ROOTFS_DIR}/bootfs/boot.scr"
    
    # 核心判断：是否需要全量格式化磁盘
    local FORMAT_NEEDED=0
    if [ ! -f "${IMG_FILE}" ]; then
        FORMAT_NEEDED=1
        log_info "Creating new image file (1GB)..."
        dd if=/dev/zero of="${IMG_FILE}" bs=1M count=1024 status=progress
    fi
    
    LOOP_DEV=$(sudo losetup -f)
    if [ -z "${LOOP_DEV}" ]; then
        log_error "No free loop device found!"
        exit 1
    fi
    
    sudo losetup -P "${LOOP_DEV}" "${IMG_FILE}"
    
    # 如果是新建文件或 Rebuild，则重新分区和格式化
    if [ ${FORMAT_NEEDED} -eq 1 ]; then
        log_info "Partitioning and Formatting ${LOOP_DEV}..."
        sudo sfdisk "${LOOP_DEV}" <<EOF
label: dos
unit: sectors

${LOOP_DEV}p1 : start=        2048, size=      196608, type=c
${LOOP_DEV}p2 : start=      198656, size=     1898496, type=83
EOF
        sudo mkfs.vfat "${LOOP_DEV}p1"
        sudo mkfs.ext4 -F "${LOOP_DEV}p2"  # -F 强制格式化，避免卡在确认提示
    else
        log_info "Image exists. Incremental update, skipping formatting."
    fi
    
    TARGET_DIR="${ROOTFS_DIR}/target"
    mkdir -p "${TARGET_DIR}/bootfs" "${TARGET_DIR}/rootfs"
    
    sudo mount "${LOOP_DEV}p1" "${TARGET_DIR}/bootfs"
    sudo mount "${LOOP_DEV}p2" "${TARGET_DIR}/rootfs"
    
    log_info "Copying files to image partitions..."
    # 使用 cp -ru 仅更新有变动的文件，更符合增量原则
    sudo cp -ru "${ROOTFS_DIR}/bootfs/"* "${TARGET_DIR}/bootfs/"
    sudo cp -ru "${ROOTFS_DIR}/rootfs/"* "${TARGET_DIR}/rootfs/"
    
    sync
    sudo umount "${TARGET_DIR}/bootfs" || true
    sudo umount "${TARGET_DIR}/rootfs" || true
    sudo losetup -d "${LOOP_DEV}"
    
    log_info "Rootfs image generated/updated at ${IMG_FILE}"
}

clean_all() {
    log_step "Cleaning all build artifacts"
    rm -rf "${OUTPUT_DIR}"
    cd "${QEMU_DIR}" && if [ -d build ]; then rm -rf build; fi
    cd "${UBOOT_DIR}" && make distclean
    cd "${KERNEL_DIR}" && make distclean
    cd "${OPENSBI_DIR}" && make distclean
    cd "${BUSYBOX_DIR}" && make distclean
    cd "${FREERTOS_DIR}" && make clean
}

usage() {
    echo "Usage: $0 [target] [mode]"
    echo "Targets:"
    echo "  all        Build everything (default)"
    echo "  clean      Clean all build artifacts and source directories"
    echo "  qemu       Build QEMU"
    echo "  boot       Build LowLevelBoot"
    echo "  opensbi    Build OpenSBI"
    echo "  uboot      Build U-Boot"
    echo "  kernel     Build Linux Kernel"
    echo "  busybox    Build BusyBox"
    echo "  freertos   Build FreeRTOS (Trusted Domain)"
    echo "  rootfs     Build Root Filesystem Image"
    echo "  firmware   Pack Firmware (fw.bin)"
    echo ""
    echo "Modes:"
    echo "  incremental (default)  Build only changed files"
    echo "  clean                  Clean before building"
    echo "  rebuild                Clean then build"
}

# ==============================================================================
# Main Logic
# ==============================================================================

if [ ! -d "${OUTPUT_DIR}" ]; then
    mkdir -p "${OUTPUT_DIR}"
fi

# Argument Parsing
TARGET=$1
if [ -z "$TARGET" ]; then
    TARGET="all"
fi

# 明确判断第2个参数，默认为 incremental
if [ -z "$2" ]; then
    BUILD_MODE="incremental"
elif [ "$2" == "clean" ]; then
    BUILD_MODE="clean"
elif [ "$2" == "rebuild" ]; then
    BUILD_MODE="rebuild"
elif [ "$2" == "incremental" ]; then
    BUILD_MODE="incremental"
else
    echo "Invalid mode: $2"
    usage
    exit 1
fi

case "$TARGET" in
    "all")
        build_qemu
        build_lowlevelboot
        build_opensbi
        build_freertos
        build_uboot
        build_kernel
        build_busybox
        pack_firmware
        build_rootfs
        ;;
    "clean")
        clean_all
        ;;
    "qemu")
        build_qemu
        ;;
    "boot")
        build_lowlevelboot
        pack_firmware
        ;;
    "opensbi")
        build_opensbi
        pack_firmware
        ;;
    "uboot")
        build_uboot
        pack_firmware
        ;;
    "kernel")
        build_kernel
        build_rootfs
        ;;
    "busybox")
        build_busybox
        build_rootfs
        ;;
    "freertos")
        build_freertos
        pack_firmware
        ;;
    "rootfs")
        build_rootfs
        ;;
    "firmware")
        pack_firmware
        ;;
    "help"|*)
        usage
        ;;
esac

echo -e "\n\033[32m[SUCCESS] Done!\033[0m"