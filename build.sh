#!/bin/bash
set -e

# ==============================================================================
# Configuration
# ==============================================================================
# 获取脚本所在的绝对路径，作为整个工程的根基
SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
OUTPUT_DIR="${SHELL_FOLDER}/output"
#JOBS=$(nproc)
JOBS=24

# Toolchains
GLIB_ELF_CROSS_COMPILE_DIR=/opt/gcc15-riscv64-unknown-linux-gnu
GLIB_ELF_CROSS_PREFIX="${GLIB_ELF_CROSS_COMPILE_DIR}/bin/riscv64-unknown-linux-gnu"

NEWLIB_ELF_CROSS_COMPILE_DIR=/opt/gcc-riscv64-unknown-elf
NEWLIB_ELF_CROSS_PREFIX="${NEWLIB_ELF_CROSS_COMPILE_DIR}/bin/riscv64-unknown-elf"

# Project Paths (全部使用绝对路径)
QEMU_DIR="${SHELL_FOLDER}/qemu-8.0.2"
BOOT_DIR="${SHELL_FOLDER}/boot"
OPENSBI_DIR="${SHELL_FOLDER}/opensbi-1.2"
UBOOT_DIR="${SHELL_FOLDER}/u-boot-2026.01"
KERNEL_DIR="${SHELL_FOLDER}/linux"
BUSYBOX_DIR="${SHELL_FOLDER}/busybox-1.36.1"
FREERTOS_DIR="${SHELL_FOLDER}/trusted_domain"
DTS_DIR="${SHELL_FOLDER}/dts"
BUSYBOX_ROOT_SCRIPT="${SHELL_FOLDER}/busybox_root_script"
LINUX_DRIVER_DIR="${SHELL_FOLDER}/linux_driver"
SYSROOT_DIR="${SHELL_FOLDER}/sysroot"

# OpenAMP Paths (新增)
THIRD_PARTY_DIR="${SHELL_FOLDER}/third_party"
LIBMETAL_DIR="${THIRD_PARTY_DIR}/libmetal"
OPENAMP_DIR="${THIRD_PARTY_DIR}/open-amp"
FREERTOS_APP_DIR="${SHELL_FOLDER}/freertos_app"
OPENAMP_ADAPTER_DIR="${FREERTOS_APP_DIR}/openamp_adapter"
BSP_INCLUDE_DIR="${SHELL_FOLDER}/bsp_include"

# FreeRTOS/裸机编译选项（用于 OpenAMP 适配层）
CFLAGS_FREERTOS="-march=rv64imafdcv -mabi=lp64d -mcmodel=medany \
                 -ffunction-sections -fdata-sections \
                 -Wall -Wextra \
                 -I${OPENAMP_ADAPTER_DIR}/include \
                 -I${BSP_INCLUDE_DIR} \
                 -I${LIBMETAL_DIR}/lib/include \
                 -I${OPENAMP_DIR}/lib/include \
                 -I${FREERTOS_DIR} \
                 -I${FREERTOS_DIR}/FreeRTOS-Kernel-v10.4.3/include \
                 -I${FREERTOS_DIR}/FreeRTOS-Kernel-v10.4.3/portable/GCC/RISC-V \
                 -DFREERTOS -O2 -g"

# 采用patch方式管理linux kernel (杜绝 ../ 相对路径，全部换成绝对路径)
KERNEL_VER="v6.10" #v6.10 v5.10
PATCH_FILE="${SHELL_FOLDER}/linux_mypatches/quard_star_linux_${KERNEL_VER}.patch"
CONFIG_FILE="${SHELL_FOLDER}/linux_myconfigs/${KERNEL_VER}_config"
# 用一个隐藏文件记录当前 Linux 目录处于哪个版本
VERSION_MARKER="${SHELL_FOLDER}/.current_kernel_ver"

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
    cd "${SHELL_FOLDER}"  # 强制归位
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
    cd "${SHELL_FOLDER}"
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
    cd "${SHELL_FOLDER}"
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
    
    # Generate DTB (使用括号开启子Shell，隔离目录切换)
    (
        cd "${DTS_DIR}"
        dtc -I dts -O dtb -o "${OUTPUT_DIR}/opensbi/quard_star_sbi.dtb" quard_star_sbi.dts
    )
}

build_uboot() {
    cd "${SHELL_FOLDER}"
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

    # Generate U-Boot DTB (使用子Shell隔离)
    (
        cd "${DTS_DIR}"
        dtc -I dts -O dtb -o "${OUTPUT_DIR}/uboot/quard_star_uboot.dtb" quard_star_uboot.dts
    )
}

build_kernel() {
    cd "${SHELL_FOLDER}"
    log_step "Building Linux Kernel $KERNEL_VER"
    
    if [ ! -d "${KERNEL_DIR}" ]; then
        echo "[INFO] 未检测到源码，正在拉取 Linux..."
        git clone --reference "${SHELL_FOLDER}/linux" -b $KERNEL_VER https://mirrors.tuna.tsinghua.edu.cn/git/linux.git "${KERNEL_DIR}"
    fi

    cd "${KERNEL_DIR}"

    if [ -f "$VERSION_MARKER" ]; then
        CURRENT_STATE=$(cat "$VERSION_MARKER")
    else
        CURRENT_STATE="none"
    fi

    if [ "$CURRENT_STATE" != "$KERNEL_VER" ]; then
        echo "[INFO] 检测到版本切换或首次编译 ($CURRENT_STATE -> $KERNEL_VER)。准备大扫除..."
        
        git reset --hard HEAD
        git clean -fdx
        make mrproper
        
        git checkout $KERNEL_VER
        
        if [ -f "$PATCH_FILE" ]; then
            echo "[INFO] 正在应用补丁: $PATCH_FILE"
            patch -p1 < "$PATCH_FILE"
        else
            echo "[WARN] 未找到补丁文件，将使用原生纯净源码。"
        fi

        if [ -f "$CONFIG_FILE" ]; then
            echo "[INFO] 载入自定义配置文件: $CONFIG_FILE"
            cp "$CONFIG_FILE" .config
        else
            echo "[WARN] 未找到自定义配置 ($CONFIG_FILE)。"
            echo "[INFO] 正在自动生成 RISC-V 原生默认配置 (defconfig)..."
            make ARCH=riscv CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" defconfig
            mkdir -p "$(dirname "$CONFIG_FILE")"
            cp .config "$CONFIG_FILE"
        fi
        
        echo "$KERNEL_VER" > "$VERSION_MARKER"
    else
        echo "[INFO] 源码版本 ($KERNEL_VER) 无变化，跳过补丁应用，准备增量编译..."
    fi

    make ARCH=riscv CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" -j"${JOBS}"

    # 复制编译产物到 output 目录
    log_info "Copying kernel Image to output directory..."
    mkdir -p "${OUTPUT_DIR}/linux_kernel"
    cp arch/riscv/boot/Image "${OUTPUT_DIR}/linux_kernel/Image"
    if [ -f "arch/riscv/boot/Image.gz" ]; then
        cp arch/riscv/boot/Image.gz "${OUTPUT_DIR}/linux_kernel/Image.gz"
    fi
    if [ -f "vmlinux" ]; then
        cp vmlinux "${OUTPUT_DIR}/linux_kernel/vmlinux"
    fi
    log_info "Kernel Image copied to ${OUTPUT_DIR}/linux_kernel/Image"
}

build_busybox() {
    cd "${SHELL_FOLDER}"
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
    cd "${SHELL_FOLDER}"
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

# ==============================================================================
# OpenAMP Build Functions (新增)
# ==============================================================================

setup_submodules() {
    cd "${SHELL_FOLDER}"
    log_step "Initializing Git Submodules"

    # 检查是否已经通过 git clone 获取了代码
    if [ -d "${LIBMETAL_DIR}/.git" ] && [ -d "${OPENAMP_DIR}/.git" ]; then
        log_info "Git repositories already present"
        return 0
    fi

    # 检查是否有 tar.gz 文件（手动下载）
    if [ -f "${THIRD_PARTY_DIR}/libmetal-1.25.0.tar.gz" ] && \
       [ -f "${THIRD_PARTY_DIR}/open-amp-1.25.0.tar.gz" ]; then
        log_info "Found tar.gz files, extracting..."
        cd "${THIRD_PARTY_DIR}"
        tar xf libmetal-1.25.0.tar.gz
        [ -d libmetal-1.25.0 ] && mv libmetal-1.25.0 libmetal
        tar xf open-amp-1.25.0.tar.gz
        [ -d open-amp-1.25.0 ] && mv open-amp-1.25.0 open-amp
        cd "${SHELL_FOLDER}"
        log_info "Extraction complete"
        return 0
    fi

    # 添加 submodules（如果不存在）
    if [ ! -d "${LIBMETAL_DIR}" ]; then
        log_info "Adding libmetal submodule..."
        git submodule add https://github.com/OpenAMP/libmetal.git "${LIBMETAL_DIR}"
    fi

    if [ ! -d "${OPENAMP_DIR}" ]; then
        log_info "Adding open-amp submodule..."
        git submodule add https://github.com/OpenAMP/open-amp.git "${OPENAMP_DIR}"
    fi

    # 更新 submodules
    git submodule update --init --recursive

    log_info "Submodules initialized successfully (libmetal v$(cat ${LIBMETAL_DIR}/VERSION | grep VERSION_MINOR | cut -d= -f2 | tr -d ' ').$(cat ${LIBMETAL_DIR}/VERSION | grep VERSION_PATCH | cut -d= -f2 | tr -d ' '), open-amp v$(cat ${OPENAMP_DIR}/VERSION | grep VERSION_MINOR | cut -d= -f2 | tr -d ' ').$(cat ${OPENAMP_DIR}/VERSION | grep VERSION_PATCH | cut -d= -f2 | tr -d ' '))"
}

build_libmetal() {
    cd "${SHELL_FOLDER}"
    log_step "Building libmetal"
    check_dir "${OUTPUT_DIR}/libmetal"

    # 确保 submodule 已初始化
    if [ ! -d "${LIBMETAL_DIR}/.git" ]; then
        log_error "libmetal submodule not initialized. Run: $0 submodules"
        exit 1
    fi

    cd "${LIBMETAL_DIR}"

    if [ "${BUILD_MODE}" == "clean" ]; then
        rm -rf build
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        rm -rf build
    fi

    # 创建构建目录
    mkdir -p build
    cd build

    # 设置编译器路径
    export PATH="/opt/gcc-riscv64-unknown-elf/bin:$PATH"

    # 使用 CMake 构建（FreeRTOS 配置）
    # 注意：暂时不编译 FreeRTOS system 层，等后续移植
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE=../cmake/platforms/cross-generic-gcc.cmake \
        -DCROSS_PREFIX=riscv64-unknown-elf- \
        -DCMAKE_INSTALL_PREFIX="$(pwd)/../lib" \
        -DCMAKE_C_FLAGS="-I${SHELL_FOLDER}/${OPENAMP_ADAPTER_DIR}/include -I${BSP_INCLUDE_DIR} -DMETAL_MAX_DEVICE_REGIONS=1" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=OFF \
        -DMETAL_BUILD_SHARED_LIBS=OFF \
        -DWITH_FREERTOS_LIB=OFF

    make -j"${JOBS}"
    make install

    log_info "libmetal built: ${LIBMETAL_DIR}/lib/libmetal.a"
}

build_openamp_lib() {
    cd "${SHELL_FOLDER}"
    log_step "Building open-amp library"
    check_dir "${OUTPUT_DIR}/openamp"

    # 确保 libmetal 已构建
    if [ ! -f "${LIBMETAL_DIR}/lib/libmetal.a" ]; then
        log_info "libmetal not found, building it first..."
        build_libmetal
    fi

    # 确保 submodule 已初始化
    if [ ! -d "${OPENAMP_DIR}/.git" ]; then
        log_error "open-amp submodule not initialized. Run: $0 submodules"
        exit 1
    fi

    cd "${OPENAMP_DIR}"

    if [ "${BUILD_MODE}" == "clean" ]; then
        rm -rf cmake/build
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        rm -rf cmake/build
    fi

    # 创建构建目录
    mkdir -p cmake/build
    cd cmake/build

    # 设置编译器路径
    export PATH="/opt/gcc-riscv64-unknown-elf/bin:$PATH"

    # 使用 CMake 构建 - 使用 libmetal 的 generic 工具链
    cmake ../.. \
        -DCMAKE_TOOLCHAIN_FILE=${LIBMETAL_DIR}/cmake/platforms/cross-generic-gcc.cmake \
        -DCROSS_PREFIX=riscv64-unknown-elf- \
        -DCMAKE_INSTALL_PREFIX="${OPENAMP_DIR}/lib" \
        -DCMAKE_C_FLAGS="-I${OPENAMP_ADAPTER_DIR}/include -I${BSP_INCLUDE_DIR} -I${LIBMETAL_DIR}/lib/include -DFREERTOS" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_FIND_ROOT_PATH="${LIBMETAL_DIR}/lib" \
        -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH \
        -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH \
        -DWITH_LIBMETAL_FIND=OFF

    make -j"${JOBS}"

    log_info "open-amp built: ${OPENAMP_DIR}/cmake/build/libopenamp.a"
}

build_openamp_adapter() {
    cd "${SHELL_FOLDER}"
    log_step "Building OpenAMP Adapter Layer"
    check_dir "${OUTPUT_DIR}/openamp_adapter"

    # 确保依赖库已构建
    if [ ! -f "${LIBMETAL_DIR}/lib/libmetal.a" ]; then
        log_info "libmetal not found, building it first..."
        build_libmetal
    fi

    if [ ! -f "${OPENAMP_DIR}/cmake/build/libopenamp.a" ]; then
        log_info "open-amp not found, building it first..."
        build_openamp_lib
    fi

    cd "${FREERTOS_APP_DIR}"

    if [ "${BUILD_MODE}" == "clean" ]; then
        make clean
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        make clean
    fi

    # 构建 OpenAMP 适配层（编译 libmetal 和 openamp 适配代码）
    log_info "Compiling OpenAMP adapter layer..."

    # 创建输出目录
    mkdir -p build/openamp_adapter/libmetal
    mkdir -p build/openamp_adapter/openamp
    mkdir -p lib

    # 编译 libmetal 适配层
    for src in openamp_adapter/libmetal/*.c; do
        if [ -f "$src" ]; then
            obj="build/$(basename "$src" .c).o"
            log_info "Compiling $src"
            ${NEWLIB_ELF_CROSS_PREFIX}-gcc ${CFLAGS_FREERTOS} -c "$src" -o "$obj"
        fi
    done

    # 编译 openamp 适配层
    for src in openamp_adapter/openamp/*.c; do
        if [ -f "$src" ]; then
            obj="build/$(basename "$src" .c).o"
            log_info "Compiling $src"
            ${NEWLIB_ELF_CROSS_PREFIX}-gcc ${CFLAGS_FREERTOS} -c "$src" -o "$obj"
        fi
    done

    # 打包成静态库
    ${NEWLIB_ELF_CROSS_PREFIX}-ar rcs lib/libopenamp_adapter.a build/*.o 2>/dev/null || true

    log_info "OpenAMP adapter built: ${FREERTOS_APP_DIR}/lib/libopenamp_adapter.a"
}

build_freertos_app() {
    cd "${SHELL_FOLDER}"
    log_step "Building FreeRTOS Application with OpenAMP"
    check_dir "${OUTPUT_DIR}/freertos_app"

    # 确保所有依赖已构建
    if [ ! -f "${FREERTOS_APP_DIR}/lib/libopenamp_adapter.a" ]; then
        log_info "OpenAMP adapter not found, building it first..."
        build_openamp_adapter
    fi

    cd "${FREERTOS_APP_DIR}"

    if [ "${BUILD_MODE}" == "clean" ]; then
        make clean
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        make clean
    fi

    # 使用 freertos_app 的 Makefile
    make CROSS_COMPILE="${NEWLIB_ELF_CROSS_PREFIX}-"

    # 拷贝输出
    cp build/freertos_app.elf "${OUTPUT_DIR}/freertos_app/" 2>/dev/null || true
    cp build/freertos_app.bin "${OUTPUT_DIR}/freertos_app/" 2>/dev/null || true

    log_info "FreeRTOS application built: ${OUTPUT_DIR}/freertos_app/"
}

build_driver() {
    cd "${SHELL_FOLDER}"
    log_step "Building Linux Drivers"
    check_dir "${OUTPUT_DIR}/linux_driver"

    # 遍历 linux_driver 下的所有子目录（包含 Makefile 的）
    for driver_dir in "${LINUX_DRIVER_DIR}"/*/; do
        if [ -f "${driver_dir}/Makefile" ]; then
            driver_name=$(basename "${driver_dir}")
            log_info "Building driver: ${driver_name}"

            cd "${driver_dir}"

            if [ "${BUILD_MODE}" == "clean" ]; then
                make clean KERNELDIR="${KERNEL_DIR}" CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-"
            elif [ "${BUILD_MODE}" == "rebuild" ]; then
                make clean KERNELDIR="${KERNEL_DIR}" CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-"
            fi

            # 编译
            make KERNELDIR="${KERNEL_DIR}" CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-"

            # 拷贝编译产物到 output/linux_driver/
            # 支持两种情况：.ko 在当前目录或在 build/ 子目录
            find . -name "*.ko" -exec cp {} "${OUTPUT_DIR}/linux_driver/" \; 2>/dev/null || true

            cd "${SHELL_FOLDER}"
        fi
    done

    log_info "Driver modules built in ${OUTPUT_DIR}/linux_driver/"
}

pack_firmware() {
    cd "${SHELL_FOLDER}"
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
    cd "${SHELL_FOLDER}"
    log_step "Building Root Filesystem Image"

    if [ "$(id -u)" != "0" ]; then
        log_info "需要 root 权限来生成根文件系统，请稍后输入密码..."
        if ! sudo -v; then
            log_error "提权失败或已取消，脚本退出。"
            exit 1
        fi
    fi

    ROOTFS_DIR="${OUTPUT_DIR}/rootfs"
    check_dir "${ROOTFS_DIR}"
    check_dir "${ROOTFS_DIR}/rootfs"
    check_dir "${ROOTFS_DIR}/bootfs"

    IMG_FILE="${ROOTFS_DIR}/rootfs.img"

    if [ "${BUILD_MODE}" == "clean" ]; then
        rm -f "${IMG_FILE}"
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        rm -f "${IMG_FILE}"
    fi

    # 1. 拷贝 busybox 安装结果
    cp -r "${OUTPUT_DIR}/busybox/"* "${ROOTFS_DIR}/rootfs/"

    # 2. 拷贝 busybox 配置脚本
    if [ -d "${BUSYBOX_ROOT_SCRIPT}" ]; then
        cp -r "${BUSYBOX_ROOT_SCRIPT}/"* "${ROOTFS_DIR}/rootfs/"
    fi

    # 3. 拷贝驱动模块到 rootfs/driver/
    mkdir -p "${ROOTFS_DIR}/rootfs/driver"
    if [ -d "${OUTPUT_DIR}/linux_driver" ] && [ "$(ls -A ${OUTPUT_DIR}/linux_driver/*.ko 2>/dev/null)" ]; then
        cp -r "${OUTPUT_DIR}/linux_driver/"*.ko "${ROOTFS_DIR}/rootfs/driver/"
        log_info "Copied $(ls ${OUTPUT_DIR}/linux_driver/*.ko 2>/dev/null | wc -l) driver module(s) to rootfs"
    fi

    # 4. 拷贝内核和 DTB 到 bootfs
    cp "${OUTPUT_DIR}/linux_kernel/Image" "${ROOTFS_DIR}/bootfs/Image"
    cp "${OUTPUT_DIR}/uboot/quard_star_uboot.dtb" "${ROOTFS_DIR}/bootfs/quard_star.dtb"

    # 5. 生成 U-Boot 启动脚本
    "${UBOOT_DIR}/tools/mkimage" -A riscv -O linux -T script -C none -a 0 -e 0 -n "Distro Boot Script" -d "${DTS_DIR}/quard_star_uboot.cmd" "${ROOTFS_DIR}/bootfs/boot.scr"
    
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
    
    if [ ${FORMAT_NEEDED} -eq 1 ]; then
        log_info "Partitioning and Formatting ${LOOP_DEV}..."
        sudo sfdisk "${LOOP_DEV}" <<EOF
label: dos
unit: sectors

${LOOP_DEV}p1 : start=        2048, size=      196608, type=c
${LOOP_DEV}p2 : start=      198656, size=     1898496, type=83
EOF
        sudo mkfs.vfat "${LOOP_DEV}p1"
        sudo mkfs.ext4 -F "${LOOP_DEV}p2"
    else
        log_info "Image exists. Incremental update, skipping formatting."
    fi
    
    TARGET_DIR="${ROOTFS_DIR}/target"
    mkdir -p "${TARGET_DIR}/bootfs" "${TARGET_DIR}/rootfs"
    
    sudo mount "${LOOP_DEV}p1" "${TARGET_DIR}/bootfs"
    sudo mount "${LOOP_DEV}p2" "${TARGET_DIR}/rootfs"
    
    log_info "Copying files to image partitions..."
    sudo cp -ru "${ROOTFS_DIR}/bootfs/"* "${TARGET_DIR}/bootfs/"
    sudo cp -ru "${ROOTFS_DIR}/rootfs/"* "${TARGET_DIR}/rootfs/"
    
    sync
    sudo umount "${TARGET_DIR}/bootfs" || true
    sudo umount "${TARGET_DIR}/rootfs" || true
    sudo losetup -d "${LOOP_DEV}"

    # 拷贝完整 rootfs 到 sysroot，供应用层开发和调试使用
    log_info "Copying complete rootfs to sysroot for application development..."
    rm -rf "${SYSROOT_DIR}"
    cp -r "${ROOTFS_DIR}/rootfs" "${SYSROOT_DIR}"

    log_info "Rootfs image: ${IMG_FILE}"
    log_info "Complete rootfs (for app dev): ${SYSROOT_DIR}/"
}

clean_all() {
    cd "${SHELL_FOLDER}"
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
    echo "  all             Build everything (default)"
    echo "  clean           Clean all build artifacts and source directories"
    echo "  qemu            Build QEMU"
    echo "  boot            Build LowLevelBoot"
    echo "  opensbi         Build OpenSBI"
    echo "  uboot           Build U-Boot"
    echo "  kernel          Build Linux Kernel"
    echo "  busybox         Build BusyBox"
    echo "  freertos        Build FreeRTOS (Trusted Domain)"
    echo "  freertos-app    Build FreeRTOS Application with OpenAMP"
    echo "  driver          Build Linux Drivers (out-of-tree)"
    echo "  rootfs          Build Root Filesystem Image"
    echo "  firmware        Pack Firmware (fw.bin)"
    echo ""
    echo "  OpenAMP-specific targets:"
    echo "  submodules      Initialize Git submodules (libmetal, open-amp)"
    echo "  libmetal        Build libmetal library"
    echo "  openamp         Build open-amp library"
    echo "  openamp-adapter Build OpenAMP adapter layer"
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

TARGET=$1
if [ -z "$TARGET" ]; then
    TARGET="all"
fi

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
        build_driver
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
    "freertos-app")
        build_freertos_app
        ;;
    "driver")
        build_driver
        ;;
    "rootfs")
        build_rootfs
        ;;
    "firmware")
        pack_firmware
        ;;
    # OpenAMP 相关目标
    "submodules")
        setup_submodules
        ;;
    "libmetal")
        build_libmetal
        ;;
    "openamp")
        build_openamp_lib
        ;;
    "openamp-adapter")
        build_openamp_adapter
        ;;
    "help"|*)
        usage
        ;;
esac

echo -e "\n\033[32m[SUCCESS] Done!\033[0m"