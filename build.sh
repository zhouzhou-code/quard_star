#!/bin/bash
set -eE   # -E: 让 ERR trap 在函数内部也生效

# ==============================================================================
# Configuration
# ==============================================================================
# 获取脚本所在的绝对路径，作为整个工程的根基
SHELL_FOLDER=$(cd "$(dirname "$0")";pwd)
SDK_ROOT_DIR="${SHELL_FOLDER}"

# 载入板级/构建配置（所有路径、工具链、内核版本、固件打包布局都在这里）
BOARD_CONFIG="${SHELL_FOLDER}/board.config"
if [ ! -f "${BOARD_CONFIG}" ]; then
    echo -e "\033[31m[ERROR] 未找到配置文件: ${BOARD_CONFIG}\033[0m"
    exit 1
fi
source "${BOARD_CONFIG}"

# ==============================================================================
# Helper Functions
# ==============================================================================

# ---- 彩色输出 ----
C_NORMAL="\033[0m"; C_RED="\033[31m"; C_GREEN="\033[32m"; C_YELLOW="\033[33m"; C_CYAN="\033[36m"

msg_info()  { echo -e "${C_GREEN}[build.sh:info] $1${C_NORMAL}"; }
msg_warn()  { echo -e "${C_YELLOW}[build.sh:warn] $1${C_NORMAL}"; }
msg_error() { echo -e "${C_RED}[build.sh:error] $1${C_NORMAL}"; }

# 兼容旧调用名
log_info()  { msg_info "$1"; }
log_warn()  { msg_warn "$1"; }
log_error() { msg_error "$1"; }

log_step() {
    local component=$1
    local mode_str=$(echo "$BUILD_MODE" | tr '[:lower:]' '[:upper:]')
    echo -e "${C_CYAN}\n------------------------- [${mode_str}] ${component} -------------------------${C_NORMAL}"
}

# 出错时打印「哪个函数 / 哪一行 / 哪条命令」失败（配合 set -eE）
err_handler() {
    local ret=$?
    [ "$ret" -eq 0 ] && return
    msg_error "构建失败 (退出码 $ret)"
    msg_error "  函数: ${FUNCNAME[1]:-main}"
    msg_error "  位置: ${BASH_SOURCE[1]:-$0}:${BASH_LINENO[0]}"
    msg_error "  命令: ${BASH_COMMAND}"
    exit "$ret"
}
trap 'err_handler' ERR

finish_build() {
    msg_info "${FUNCNAME[1]} 完成。"
}

check_dir() {
    if [ ! -d "$1" ]; then
        mkdir -p "$1"
    fi
}

# 检测主机构建依赖（清单: project/scripts/build-depend-tools.txt）
build_check() {
    log_step "Checking host build dependencies"
    local deps_file="${PROJECT_DIR}/scripts/build-depend-tools.txt"
    if [ ! -f "$deps_file" ]; then
        msg_warn "依赖清单不存在: $deps_file，跳过检测"
        return 0
    fi
    local missing="" line cmd pkg
    while IFS= read -r line; do
        line="${line%%#*}"
        line="$(echo "$line" | sed -e 's/^[[:space:]]*//' -e 's/[[:space:]]*$//')"
        [ -z "$line" ] && continue
        cmd="${line%,*}"
        pkg="${line##*,}"
        if eval "$cmd" >/dev/null 2>&1; then
            echo -e "  ${C_GREEN}[OK]${C_NORMAL}  $cmd"
        else
            echo -e "  ${C_RED}[缺]${C_NORMAL}  $cmd  ->  $pkg"
            case " $missing " in *" $pkg "*) ;; *) missing="$missing $pkg";; esac
        fi
    done < "$deps_file"
    if [ -n "$missing" ]; then
        msg_error "缺少主机依赖，请先安装："
        echo -e "    ${C_YELLOW}sudo apt install -y$missing${C_NORMAL}"
        return 1
    fi
    msg_info "主机依赖检测通过"
}

# 检测交叉工具链是否就位且可运行（捕获 libmpc 那类缺运行库的情况）
check_toolchain() {
    local ok=1 pfx gcc
    for pfx in "${GLIB_ELF_CROSS_PREFIX}" "${NEWLIB_ELF_CROSS_PREFIX}"; do
        gcc="${pfx}-gcc"
        if [ ! -x "$gcc" ]; then
            msg_error "未找到交叉编译器: $gcc"
            msg_info  "请把工具链放到 ${TOOLCHAIN_DIR}/ 下，或设置 TOOLCHAIN_DIR 环境变量"
            ok=0; continue
        fi
        if ! "$gcc" --version >/dev/null 2>&1; then
            msg_error "交叉编译器存在但无法运行: $gcc"
            msg_info  "通常是缺少运行库，常见: sudo apt install -y libmpc3"
            "$gcc" --version 2>&1 | head -1 | sed 's/^/        /'
            ok=0
        fi
    done
    [ "$ok" -eq 1 ] || return 1
    msg_info "工具链检测通过 (gcc $(${GLIB_ELF_CROSS_PREFIX}-gcc -dumpversion 2>/dev/null))"
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
        git clone -b $KERNEL_VER https://mirrors.tuna.tsinghua.edu.cn/git/linux.git "${KERNEL_DIR}"
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


build_freertos() {
    cd "${SHELL_FOLDER}"
    log_step "Building FreeRTOS (Trusted Domain)"
    check_dir "${OUTPUT_DIR}/trusted_domain"

    cd "${TRUSTED_DOMAIN_DIR}"
    if [ "${BUILD_MODE}" == "clean" ]; then
        make clean
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        make clean
    fi

    # 检查是否需要链接 OpenAMP 库
    if [ -f "${LIBMETAL_DIR}/lib/libmetal.a" ] && [ -f "${OPENAMP_DIR}/cmake/build/libopenamp.a" ]; then
        log_info "Building FreeRTOS with OpenAMP support..."
        # 更新 Makefile 中的库路径
        make CROSS_COMPILE="${NEWLIB_ELF_CROSS_PREFIX}-" \
             WITH_OPENAMP=1 \
             LIBMETAL_LIB="${LIBMETAL_DIR}/lib/libmetal.a" \
             OPENAMP_LIB="${OPENAMP_DIR}/cmake/build/libopenamp.a"
    else
        log_info "Building FreeRTOS without OpenAMP (libraries not found)..."
        make CROSS_COMPILE="${NEWLIB_ELF_CROSS_PREFIX}-"
    fi

    cp "${TRUSTED_DOMAIN_DIR}/build/trusted_fw.bin" "${OUTPUT_DIR}/trusted_domain/"
    cp "${TRUSTED_DOMAIN_DIR}/build/resource_table.bin" "${OUTPUT_DIR}/trusted_domain/"
    cp "${TRUSTED_DOMAIN_DIR}/build/trusted_fw.elf" "${OUTPUT_DIR}/trusted_domain/"
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
    export PATH="${NEWLIB_ELF_CROSS_COMPILE_DIR}/bin:$PATH"

    # 使用 CMake 构建（FreeRTOS 配置）
    # 注意：暂时不编译 FreeRTOS system 层，等后续移植
    cmake .. \
        -DCMAKE_TOOLCHAIN_FILE=../cmake/platforms/cross-generic-gcc.cmake \
        -DCROSS_PREFIX=riscv64-unknown-elf- \
        -DCMAKE_SYSTEM_PROCESSOR=riscv64 \
        -DCMAKE_INSTALL_PREFIX="$(pwd)/../lib" \
        -DCMAKE_C_FLAGS="-I${OPENAMP_ADAPTER_DIR}/include -I${BSP_INCLUDE_DIR} -DMETAL_MAX_DEVICE_REGIONS=1" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=OFF \
        -DMETAL_BUILD_SHARED_LIBS=OFF \
        -DWITH_FREERTOS_LIB=OFF \
        -DMETAL_MAX_DEVICE_REGIONS=1

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
    export PATH="${NEWLIB_ELF_CROSS_COMPILE_DIR}/bin:$PATH"

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
    log_step "Building OpenAMP Adapter Layer for FreeRTOS"
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

    # 编译 openamp_adapter 中的适配代码
    cd "${TRUSTED_DOMAIN_DIR}"

    if [ "${BUILD_MODE}" == "clean" ]; then
        rm -rf build/openamp_adapter
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        rm -rf build/openamp_adapter
    fi

    # 创建输出目录
    mkdir -p build/openamp_adapter/libmetal
    mkdir -p build/openamp_adapter/openamp
    mkdir -p lib

    # 编译 libmetal 适配层
    log_info "Compiling libmetal adapter layer..."
    for src in openamp_adapter/libmetal/*.c; do
        if [ -f "$src" ]; then
            obj="build/openamp_adapter/libmetal/$(basename "$src" .c).o"
            log_info "Compiling $src"
            ${NEWLIB_ELF_CROSS_PREFIX}-gcc ${CFLAGS_FREERTOS} -c "$src" -o "$obj"
        fi
    done

    # 编译 openamp 适配层
    log_info "Compiling openamp adapter layer..."
    for src in openamp_adapter/openamp/*.c; do
        if [ -f "$src" ]; then
            obj="build/openamp_adapter/openamp/$(basename "$src" .c).o"
            log_info "Compiling $src"
            ${NEWLIB_ELF_CROSS_PREFIX}-gcc ${CFLAGS_FREERTOS} -c "$src" -o "$obj"
        fi
    done

    # 打包成静态库
    ${NEWLIB_ELF_CROSS_PREFIX}-ar rcs lib/libopenamp_adapter.a build/openamp_adapter/*/*.o 2>/dev/null || true

    log_info "OpenAMP adapter built: ${TRUSTED_DOMAIN_DIR}/lib/libopenamp_adapter.a"
}

build_driver() {
    cd "${SHELL_FOLDER}"
    log_step "Building Linux Drivers"
    check_dir "${OUTPUT_DIR}/linux_driver"
    rm -f "${OUTPUT_DIR}/linux_driver/"*.ko

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
    log_driver_artifacts "${OUTPUT_DIR}/linux_driver"
    sync_driver_to_rootfs
}

sync_driver_to_rootfs() {
    local ROOTFS_STAGE_DIR="${OUTPUT_DIR}/rootfs/rootfs/driver"
    local IMG_FILE="${OUTPUT_DIR}/rootfs/rootfs.img"
    local MOUNT_DIR="${OUTPUT_DIR}/rootfs/driver_sync_mount"
    local loop_dev=""
    local sudo_cmd=""

    if [ "${BUILD_MODE}" == "clean" ]; then
        return 0
    fi

    if [ ! -d "${OUTPUT_DIR}/linux_driver" ] || ! ls "${OUTPUT_DIR}/linux_driver/"*.ko >/dev/null 2>&1; then
        log_info "No built .ko files found, skip rootfs sync"
        return 0
    fi

    # 1) 同步到 rootfs staging 目录（供后续 rootfs 打包使用）
    check_dir "${ROOTFS_STAGE_DIR}"
    rm -f "${ROOTFS_STAGE_DIR}/"*.ko
    cp -f "${OUTPUT_DIR}/linux_driver/"*.ko "${ROOTFS_STAGE_DIR}/"
    verify_driver_sync "${OUTPUT_DIR}/linux_driver" "${ROOTFS_STAGE_DIR}"
    log_driver_artifacts "${ROOTFS_STAGE_DIR}"
    log_info "Driver modules synced to staging: ${ROOTFS_STAGE_DIR}"

    # 2) 若已有 rootfs.img，则直接热更新镜像内 /driver（避免必须再跑 rootfs 目标）
    if [ ! -f "${IMG_FILE}" ]; then
        log_info "No rootfs image found, skip image sync (${IMG_FILE})"
        return 0
    fi

    if [ "$(id -u)" != "0" ]; then
        sudo_cmd="sudo"
        if ! sudo -n true 2>/dev/null; then
            log_info "rootfs.img exists; syncing drivers needs sudo, please enter password if prompted..."
            if ! { sudo -n true 2>/dev/null || sudo -v; }; then
                log_error "Cannot sync driver modules into rootfs image without sudo"
                return 1
            fi
        fi
    fi

    check_dir "${MOUNT_DIR}"

    cleanup_driver_sync() {
        if mountpoint -q "${MOUNT_DIR}"; then
            ${sudo_cmd} umount "${MOUNT_DIR}" || true
        fi
        if [ -n "${loop_dev}" ]; then
            ${sudo_cmd} losetup -d "${loop_dev}" || true
        fi
    }

    trap cleanup_driver_sync EXIT

    loop_dev=$(${sudo_cmd} losetup -f)
    ${sudo_cmd} losetup -P "${loop_dev}" "${IMG_FILE}"
    ${sudo_cmd} mount "${loop_dev}p2" "${MOUNT_DIR}"
    ${sudo_cmd} mkdir -p "${MOUNT_DIR}/driver"
    ${sudo_cmd} find "${MOUNT_DIR}/driver" -maxdepth 1 -name "*.ko" -delete
    ${sudo_cmd} cp -f "${OUTPUT_DIR}/linux_driver/"*.ko "${MOUNT_DIR}/driver/"
    verify_driver_sync "${OUTPUT_DIR}/linux_driver" "${MOUNT_DIR}/driver"
    sync

    cleanup_driver_sync
    trap - EXIT

    log_info "Driver modules synced into rootfs image: ${IMG_FILE}:/driver"
}

verify_driver_sync() {
    local src_dir="$1"
    local dst_dir="$2"
    local ko
    local ko_name
    local src_count
    local dst_count

    src_count=$(find "${src_dir}" -maxdepth 1 -name "*.ko" | wc -l)
    dst_count=$(find "${dst_dir}" -maxdepth 1 -name "*.ko" | wc -l)
    if [ "${src_count}" -ne "${dst_count}" ]; then
        log_error "Driver sync count mismatch: ${src_dir}=${src_count}, ${dst_dir}=${dst_count}"
        return 1
    fi

    for ko in "${src_dir}"/*.ko; do
        ko_name=$(basename "${ko}")
        if [ ! -f "${dst_dir}/${ko_name}" ]; then
            log_error "Driver sync missing: ${dst_dir}/${ko_name}"
            return 1
        fi
        if ! cmp -s "${ko}" "${dst_dir}/${ko_name}"; then
            log_error "Driver sync content mismatch: ${ko_name}"
            return 1
        fi
    done
}

log_driver_artifacts() {
    local dir="$1"
    local ko
    local ko_name
    local sha

    for ko in "${dir}"/*.ko; do
        ko_name=$(basename "${ko}")
        sha=$(sha256sum "${ko}" | awk '{print $1}')
        log_info "  ${ko_name} sha256=${sha}"
    done
}

verify_driver_deploy() {
    local ROOTFS_STAGE_DIR="${OUTPUT_DIR}/rootfs/rootfs/driver"
    local IMG_FILE="${OUTPUT_DIR}/rootfs/rootfs.img"
    local MOUNT_DIR="${OUTPUT_DIR}/rootfs/driver_verify_mount"
    local loop_dev=""
    local sudo_cmd=""

    log_step "Verifying Driver Deployment"

    if [ ! -d "${OUTPUT_DIR}/linux_driver" ] || ! ls "${OUTPUT_DIR}/linux_driver/"*.ko >/dev/null 2>&1; then
        log_error "No built driver modules found in ${OUTPUT_DIR}/linux_driver"
        return 1
    fi

    if [ ! -d "${ROOTFS_STAGE_DIR}" ]; then
        log_error "No driver staging directory found: ${ROOTFS_STAGE_DIR}"
        return 1
    fi

    verify_driver_sync "${OUTPUT_DIR}/linux_driver" "${ROOTFS_STAGE_DIR}"
    log_info "Verified staging sync: ${ROOTFS_STAGE_DIR}"

    if [ ! -f "${IMG_FILE}" ]; then
        log_info "No rootfs image found, skip image verification (${IMG_FILE})"
        return 0
    fi

    if [ "$(id -u)" != "0" ]; then
        sudo_cmd="sudo"
        if ! sudo -n true 2>/dev/null; then
            log_info "rootfs.img verification needs sudo, please enter password if prompted..."
            if ! { sudo -n true 2>/dev/null || sudo -v; }; then
                log_error "Cannot verify rootfs image without sudo"
                return 1
            fi
        fi
    fi

    check_dir "${MOUNT_DIR}"

    cleanup_driver_verify() {
        if mountpoint -q "${MOUNT_DIR}"; then
            ${sudo_cmd} umount "${MOUNT_DIR}" || true
        fi
        if [ -n "${loop_dev}" ]; then
            ${sudo_cmd} losetup -d "${loop_dev}" || true
        fi
    }

    trap cleanup_driver_verify EXIT

    loop_dev=$(${sudo_cmd} losetup -f)
    ${sudo_cmd} losetup -P "${loop_dev}" "${IMG_FILE}"
    ${sudo_cmd} mount -o ro "${loop_dev}p2" "${MOUNT_DIR}"

    verify_driver_sync "${OUTPUT_DIR}/linux_driver" "${MOUNT_DIR}/driver"

    cleanup_driver_verify
    trap - EXIT

    log_info "Verified image sync: ${IMG_FILE}:/driver"
}

# ==============================================================================
# Menuconfig 目标：对各组件跑 menuconfig，并把结果存回被跟踪的配置文件
# （menuconfig 为交互式 TUI，需要在真实终端里运行，依赖 libncurses-dev）
# ==============================================================================

build_kernelconfig() {
    cd "${SHELL_FOLDER}"
    log_step "Kernel menuconfig ($KERNEL_VER)"
    [ -d "${KERNEL_DIR}" ] || { msg_error "内核源码不存在: ${KERNEL_DIR}"; return 1; }
    cd "${KERNEL_DIR}"
    # 无 .config 时先用已保存的配置初始化
    if [ ! -f .config ] && [ -f "${CONFIG_FILE}" ]; then
        cp "${CONFIG_FILE}" .config
    fi
    make ARCH=riscv CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" menuconfig
    # 存回被跟踪的内核配置
    mkdir -p "$(dirname "${CONFIG_FILE}")"
    cp .config "${CONFIG_FILE}"
    msg_info "内核配置已存回: ${CONFIG_FILE}"
    finish_build
}

build_ubootconfig() {
    cd "${SHELL_FOLDER}"
    log_step "U-Boot menuconfig"
    cd "${UBOOT_DIR}"
    if [ ! -f .config ]; then
        make CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" qemu-quard-star_defconfig
    fi
    make CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" menuconfig
    # 用 savedefconfig 生成最小 defconfig 并存回
    make CROSS_COMPILE="${GLIB_ELF_CROSS_PREFIX}-" savedefconfig
    cp defconfig configs/qemu-quard-star_defconfig
    msg_warn "U-Boot defconfig 由 savedefconfig 重新生成(原注释会丢失)，请 git diff 确认"
    msg_info "U-Boot defconfig 已存回: ${UBOOT_DIR}/configs/qemu-quard-star_defconfig"
    finish_build
}


build_buildrootconfig() {
    cd "${SHELL_FOLDER}"
    log_step "Buildroot menuconfig"
    [ -f "${BUILDROOT_DIR}/Makefile" ] || { msg_error "buildroot 源码不存在: ${BUILDROOT_DIR}"; return 1; }
    local clean_path="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
    local run_as=""
    if [ "$(id -u)" = "0" ]; then
        [ -n "${SUDO_USER:-}" ] || { msg_error "请用普通用户运行 ./build.sh buildrootconfig"; return 1; }
        run_as="sudo -u ${SUDO_USER}"
    fi
    ${run_as} mkdir -p "${BUILDROOT_OUT}"
    [ -f "${BUILDROOT_OUT}/.config" ] || ${run_as} env PATH="${clean_path}" \
        make -C "${BUILDROOT_DIR}" O="${BUILDROOT_OUT}" BR2_DEFCONFIG="${BUILDROOT_DEFCONFIG}" defconfig
    ${run_as} env PATH="${clean_path}" make -C "${BUILDROOT_DIR}" O="${BUILDROOT_OUT}" menuconfig
    # 存回我们的 defconfig(最小化)
    ${run_as} env PATH="${clean_path}" \
        make -C "${BUILDROOT_DIR}" O="${BUILDROOT_OUT}" BR2_DEFCONFIG="${BUILDROOT_DEFCONFIG}" savedefconfig
    msg_info "Buildroot defconfig 已存回: ${BUILDROOT_DEFCONFIG}"
    finish_build
}

pack_firmware() {
    cd "${SHELL_FOLDER}"
    log_step "Packing Firmware (fw.bin)"
    check_dir "${OUTPUT_DIR}/fw"
    cd "${OUTPUT_DIR}/fw"
    
    rm -f fw.bin
    dd if=/dev/zero of=fw.bin bs=1k count="${FW_SIZE_KB}" status=none

    # 按 board.config 里的声明式 FW_LAYOUT 逐个写入，并做越界/重叠校验
    local prev_end=0 item off rel src sz_kb
    for item in "${FW_LAYOUT[@]}"; do
        off="${item%%:*}"
        rel="${item#*:}"
        src="${OUTPUT_DIR}/${rel}"
        if [ ! -f "$src" ]; then
            msg_error "固件部件缺失: $src  (布局项: ${item})"
            return 1
        fi
        sz_kb=$(( ( $(stat -c %s "$src") + 1023 ) / 1024 ))
        if [ "$off" -lt "$prev_end" ]; then
            msg_error "布局重叠: ${rel} 偏移 ${off}K < 上一部件结束 ${prev_end}K"
            return 1
        fi
        if [ $(( off + sz_kb )) -gt "$FW_SIZE_KB" ]; then
            msg_error "越界: ${rel} (${off}K + ${sz_kb}K) 超过 fw.bin 总大小 ${FW_SIZE_KB}K"
            return 1
        fi
        dd of=fw.bin bs=1k conv=notrunc seek="${off}" if="$src" status=none
        printf "  %-36s @ %6sK  (%4sK)\n" "$rel" "$off" "$sz_kb"
        prev_end=$(( off + sz_kb ))
    done

    log_info "Firmware packed at ${OUTPUT_DIR}/fw/fw.bin"
}

build_buildroot() {
    cd "${SHELL_FOLDER}"
    log_step "Building rootfs via Buildroot"

    # buildroot 拒绝带空格的 PATH(WSL 把 Windows PATH 导入会触发) —— 用干净 PATH
    local clean_path="/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
    # 代理透传(若环境里设置了，供 buildroot 下载用)
    local proxy_env=""
    [ -n "${HTTP_PROXY:-}" ]  && proxy_env="$proxy_env HTTP_PROXY=${HTTP_PROXY} http_proxy=${HTTP_PROXY}"
    [ -n "${HTTPS_PROXY:-}" ] && proxy_env="$proxy_env HTTPS_PROXY=${HTTPS_PROXY} https_proxy=${HTTPS_PROXY}"
    [ -n "${ALL_PROXY:-}" ]   && proxy_env="$proxy_env ALL_PROXY=${ALL_PROXY} all_proxy=${ALL_PROXY}"

    # buildroot 不能以 root 构建；若在 sudo 下则回落到 $SUDO_USER
    local run_as=""
    if [ "$(id -u)" = "0" ]; then
        if [ -z "${SUDO_USER:-}" ]; then
            msg_error "buildroot 不能以 root 构建。请直接用普通用户运行 ./build.sh（脚本会在 rootfs 阶段自行提权）"
            return 1
        fi
        run_as="sudo -u ${SUDO_USER}"
        msg_info "检测到 root，buildroot 将以用户 ${SUDO_USER} 身份构建"
    fi

    # 未找到 buildroot 源码则自动克隆(体积大、已 gitignore；联网经 proxy_env)
    if [ ! -f "${BUILDROOT_DIR}/Makefile" ]; then
        msg_info "未找到 buildroot 源码，正在克隆 ${BUILDROOT_VERSION} ..."
        ${run_as} env PATH="${clean_path}" ${proxy_env} \
            git clone --depth 1 --branch "${BUILDROOT_VERSION}" "${BUILDROOT_REPO}" "${BUILDROOT_DIR}" \
            || { msg_error "buildroot 克隆失败(需要网络/代理)"; return 1; }
    fi

    ${run_as} mkdir -p "${BUILDROOT_OUT}"

    if [ "${BUILD_MODE}" == "clean" ]; then
        ${run_as} env PATH="${clean_path}" make -C "${BUILDROOT_DIR}" O="${BUILDROOT_OUT}" clean
        return 0
    elif [ "${BUILD_MODE}" == "rebuild" ]; then
        ${run_as} env PATH="${clean_path}" make -C "${BUILDROOT_DIR}" O="${BUILDROOT_OUT}" clean
    fi

    # 生成/刷新 buildroot 配置
    ${run_as} env PATH="${clean_path}" make -C "${BUILDROOT_DIR}" O="${BUILDROOT_OUT}" \
        BR2_DEFCONFIG="${BUILDROOT_DEFCONFIG}" defconfig

    # 构建 rootfs（联网下载经 proxy_env）
    ${run_as} env PATH="${clean_path}" ${proxy_env} \
        make -C "${BUILDROOT_DIR}" O="${BUILDROOT_OUT}" -j"${JOBS}"

    [ -f "${BUILDROOT_ROOTFS_TAR}" ] || { msg_error "buildroot 未产出 ${BUILDROOT_ROOTFS_TAR}"; return 1; }
    msg_info "Buildroot rootfs: ${BUILDROOT_ROOTFS_TAR}"
    finish_build
}

# 组装最终 rootfs.img：buildroot 基础 rootfs + overlay + 注入驱动 -> 2 分区(boot vfat / rootfs ext4)
# 往 rootfs 加自己的东西(静态文件/程序/驱动)放哪，见 docs/rootfs-additions.md
build_rootfs() {
    cd "${SHELL_FOLDER}"
    log_step "Building Root Filesystem Image"

    if [ "$(id -u)" != "0" ]; then
        log_info "需要 root 权限来生成根文件系统，请稍后输入密码..."
        if ! { sudo -n true 2>/dev/null || sudo -v; }; then
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

    # rootfs 内容填充：解 buildroot 的 rootfs.tar 入暂存区
    # tar 含 devnode 与 root 属主，需 sudo；故后续驱动注入/同步也用 $SUDO
    local SUDO="sudo"
    [ -f "${BUILDROOT_ROOTFS_TAR}" ] || { log_error "buildroot rootfs.tar 不存在，请先 ./build.sh buildroot"; exit 1; }
    log_info "从 buildroot 解包 rootfs ..."
    sudo find "${ROOTFS_DIR}/rootfs" -mindepth 1 -delete 2>/dev/null || true
    sudo tar -xf "${BUILDROOT_ROOTFS_TAR}" -C "${ROOTFS_DIR}/rootfs"

    # 3. 拷贝驱动模块到 rootfs/driver/
    ${SUDO} mkdir -p "${ROOTFS_DIR}/rootfs/driver"
    if [ -d "${OUTPUT_DIR}/linux_driver" ] && [ "$(ls -A ${OUTPUT_DIR}/linux_driver/*.ko 2>/dev/null)" ]; then
        ${SUDO} cp -r "${OUTPUT_DIR}/linux_driver/"*.ko "${ROOTFS_DIR}/rootfs/driver/"
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

    # sysroot 指向 buildroot 的 staging（带头文件的开发 sysroot，交叉编译应用用 --sysroot=sysroot）
    # 不再拷贝最终 rootfs（那份内容浏览用 output/rootfs/rootfs/ 即可，避免重复）
    if [ -d "${BUILDROOT_OUT}/staging" ]; then
        sudo rm -rf "${SYSROOT_DIR}" 2>/dev/null || rm -rf "${SYSROOT_DIR}" 2>/dev/null || true
        ln -snf "${BUILDROOT_OUT}/staging" "${SYSROOT_DIR}"
        log_info "sysroot -> ${BUILDROOT_OUT}/staging (开发 sysroot, 交叉编译用 --sysroot=${SYSROOT_DIR})"
    fi

    log_info "Rootfs image: ${IMG_FILE}"
    log_info "浏览最终 rootfs: ${ROOTFS_DIR}/rootfs/"
}

clean_all() {
    cd "${SHELL_FOLDER}"
    log_step "Cleaning all build artifacts"
    rm -rf "${OUTPUT_DIR}"
    cd "${QEMU_DIR}" && if [ -d build ]; then rm -rf build; fi
    cd "${UBOOT_DIR}" && make distclean
    cd "${KERNEL_DIR}" && make distclean
    cd "${OPENSBI_DIR}" && make distclean
    cd "${FREERTOS_DIR}" && make clean
}

usage() {
    echo "Usage: $0 [target] [mode]"
    echo "Targets:"
    echo "  all             Build everything (default)"
    echo "  check           Check host build deps & cross-toolchain"
    echo "  clean           Clean all build artifacts and source directories"
    echo "  qemu            Build QEMU"
    echo "  boot            Build LowLevelBoot"
    echo "  opensbi         Build OpenSBI"
    echo "  uboot           Build U-Boot"
    echo "  kernel          Build Linux Kernel"
    echo "  freertos        Build FreeRTOS (Trusted Domain)"
    echo "  driver          Build Linux Drivers (out-of-tree)"
    echo "  rootfs          Build Root Filesystem Image (from buildroot rootfs.tar)"
    echo "  buildroot       Build rootfs via Buildroot"
    echo "  firmware        Pack Firmware (fw.bin)"
    echo "  verify-driver   Verify driver .ko consistency (output/staging/image)"
    echo ""
    echo "  Menuconfig targets (saves result back to tracked config):"
    echo "  kernelconfig    Linux kernel menuconfig  -> project/configs/<ver>_config"
    echo "  ubootconfig     U-Boot menuconfig        -> u-boot configs/qemu-quard-star_defconfig"
    echo "  buildrootconfig Buildroot menuconfig     -> project/configs/buildroot_quard_star_defconfig"
    echo ""
    echo "  OpenAMP-specific targets:"
    echo "  submodules      Initialize Git submodules (libmetal, open-amp)"
    echo "  libmetal        Build libmetal library"
    echo "  openamp         Build open-amp library"
    echo "  openamp-adapter Build OpenAMP adapter layer (trusted_domain)"
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

# 构建类目标开跑前自动校验交叉工具链（help/check/clean 跳过）
case "$TARGET" in
    help|check|clean) : ;;
    *) check_toolchain || exit 1 ;;
esac

case "$TARGET" in
    "all")
        build_check || exit 1
        build_qemu
        build_lowlevelboot
        build_opensbi
        build_freertos
        build_uboot
        build_kernel
        build_buildroot
        build_driver
        pack_firmware
        build_rootfs
        ;;
    "check")
        rc=0
        build_check || rc=1
        check_toolchain || rc=1
        if [ $rc -eq 0 ]; then msg_info "全部检测通过 ✓"; else msg_error "存在缺失项，请按上面提示安装后重试"; fi
        exit $rc
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
    "buildroot")
        build_buildroot
        build_rootfs
        ;;
    "freertos")
        build_freertos
        pack_firmware
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
    "verify-driver")
        verify_driver_deploy
        ;;
    "kernelconfig")
        build_kernelconfig
        ;;
    "ubootconfig")
        build_ubootconfig
        ;;
    "buildrootconfig")
        build_buildrootconfig
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
