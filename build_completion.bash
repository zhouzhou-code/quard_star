# build_completion.bash

# 定义一个函数，用于tab补全build.sh脚本的参数，在终端中source build_completion.bash后即可使用
_build_sh_completion() {
    local cur prev opts modes
    COMPREPLY=()
    # 获取当前正在输入的词
    cur="${COMP_WORDS[COMP_CWORD]}"
    
    # 定义第一层参数 (targets)
    opts="all clean qemu boot opensbi uboot kernel busybox freertos rootfs firmware"
    
    # 定义第二层参数 (modes)
    modes="incremental clean rebuild"

    # 如果正在输入第一个参数（Target）
    if [[ ${COMP_CWORD} -eq 1 ]]; then
        COMPREPLY=( $(compgen -W "${opts}" -- "${cur}") )
        return 0
    fi
    
    # 如果正在输入第二个参数（Mode）
    if [[ ${COMP_CWORD} -eq 2 ]]; then
        COMPREPLY=( $(compgen -W "${modes}" -- "${cur}") )
        return 0
    fi
}

# 绑定这个补全规则到你的编译脚本上，实现自动补全功能
# 这里绑定了绝对路径、相对路径和单文件名三种习惯
complete -F _build_sh_completion ./build.sh
complete -F _build_sh_completion build.sh