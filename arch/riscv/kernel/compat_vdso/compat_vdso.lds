/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * This header exists to force full rebuild when the compiler is upgraded.
 *
 * When fixdep scans this, it will find this string "CONFIG_CC_VERSION_TEXT"
 * and add dependency on include/config/CC_VERSION_TEXT, which is touched
 * by Kconfig when the version string from the compiler changes.
 */
/* Additional tree-wide dependencies start here. */
/*
 * If any of the GCC plugins change, we need to rebuild everything that
 * was built with them, as they may have changed their behavior and those
 * behaviors may need to be synchronized across all translation units.
 */
/*
 * If the randstruct seed itself changes (whether for GCC plugins or
 * Clang), the entire tree needs to be rebuilt since the randomization of
 * structures may change between compilation units if not.
 */
/*
 * If any external changes affect Clang's integer wrapping sanitizer
 * behavior, a full rebuild is needed as the coverage for wrapping types
 * may have changed, which may impact the expected behaviors that should
 * not differ between compilation units.
 */
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Automatically generated file; DO NOT EDIT.
 * Linux/riscv 6.19.0-rc7 Kernel Configuration
 */
/*
 * The use of "&&" / "||" is limited in certain expressions.
 * The following enable to calculate "and" / "or" with macro expansion only.
 */
/*
 * Helper macros to use CONFIG_ options in C/CPP expressions. Note that
 * these only work with boolean and tristate options.
 */
/*
 * Getting something that works in C and CPP for an arg that may or may
 * not be defined is tricky.  Here, if we have "#define CONFIG_BOOGER 1"
 * we match on the placeholder define, insert the "0," for arg1 and generate
 * the triplet (0, 1, 0).  Then the last step cherry picks the 2nd arg (a one).
 * When CONFIG_BOOGER is not defined, we generate a (... 1, 0) pair, and when
 * the last step cherry picks the 2nd arg, we get a zero.
 */
/*
 * IS_BUILTIN(CONFIG_FOO) evaluates to 1 if CONFIG_FOO is set to 'y', 0
 * otherwise. For boolean options, this is equivalent to
 * IS_ENABLED(CONFIG_FOO).
 */
/*
 * IS_MODULE(CONFIG_FOO) evaluates to 1 if CONFIG_FOO is set to 'm', 0
 * otherwise.  CONFIG_FOO=m results in "#define CONFIG_FOO_MODULE 1" in
 * autoconf.h.
 */
/*
 * IS_REACHABLE(CONFIG_FOO) evaluates to 1 if the currently compiled
 * code can call a function defined in code compiled based on CONFIG_FOO.
 * This is similar to IS_ENABLED(), but returns false when invoked from
 * built-in code when CONFIG_FOO is set to 'm'.
 */
/*
 * IS_ENABLED(CONFIG_FOO) evaluates to 1 if CONFIG_FOO is set to 'y' or 'm',
 * 0 otherwise.  Note that CONFIG_FOO=y results in "#define CONFIG_FOO 1" in
 * autoconf.h, while CONFIG_FOO=m results in "#define CONFIG_FOO_MODULE 1".
 */
/* SPDX-License-Identifier: GPL-2.0-only */
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Regents of the University of California
 */
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2009 Chen Liqin <liqin.chen@sunplusct.com>
 * Copyright (C) 2012 Regents of the University of California
 * Copyright (C) 2017 SiFive
 * Copyright (C) 2017 XiaojingZhu <zhuxiaoj@ict.ac.cn>
 */
/* SPDX-License-Identifier: GPL-2.0 */
/* SPDX-License-Identifier: GPL-2.0 */
/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* const.h: Macros for dealing with constants.  */
/* Some constant macros are used in both assembler and
 * C code.  Therefore we cannot annotate them always with
 * 'UL' and other type specifiers unilaterally.  We
 * use the following macros to deal with this.
 *
 * Similarly, _AT() will cast an expression with a type in C, but
 * leave it unchanged in asm.
 */
/* SPDX-License-Identifier: GPL-2.0 */
/* SPDX-License-Identifier: GPL-2.0 WITH Linux-syscall-note */
/* const.h: Macros for dealing with constants.  */
/*
 * PAGE_SHIFT determines the page size.
 *
 * Note: This definition is required because PAGE_SHIFT is used
 * in several places throughout the codebase.
 */
/*
 * PAGE_OFFSET -- the first address of the first page of memory.
 * When not using MMU this corresponds to the first free page in
 * physical memory (aligned on a page boundary).
 */
/* SPDX-License-Identifier: GPL-2.0 */
/* SPDX-License-Identifier: GPL-2.0 */
/* SPDX-License-Identifier: GPL-2.0 */
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 ARM Limited
 * Copyright (C) 2014 Regents of the University of California
 * Copyright (C) 2017 SiFive
 */
/*
 * All systems with an MMU have a VDSO, but systems without an MMU don't
 * support shared libraries and therefore don't have one.
 */
/* SPDX-License-Identifier: GPL-2.0 */
OUTPUT_ARCH(riscv)
SECTIONS
{
 PROVIDE(vdso_u_data = . - 4 * (1 << 12)); PROVIDE(vdso_u_time_data = vdso_u_data); PROVIDE(vdso_u_rng_data = vdso_u_data + 2 * (1 << 12)); PROVIDE(vdso_u_arch_data = vdso_u_data + 3 * (1 << 12));
 . = SIZEOF_HEADERS;
 .hash : { *(.hash) } :text
 .gnu.hash : { *(.gnu.hash) }
 .dynsym : { *(.dynsym) }
 .dynstr : { *(.dynstr) }
 .gnu.version : { *(.gnu.version) }
 .gnu.version_d : { *(.gnu.version_d) }
 .gnu.version_r : { *(.gnu.version_r) }
 .dynamic : { *(.dynamic) } :text :dynamic
 .rodata : {
  *(.rodata .rodata.* .gnu.linkonce.r.*)
  *(.got.plt) *(.got)
  *(.data .data.* .gnu.linkonce.d.*)
  *(.dynbss)
  *(.bss .bss.* .gnu.linkonce.b.*)
 } :text
 .note : { *(.note.*) } :text :note
 .eh_frame_hdr : { *(.eh_frame_hdr) } :text :eh_frame_hdr
 .eh_frame : { KEEP (*(.eh_frame)) } :text
 /*
	 * Text is well-separated from actual data: there's plenty of
	 * stuff that isn't used at runtime in between.
	 */
 . = ALIGN(16);
 .text : { *(.text .text.*) } :text
 . = ALIGN(4);
 .alternative : {
  *(.alternative)
 }
}
/*
 * We must supply the ELF program headers explicitly to get just one
 * PT_LOAD segment, and set the flags explicitly to make segments read-only.
 */
PHDRS
{
 text PT_LOAD FLAGS(5) FILEHDR PHDRS; /* PF_R|PF_X */
 dynamic PT_DYNAMIC FLAGS(4); /* PF_R */
 note PT_NOTE FLAGS(4); /* PF_R */
 eh_frame_hdr PT_GNU_EH_FRAME;
}
/*
 * This controls what symbols we export from the DSO.
 */
VERSION
{
 LINUX_4.15 {
 global:
  __vdso_rt_sigreturn;
  __vdso_getcpu;
  __vdso_flush_icache;
 local: *;
 };
}
