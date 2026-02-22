# Linux Kernel Config Comparison Report
**File 1**: `linux-5.10/.config`
**File 2**: `linux-5.10/old_perfect.config`

## 不同配置值 (176 项)
| 配置项 | Value in File 1 | Value in File 2 |
| :--- | :--- | :--- |
| `CONFIG_AUDIT` | `is not set` | `y` |
| `CONFIG_BACKLIGHT_CLASS_DEVICE` | `y` | `m` |
| `CONFIG_BLK_CGROUP` | `is not set` | `y` |
| `CONFIG_BLK_DEV_NVME` | `is not set` | `m` |
| `CONFIG_BRIDGE` | `is not set` | `m` |
| `CONFIG_BTRFS_FS` | `is not set` | `m` |
| `CONFIG_CAN` | `is not set` | `m` |
| `CONFIG_CC_VERSION_TEXT` | `"riscv64-unknown-linux-gnu-gcc (g5115c7e44) 15.2.0"` | `"riscv64-linux-gcc.br_real (Buildroot 2021.11-18033-g83947c7bb6) 14.3.0"` |
| `CONFIG_CGROUP_CPUACCT` | `is not set` | `y` |
| `CONFIG_CGROUP_DEVICE` | `is not set` | `y` |
| `CONFIG_CGROUP_FREEZER` | `is not set` | `y` |
| `CONFIG_CGROUP_NET_CLASSID` | `is not set` | `y` |
| `CONFIG_CGROUP_NET_PRIO` | `is not set` | `y` |
| `CONFIG_CGROUP_PIDS` | `is not set` | `y` |
| `CONFIG_COMPAT_32BIT_TIME` | `is not set` | `y` |
| `CONFIG_CONFIGFS_FS` | `is not set` | `m` |
| `CONFIG_CPUSETS` | `is not set` | `y` |
| `CONFIG_CRC_CCITT` | `is not set` | `m` |
| `CONFIG_CRYPTO` | `y` | `m` |
| `CONFIG_CRYPTO_AEAD` | `y` | `m` |
| `CONFIG_CRYPTO_AEAD2` | `y` | `m` |
| `CONFIG_CRYPTO_AES` | `is not set` | `m` |
| `CONFIG_CRYPTO_ALGAPI` | `y` | `m` |
| `CONFIG_CRYPTO_ALGAPI2` | `y` | `m` |
| `CONFIG_CRYPTO_AUTHENC` | `is not set` | `m` |
| `CONFIG_CRYPTO_BLAKE2B` | `is not set` | `m` |
| `CONFIG_CRYPTO_CBC` | `is not set` | `m` |
| `CONFIG_CRYPTO_CRC32C` | `y` | `m` |
| `CONFIG_CRYPTO_CTR` | `is not set` | `m` |
| `CONFIG_CRYPTO_DEV_VIRTIO` | `y` | `m` |
| `CONFIG_CRYPTO_DRBG_MENU` | `is not set` | `m` |
| `CONFIG_CRYPTO_ECB` | `is not set` | `m` |
| `CONFIG_CRYPTO_ECHAINIV` | `is not set` | `m` |
| `CONFIG_CRYPTO_ENGINE` | `y` | `m` |
| `CONFIG_CRYPTO_GCM` | `is not set` | `m` |
| `CONFIG_CRYPTO_GHASH` | `is not set` | `m` |
| `CONFIG_CRYPTO_HASH` | `y` | `m` |
| `CONFIG_CRYPTO_HASH2` | `y` | `m` |
| `CONFIG_CRYPTO_HMAC` | `is not set` | `m` |
| `CONFIG_CRYPTO_JITTERENTROPY` | `is not set` | `m` |
| `CONFIG_CRYPTO_LIB_POLY1305_RSIZE` | `1` | `2` |
| `CONFIG_CRYPTO_MANAGER` | `is not set` | `m` |
| `CONFIG_CRYPTO_RNG2` | `y` | `m` |
| `CONFIG_CRYPTO_RSA` | `is not set` | `m` |
| `CONFIG_CRYPTO_SEQIV` | `is not set` | `m` |
| `CONFIG_CRYPTO_SHA256` | `is not set` | `m` |
| `CONFIG_CRYPTO_SHA3` | `is not set` | `m` |
| `CONFIG_CRYPTO_SHA512` | `is not set` | `m` |
| `CONFIG_CRYPTO_SKCIPHER` | `y` | `m` |
| `CONFIG_CRYPTO_SKCIPHER2` | `y` | `m` |
| `CONFIG_CRYPTO_USER_API` | `y` | `m` |
| `CONFIG_CRYPTO_USER_API_HASH` | `y` | `m` |
| `CONFIG_CRYPTO_XXHASH` | `is not set` | `m` |
| `CONFIG_DAX` | `is not set` | `y` |
| `CONFIG_DEBUG_ATOMIC_SLEEP` | `y` | `is not set` |
| `CONFIG_DEBUG_LIST` | `y` | `is not set` |
| `CONFIG_DEBUG_MUTEXES` | `y` | `is not set` |
| `CONFIG_DEBUG_PAGEALLOC` | `y` | `is not set` |
| `CONFIG_DEBUG_PER_CPU_MAPS` | `y` | `is not set` |
| `CONFIG_DEBUG_PLIST` | `y` | `is not set` |
| `CONFIG_DEBUG_RT_MUTEXES` | `y` | `is not set` |
| `CONFIG_DEBUG_RWSEMS` | `y` | `is not set` |
| `CONFIG_DEBUG_SG` | `y` | `is not set` |
| `CONFIG_DEBUG_SPINLOCK` | `y` | `is not set` |
| `CONFIG_DEBUG_VM` | `y` | `is not set` |
| `CONFIG_DEBUG_VM_PGTABLE` | `y` | `is not set` |
| `CONFIG_DETECT_HUNG_TASK` | `y` | `is not set` |
| `CONFIG_DMADEVICES` | `is not set` | `y` |
| `CONFIG_DRM` | `y` | `m` |
| `CONFIG_DRM_GEM_SHMEM_HELPER` | `y` | `m` |
| `CONFIG_DRM_KMS_HELPER` | `y` | `m` |
| `CONFIG_DRM_NOUVEAU` | `is not set` | `m` |
| `CONFIG_DRM_PANEL_ORIENTATION_QUIRKS` | `y` | `m` |
| `CONFIG_DRM_RADEON` | `y` | `m` |
| `CONFIG_DRM_TTM` | `y` | `m` |
| `CONFIG_DRM_VIRTIO_GPU` | `y` | `m` |
| `CONFIG_DUMMY` | `is not set` | `m` |
| `CONFIG_EEPROM_93CX6` | `is not set` | `y` |
| `CONFIG_EXPERT` | `y` | `is not set` |
| `CONFIG_EXT4_FS_SECURITY` | `is not set` | `y` |
| `CONFIG_EXTCON` | `is not set` | `y` |
| `CONFIG_FLATMEM_MANUAL` | `y` | `is not set` |
| `CONFIG_FTRACE` | `is not set` | `y` |
| `CONFIG_GCC_VERSION` | `150200` | `140300` |
| `CONFIG_GENERIC_PHY` | `is not set` | `y` |
| `CONFIG_GPIOLIB` | `is not set` | `y` |
| `CONFIG_HID_A4TECH` | `is not set` | `y` |
| `CONFIG_HID_BELKIN` | `is not set` | `y` |
| `CONFIG_HID_CHERRY` | `is not set` | `y` |
| `CONFIG_HID_CHICONY` | `is not set` | `y` |
| `CONFIG_HID_CYPRESS` | `is not set` | `y` |
| `CONFIG_HID_EZKEY` | `is not set` | `y` |
| `CONFIG_HID_ITE` | `is not set` | `y` |
| `CONFIG_HID_KENSINGTON` | `is not set` | `y` |
| `CONFIG_HID_MICROSOFT` | `is not set` | `y` |
| `CONFIG_HID_MONTEREY` | `is not set` | `y` |
| `CONFIG_HID_REDRAGON` | `is not set` | `y` |
| `CONFIG_HOTPLUG_CPU` | `is not set` | `y` |
| `CONFIG_HUGETLBFS` | `is not set` | `y` |
| `CONFIG_I2C_ALGOBIT` | `y` | `m` |
| `CONFIG_I2C_CHARDEV` | `is not set` | `m` |
| `CONFIG_I2C_DESIGNWARE_PLATFORM` | `is not set` | `y` |
| `CONFIG_I2C_HID` | `is not set` | `y` |
| `CONFIG_I2C_MUX` | `is not set` | `m` |
| `CONFIG_IIO` | `is not set` | `y` |
| `CONFIG_INET_ESP` | `is not set` | `m` |
| `CONFIG_INIT_STACK_ALL_ZERO` | `is not set` | `y` |
| `CONFIG_INIT_STACK_NONE` | `y` | `is not set` |
| `CONFIG_INPUT_EVDEV` | `is not set` | `y` |
| `CONFIG_INPUT_FF_MEMLESS` | `is not set` | `y` |
| `CONFIG_IOSCHED_BFQ` | `is not set` | `y` |
| `CONFIG_IPVLAN` | `is not set` | `m` |
| `CONFIG_ISO9660_FS` | `is not set` | `y` |
| `CONFIG_LD_VERSION` | `245000000` | `24301` |
| `CONFIG_LIBNVDIMM` | `is not set` | `y` |
| `CONFIG_LSM` | `"lockdown,yama,loadpin,safesetid,integrity,bpf"` | `"landlock,lockdown,yama,loadpin,safesetid,ipe,bpf"` |
| `CONFIG_MACVLAN` | `is not set` | `m` |
| `CONFIG_MAILBOX` | `is not set` | `y` |
| `CONFIG_MD` | `is not set` | `y` |
| `CONFIG_MEDIA_SUPPORT` | `is not set` | `m` |
| `CONFIG_MEMCG` | `is not set` | `y` |
| `CONFIG_MFD_AXP20X_I2C` | `is not set` | `y` |
| `CONFIG_MICREL_PHY` | `is not set` | `y` |
| `CONFIG_MMC_CQHCI` | `is not set` | `y` |
| `CONFIG_MMC_DW` | `is not set` | `y` |
| `CONFIG_MMC_SDHCI` | `is not set` | `y` |
| `CONFIG_MTD` | `is not set` | `y` |
| `CONFIG_NETFILTER` | `is not set` | `y` |
| `CONFIG_NETWORK_SECMARK` | `is not set` | `y` |
| `CONFIG_NET_L3_MASTER_DEV` | `is not set` | `y` |
| `CONFIG_NET_SCHED` | `is not set` | `y` |
| `CONFIG_NFS_V2` | `y` | `is not set` |
| `CONFIG_NFS_V4_2_READ_PLUS` | `is not set` | `y` |
| `CONFIG_NLS_CODEPAGE_437` | `is not set` | `y` |
| `CONFIG_NLS_ISO8859_1` | `is not set` | `m` |
| `CONFIG_NOP_USB_XCEIV` | `is not set` | `m` |
| `CONFIG_NR_CPUS` | `8` | `64` |
| `CONFIG_OID_REGISTRY` | `y` | `m` |
| `CONFIG_OVERLAY_FS` | `is not set` | `m` |
| `CONFIG_PCS_XPCS` | `is not set` | `m` |
| `CONFIG_PERF_EVENTS` | `is not set` | `y` |
| `CONFIG_PGTABLE_LEVELS` | `3` | `5` |
| `CONFIG_PINCTRL` | `is not set` | `y` |
| `CONFIG_PM` | `is not set` | `y` |
| `CONFIG_PM_DEVFREQ` | `is not set` | `y` |
| `CONFIG_PNFS_FLEXFILE_LAYOUT` | `m` | `y` |
| `CONFIG_PROFILING` | `is not set` | `y` |
| `CONFIG_PWM` | `is not set` | `y` |
| `CONFIG_RCU_EQS_DEBUG` | `y` | `is not set` |
| `CONFIG_RCU_TRACE` | `is not set` | `y` |
| `CONFIG_REGULATOR` | `is not set` | `y` |
| `CONFIG_RESET_CONTROLLER` | `is not set` | `y` |
| `CONFIG_RISCV_SBI_V01` | `y` | `is not set` |
| `CONFIG_SATA_MOBILE_LPM_POLICY` | `0` | `3` |
| `CONFIG_SCHED_STACK_END_CHECK` | `y` | `is not set` |
| `CONFIG_SECURITY` | `is not set` | `y` |
| `CONFIG_SECURITYFS` | `is not set` | `y` |
| `CONFIG_SERIAL_8250_DW` | `is not set` | `y` |
| `CONFIG_SOFTLOCKUP_DETECTOR` | `y` | `is not set` |
| `CONFIG_SOUND` | `is not set` | `y` |
| `CONFIG_SPARSEMEM_MANUAL` | `is not set` | `y` |
| `CONFIG_SPI_MEM` | `is not set` | `y` |
| `CONFIG_STMMAC_ETH` | `is not set` | `m` |
| `CONFIG_SUNRPC_GSS` | `y` | `m` |
| `CONFIG_THERMAL` | `is not set` | `y` |
| `CONFIG_USB_CDNS3` | `is not set` | `m` |
| `CONFIG_USB_GADGET` | `is not set` | `y` |
| `CONFIG_USB_MUSB_HDRC` | `is not set` | `m` |
| `CONFIG_USB_ROLE_SWITCH` | `is not set` | `m` |
| `CONFIG_VETH` | `is not set` | `m` |
| `CONFIG_VIRTIO_DMA_SHARED_BUFFER` | `y` | `m` |
| `CONFIG_VLAN_8021Q` | `is not set` | `m` |
| `CONFIG_VXLAN` | `is not set` | `m` |
| `CONFIG_WATCHDOG` | `is not set` | `y` |
| `CONFIG_WQ_WATCHDOG` | `y` | `is not set` |
| `CONFIG_XFRM_USER` | `is not set` | `m` |

## 仅在 File 1 中存在 (233 项)
| 配置项 | Value |
| :--- | :--- |
| `CONFIG_ABX500_CORE` | `is not set` |
| `CONFIG_AHCI_QORIQ` | `is not set` |
| `CONFIG_ANDROID` | `is not set` |
| `CONFIG_ARCH_CLOCKSOURCE_INIT` | `y` |
| `CONFIG_ARCH_RV32I` | `is not set` |
| `CONFIG_ARCH_WANT_GENERAL_HUGETLB` | `y` |
| `CONFIG_AURORA_NB8800` | `is not set` |
| `CONFIG_AUTOFS4_FS` | `y` |
| `CONFIG_BASE_FULL` | `y` |
| `CONFIG_BASE_SMALL` | `0` |
| `CONFIG_BLK_CMDLINE_PARSER` | `is not set` |
| `CONFIG_BLK_DEV_CRYPTOLOOP` | `is not set` |
| `CONFIG_BLK_DEV_RSXX` | `is not set` |
| `CONFIG_BLK_DEV_SKD` | `is not set` |
| `CONFIG_BLK_DEV_SX8` | `is not set` |
| `CONFIG_BLK_DEV_UMEM` | `is not set` |
| `CONFIG_BLK_MQ_PCI` | `y` |
| `CONFIG_BLK_MQ_VIRTIO` | `y` |
| `CONFIG_BLK_SCSI_REQUEST` | `y` |
| `CONFIG_BOOTPARAM_HUNG_TASK_PANIC` | `is not set` |
| `CONFIG_BOOTPARAM_HUNG_TASK_PANIC_VALUE` | `0` |
| `CONFIG_BOOTPARAM_SOFTLOCKUP_PANIC` | `is not set` |
| `CONFIG_BOOTPARAM_SOFTLOCKUP_PANIC_VALUE` | `0` |
| `CONFIG_BPFILTER` | `is not set` |
| `CONFIG_CC_CAN_LINK_STATIC` | `y` |
| `CONFIG_CC_HAS_ASM_GOTO` | `y` |
| `CONFIG_CC_HAS_SANCOV_TRACE_PC` | `y` |
| `CONFIG_CDROM_PKTCDVD` | `is not set` |
| `CONFIG_CLEANCACHE` | `is not set` |
| `CONFIG_CLKDEV_LOOKUP` | `y` |
| `CONFIG_CLK_SIFIVE_FU540_PRCI` | `y` |
| `CONFIG_CPUMASK_OFFSTACK` | `is not set` |
| `CONFIG_CRC32_BIT` | `is not set` |
| `CONFIG_CRC32_SARWATE` | `is not set` |
| `CONFIG_CRC32_SELFTEST` | `is not set` |
| `CONFIG_CRC32_SLICEBY4` | `is not set` |
| `CONFIG_CRC32_SLICEBY8` | `y` |
| `CONFIG_CRC4` | `is not set` |
| `CONFIG_CRC64` | `is not set` |
| `CONFIG_CRC8` | `is not set` |
| `CONFIG_CRC_T10DIF` | `is not set` |
| `CONFIG_CRYPTO_ANSI_CPRNG` | `is not set` |
| `CONFIG_CRYPTO_BLAKE2S` | `is not set` |
| `CONFIG_CRYPTO_CFB` | `is not set` |
| `CONFIG_CRYPTO_CRCT10DIF` | `is not set` |
| `CONFIG_CRYPTO_CURVE25519` | `is not set` |
| `CONFIG_CRYPTO_KEYWRAP` | `is not set` |
| `CONFIG_CRYPTO_LIB_BLAKE2S` | `is not set` |
| `CONFIG_CRYPTO_LIB_CHACHA` | `is not set` |
| `CONFIG_CRYPTO_LIB_CHACHA20POLY1305` | `is not set` |
| `CONFIG_CRYPTO_LIB_CURVE25519` | `is not set` |
| `CONFIG_CRYPTO_LIB_POLY1305` | `is not set` |
| `CONFIG_CRYPTO_MANAGER_DISABLE_TESTS` | `y` |
| `CONFIG_CRYPTO_NULL2` | `y` |
| `CONFIG_CRYPTO_OFB` | `is not set` |
| `CONFIG_CRYPTO_POLY1305` | `is not set` |
| `CONFIG_CRYPTO_RMD128` | `is not set` |
| `CONFIG_CRYPTO_RMD256` | `is not set` |
| `CONFIG_CRYPTO_RMD320` | `is not set` |
| `CONFIG_CRYPTO_SALSA20` | `is not set` |
| `CONFIG_CRYPTO_SM2` | `is not set` |
| `CONFIG_CRYPTO_SM3` | `is not set` |
| `CONFIG_CRYPTO_SM4` | `is not set` |
| `CONFIG_CRYPTO_TEST` | `is not set` |
| `CONFIG_CRYPTO_TGR192` | `is not set` |
| `CONFIG_CRYPTO_VMAC` | `is not set` |
| `CONFIG_DEBUG_BLOCK_EXT_DEVT` | `y` |
| `CONFIG_DEBUG_CREDENTIALS` | `is not set` |
| `CONFIG_DEBUG_FORCE_FUNCTION_ALIGN_32B` | `is not set` |
| `CONFIG_DEBUG_FS_DISALLOW_MOUNT` | `is not set` |
| `CONFIG_DEBUG_INFO` | `is not set` |
| `CONFIG_DEBUG_PAGEALLOC_ENABLE_DEFAULT` | `is not set` |
| `CONFIG_DEBUG_TIMEKEEPING` | `y` |
| `CONFIG_DEBUG_VM_PGFLAGS` | `y` |
| `CONFIG_DEBUG_VM_RB` | `is not set` |
| `CONFIG_DEBUG_VM_VMACACHE` | `is not set` |
| `CONFIG_DECNET` | `is not set` |
| `CONFIG_DEFAULT_HUNG_TASK_TIMEOUT` | `120` |
| `CONFIG_DEVKMEM` | `is not set` |
| `CONFIG_DRM_DEBUG_DP_MST_TOPOLOGY_REFS` | `is not set` |
| `CONFIG_DRM_DEBUG_SELFTEST` | `is not set` |
| `CONFIG_DRM_DP_AUX_CHARDEV` | `is not set` |
| `CONFIG_DRM_DP_CEC` | `is not set` |
| `CONFIG_DRM_FBDEV_LEAK_PHYS_SMEM` | `is not set` |
| `CONFIG_DRM_I2C_CH7006` | `is not set` |
| `CONFIG_DRM_I2C_NXP_TDA9950` | `is not set` |
| `CONFIG_DRM_I2C_SIL164` | `is not set` |
| `CONFIG_DRM_KMS_FB_HELPER` | `y` |
| `CONFIG_DRM_LEGACY` | `is not set` |
| `CONFIG_DRM_MXSFB` | `is not set` |
| `CONFIG_DRM_RCAR_DW_HDMI` | `is not set` |
| `CONFIG_DRM_RCAR_LVDS` | `is not set` |
| `CONFIG_DRM_TTM_DMA_PAGE_POOL` | `y` |
| `CONFIG_ECHO` | `is not set` |
| `CONFIG_EEPROM_LEGACY` | `is not set` |
| `CONFIG_EMBEDDED` | `is not set` |
| `CONFIG_ENABLE_MUST_CHECK` | `y` |
| `CONFIG_EXT3_FS` | `is not set` |
| `CONFIG_FB_ARK` | `is not set` |
| `CONFIG_FB_CMDLINE` | `y` |
| `CONFIG_FB_MATROX` | `is not set` |
| `CONFIG_FB_MODE_HELPERS` | `is not set` |
| `CONFIG_FB_S3` | `is not set` |
| `CONFIG_FB_SYS_FOPS` | `y` |
| `CONFIG_FB_VT8623` | `is not set` |
| `CONFIG_FIRMWARE_EDID` | `is not set` |
| `CONFIG_FIRMWARE_MEMMAP` | `is not set` |
| `CONFIG_FLATMEM` | `y` |
| `CONFIG_FLAT_NODE_MEM_MAP` | `y` |
| `CONFIG_FRONTSWAP` | `is not set` |
| `CONFIG_FS_DAX` | `is not set` |
| `CONFIG_GENERIC_MSI_IRQ_DOMAIN` | `y` |
| `CONFIG_GENERIC_PTDUMP` | `y` |
| `CONFIG_GUP_BENCHMARK` | `is not set` |
| `CONFIG_GVE` | `is not set` |
| `CONFIG_HABANA_AI` | `is not set` |
| `CONFIG_HANDLE_DOMAIN_IRQ` | `y` |
| `CONFIG_HAVE_CONTEXT_TRACKING` | `y` |
| `CONFIG_HAVE_DYNAMIC_FTRACE_WITH_REGS` | `y` |
| `CONFIG_HAVE_FTRACE_MCOUNT_RECORD` | `y` |
| `CONFIG_HAVE_FUTEX_CMPXCHG` | `y` |
| `CONFIG_HAVE_HARDENED_USERCOPY_ALLOCATOR` | `y` |
| `CONFIG_HAVE_MOD_ARCH_SPECIFIC` | `y` |
| `CONFIG_HAVE_NET_DSA` | `y` |
| `CONFIG_HID_APPLE` | `is not set` |
| `CONFIG_HOSTAP` | `is not set` |
| `CONFIG_HTC_PASIC3` | `is not set` |
| `CONFIG_HVC_RISCV_SBI` | `y` |
| `CONFIG_HW_CONSOLE` | `y` |
| `CONFIG_I2C_COMPAT` | `y` |
| `CONFIG_INPUT_EVBUG` | `is not set` |
| `CONFIG_INPUT_POLLDEV` | `is not set` |
| `CONFIG_IP_DCCP` | `is not set` |
| `CONFIG_IXGB` | `is not set` |
| `CONFIG_KALLSYMS_BASE_RELATIVE` | `y` |
| `CONFIG_KEYBOARD_ADP5589` | `is not set` |
| `CONFIG_KEYBOARD_MCS` | `is not set` |
| `CONFIG_KEYBOARD_TCA6416` | `is not set` |
| `CONFIG_LIBCRC32C` | `is not set` |
| `CONFIG_LOCKUP_DETECTOR` | `y` |
| `CONFIG_MANDATORY_FILE_LOCKING` | `y` |
| `CONFIG_MAXPHYSMEM_128GB` | `y` |
| `CONFIG_MAXPHYSMEM_2GB` | `is not set` |
| `CONFIG_MDIO_DEVICE` | `y` |
| `CONFIG_MDIO_DEVRES` | `y` |
| `CONFIG_MFD_INTEL_M10_BMC` | `is not set` |
| `CONFIG_MFD_PCF50633` | `is not set` |
| `CONFIG_MFD_RK808` | `is not set` |
| `CONFIG_MFD_ROHM_BD70528` | `is not set` |
| `CONFIG_MFD_SEC_CORE` | `is not set` |
| `CONFIG_MFD_SL28CPLD` | `is not set` |
| `CONFIG_MFD_TI_AM335X_TSCADC` | `is not set` |
| `CONFIG_MFD_TPS80031` | `is not set` |
| `CONFIG_MFD_WL1273_CORE` | `is not set` |
| `CONFIG_MICREL_KS8995MA` | `is not set` |
| `CONFIG_MICROCHIP_PIT64B` | `is not set` |
| `CONFIG_MMC_GOLDFISH` | `is not set` |
| `CONFIG_MODULE_SECTIONS` | `y` |
| `CONFIG_NET_VENDOR_AURORA` | `y` |
| `CONFIG_NVM` | `is not set` |
| `CONFIG_OF_NET` | `y` |
| `CONFIG_PAGE_OFFSET` | `0xffffffe000000000` |
| `CONFIG_PANIC_ON_OOPS_VALUE` | `0` |
| `CONFIG_PATA_PLATFORM` | `is not set` |
| `CONFIG_PA_BITS` | `56` |
| `CONFIG_PC104` | `is not set` |
| `CONFIG_PCIE_BUS_DEFAULT` | `y` |
| `CONFIG_PCIE_BUS_PEER2PEER` | `is not set` |
| `CONFIG_PCIE_BUS_PERFORMANCE` | `is not set` |
| `CONFIG_PCIE_BUS_SAFE` | `is not set` |
| `CONFIG_PCIE_BUS_TUNE_OFF` | `is not set` |
| `CONFIG_PCIE_BW` | `is not set` |
| `CONFIG_PCI_J721E_HOST` | `is not set` |
| `CONFIG_PCI_MSI_ARCH_FALLBACKS` | `y` |
| `CONFIG_PCI_MSI_IRQ_DOMAIN` | `y` |
| `CONFIG_PDA_POWER` | `is not set` |
| `CONFIG_PHY_FSL_IMX8MQ_USB` | `is not set` |
| `CONFIG_PHY_MIXEL_MIPI_DPHY` | `is not set` |
| `CONFIG_PREEMPT_COUNT` | `y` |
| `CONFIG_PRINTK_SAFE_LOG_BUF_SHIFT` | `13` |
| `CONFIG_PRISM54` | `is not set` |
| `CONFIG_RANDOM_TRUST_BOOTLOADER` | `is not set` |
| `CONFIG_RAW_DRIVER` | `is not set` |
| `CONFIG_REISERFS_FS` | `is not set` |
| `CONFIG_RTC_DRV_BQ4802` | `is not set` |
| `CONFIG_RTC_DRV_V3020` | `is not set` |
| `CONFIG_SCHED_DEBUG` | `y` |
| `CONFIG_SCSI_GDTH` | `is not set` |
| `CONFIG_SENSORS_ADM1021` | `is not set` |
| `CONFIG_SENSORS_ASPEED` | `is not set` |
| `CONFIG_SENSORS_LIS3_SPI` | `is not set` |
| `CONFIG_SENSORS_MAX6642` | `is not set` |
| `CONFIG_SENSORS_SMM665` | `is not set` |
| `CONFIG_SERIAL_8250_ASPEED_VUART` | `is not set` |
| `CONFIG_SERIAL_8250_DEPRECATED_OPTIONS` | `y` |
| `CONFIG_SGETMASK_SYSCALL` | `is not set` |
| `CONFIG_SIFIVE_L2` | `is not set` |
| `CONFIG_SLAB` | `is not set` |
| `CONFIG_SLOB` | `is not set` |
| `CONFIG_SOC_SIFIVE` | `y` |
| `CONFIG_SOC_VIRT` | `y` |
| `CONFIG_SPI_NXP_FLEXSPI` | `is not set` |
| `CONFIG_SPI_PXA2XX` | `is not set` |
| `CONFIG_SPI_ROCKCHIP` | `is not set` |
| `CONFIG_SPLIT_PTLOCK_CPUS` | `4` |
| `CONFIG_SRCU` | `y` |
| `CONFIG_STRING_SELFTEST` | `is not set` |
| `CONFIG_SYSFS_DEPRECATED` | `is not set` |
| `CONFIG_SYSV_FS` | `is not set` |
| `CONFIG_SYS_SUPPORTS_HUGETLBFS` | `y` |
| `CONFIG_TINYDRM_ST7586` | `is not set` |
| `CONFIG_TINYDRM_ST7735R` | `is not set` |
| `CONFIG_TRACE_SINK` | `is not set` |
| `CONFIG_TTY_PRINTK` | `is not set` |
| `CONFIG_UNINLINE_SPIN_UNLOCK` | `y` |
| `CONFIG_UNIX_SCM` | `y` |
| `CONFIG_UNUSED_SYMBOLS` | `is not set` |
| `CONFIG_USB_FOTG210_HCD` | `is not set` |
| `CONFIG_USB_FTDI_ELAN` | `is not set` |
| `CONFIG_USB_LGM_PHY` | `is not set` |
| `CONFIG_USELIB` | `is not set` |
| `CONFIG_VA_BITS` | `39` |
| `CONFIG_VGA_CONSOLE` | `y` |
| `CONFIG_VME_BUS` | `is not set` |
| `CONFIG_VXGE` | `is not set` |
| `CONFIG_WIMAX` | `is not set` |
| `CONFIG_WIRELESS_WDS` | `is not set` |
| `CONFIG_WLAN_VENDOR_CISCO` | `y` |
| `CONFIG_XILINX_AXI_EMAC` | `is not set` |
| `CONFIG_XZ_DEC_IA64` | `y` |
| `CONFIG_ZBUD` | `is not set` |
| `CONFIG_ZPOOL` | `is not set` |
| `CONFIG_ZSMALLOC` | `is not set` |

## 仅在 File 2 中存在 (3056 项)
| 配置项 | Value |
| :--- | :--- |
| `CONFIG_ABP060MG` | `is not set` |
| `CONFIG_ACLINT_SSWI` | `is not set` |
| `CONFIG_ACPI` | `y` |
| `CONFIG_ACPI_AC` | `y` |
| `CONFIG_ACPI_ALS` | `is not set` |
| `CONFIG_ACPI_BATTERY` | `y` |
| `CONFIG_ACPI_BGRT` | `is not set` |
| `CONFIG_ACPI_BUTTON` | `y` |
| `CONFIG_ACPI_CONFIGFS` | `is not set` |
| `CONFIG_ACPI_CONTAINER` | `is not set` |
| `CONFIG_ACPI_CPPC_CPUFREQ` | `m` |
| `CONFIG_ACPI_CPPC_CPUFREQ_FIE` | `y` |
| `CONFIG_ACPI_CPPC_LIB` | `y` |
| `CONFIG_ACPI_DEBUG` | `y` |
| `CONFIG_ACPI_DEBUGGER` | `is not set` |
| `CONFIG_ACPI_DOCK` | `is not set` |
| `CONFIG_ACPI_EC` | `is not set` |
| `CONFIG_ACPI_FAN` | `y` |
| `CONFIG_ACPI_FFH` | `is not set` |
| `CONFIG_ACPI_GENERIC_GSI` | `y` |
| `CONFIG_ACPI_HED` | `is not set` |
| `CONFIG_ACPI_I2C_OPREGION` | `y` |
| `CONFIG_ACPI_MCFG` | `y` |
| `CONFIG_ACPI_MDIO` | `y` |
| `CONFIG_ACPI_NFIT` | `is not set` |
| `CONFIG_ACPI_PCC` | `y` |
| `CONFIG_ACPI_PCI_SLOT` | `is not set` |
| `CONFIG_ACPI_PFRUT` | `is not set` |
| `CONFIG_ACPI_PPTT` | `y` |
| `CONFIG_ACPI_PROCESSOR` | `y` |
| `CONFIG_ACPI_PROCESSOR_IDLE` | `y` |
| `CONFIG_ACPI_REDUCED_HARDWARE_ONLY` | `y` |
| `CONFIG_ACPI_RIMT` | `y` |
| `CONFIG_ACPI_SPCR_TABLE` | `y` |
| `CONFIG_ACPI_TAD` | `is not set` |
| `CONFIG_ACPI_THERMAL` | `y` |
| `CONFIG_ACPI_THERMAL_LIB` | `y` |
| `CONFIG_ACPI_VIDEO` | `m` |
| `CONFIG_AD2S1200` | `is not set` |
| `CONFIG_AD2S1210` | `is not set` |
| `CONFIG_AD2S90` | `is not set` |
| `CONFIG_AD3530R` | `is not set` |
| `CONFIG_AD3552R` | `is not set` |
| `CONFIG_AD3552R_HS` | `is not set` |
| `CONFIG_AD4000` | `is not set` |
| `CONFIG_AD4030` | `is not set` |
| `CONFIG_AD4080` | `is not set` |
| `CONFIG_AD4130` | `is not set` |
| `CONFIG_AD4170_4` | `is not set` |
| `CONFIG_AD4695` | `is not set` |
| `CONFIG_AD4851` | `is not set` |
| `CONFIG_AD5064` | `is not set` |
| `CONFIG_AD5110` | `is not set` |
| `CONFIG_AD5272` | `is not set` |
| `CONFIG_AD5360` | `is not set` |
| `CONFIG_AD5380` | `is not set` |
| `CONFIG_AD5421` | `is not set` |
| `CONFIG_AD5446_I2C` | `is not set` |
| `CONFIG_AD5446_SPI` | `is not set` |
| `CONFIG_AD5449` | `is not set` |
| `CONFIG_AD5504` | `is not set` |
| `CONFIG_AD5592R` | `is not set` |
| `CONFIG_AD5593R` | `is not set` |
| `CONFIG_AD5624R_SPI` | `is not set` |
| `CONFIG_AD5686_SPI` | `is not set` |
| `CONFIG_AD5696_I2C` | `is not set` |
| `CONFIG_AD5755` | `is not set` |
| `CONFIG_AD5758` | `is not set` |
| `CONFIG_AD5761` | `is not set` |
| `CONFIG_AD5764` | `is not set` |
| `CONFIG_AD5766` | `is not set` |
| `CONFIG_AD5770R` | `is not set` |
| `CONFIG_AD5791` | `is not set` |
| `CONFIG_AD7091R5` | `is not set` |
| `CONFIG_AD7091R8` | `is not set` |
| `CONFIG_AD7124` | `is not set` |
| `CONFIG_AD7150` | `is not set` |
| `CONFIG_AD7173` | `is not set` |
| `CONFIG_AD7191` | `is not set` |
| `CONFIG_AD7192` | `is not set` |
| `CONFIG_AD7266` | `is not set` |
| `CONFIG_AD7280` | `is not set` |
| `CONFIG_AD7291` | `is not set` |
| `CONFIG_AD7292` | `is not set` |
| `CONFIG_AD7293` | `is not set` |
| `CONFIG_AD7298` | `is not set` |
| `CONFIG_AD7303` | `is not set` |
| `CONFIG_AD7380` | `is not set` |
| `CONFIG_AD74115` | `is not set` |
| `CONFIG_AD74413R` | `is not set` |
| `CONFIG_AD7476` | `is not set` |
| `CONFIG_AD7606_IFACE_PARALLEL` | `is not set` |
| `CONFIG_AD7606_IFACE_SPI` | `is not set` |
| `CONFIG_AD7625` | `is not set` |
| `CONFIG_AD7746` | `is not set` |
| `CONFIG_AD7766` | `is not set` |
| `CONFIG_AD7768_1` | `is not set` |
| `CONFIG_AD7779` | `is not set` |
| `CONFIG_AD7780` | `is not set` |
| `CONFIG_AD7791` | `is not set` |
| `CONFIG_AD7793` | `is not set` |
| `CONFIG_AD7887` | `is not set` |
| `CONFIG_AD7923` | `is not set` |
| `CONFIG_AD7944` | `is not set` |
| `CONFIG_AD7949` | `is not set` |
| `CONFIG_AD799X` | `is not set` |
| `CONFIG_AD8366` | `is not set` |
| `CONFIG_AD8460` | `is not set` |
| `CONFIG_AD8801` | `is not set` |
| `CONFIG_AD9467` | `is not set` |
| `CONFIG_AD9523` | `is not set` |
| `CONFIG_AD9739A` | `is not set` |
| `CONFIG_ADA4250` | `is not set` |
| `CONFIG_ADE9000` | `is not set` |
| `CONFIG_ADF4350` | `is not set` |
| `CONFIG_ADF4371` | `is not set` |
| `CONFIG_ADF4377` | `is not set` |
| `CONFIG_ADIN1100_PHY` | `is not set` |
| `CONFIG_ADIS16080` | `is not set` |
| `CONFIG_ADIS16130` | `is not set` |
| `CONFIG_ADIS16136` | `is not set` |
| `CONFIG_ADIS16201` | `is not set` |
| `CONFIG_ADIS16209` | `is not set` |
| `CONFIG_ADIS16260` | `is not set` |
| `CONFIG_ADIS16400` | `is not set` |
| `CONFIG_ADIS16460` | `is not set` |
| `CONFIG_ADIS16475` | `is not set` |
| `CONFIG_ADIS16480` | `is not set` |
| `CONFIG_ADIS16550` | `is not set` |
| `CONFIG_ADJD_S311` | `is not set` |
| `CONFIG_ADMFM2000` | `is not set` |
| `CONFIG_ADMV1013` | `is not set` |
| `CONFIG_ADMV1014` | `is not set` |
| `CONFIG_ADMV4420` | `is not set` |
| `CONFIG_ADMV8818` | `is not set` |
| `CONFIG_ADP810` | `is not set` |
| `CONFIG_ADRF6780` | `is not set` |
| `CONFIG_ADUX1020` | `is not set` |
| `CONFIG_ADXL313_I2C` | `is not set` |
| `CONFIG_ADXL313_SPI` | `is not set` |
| `CONFIG_ADXL345_I2C` | `is not set` |
| `CONFIG_ADXL345_SPI` | `is not set` |
| `CONFIG_ADXL355_I2C` | `is not set` |
| `CONFIG_ADXL355_SPI` | `is not set` |
| `CONFIG_ADXL367_I2C` | `is not set` |
| `CONFIG_ADXL367_SPI` | `is not set` |
| `CONFIG_ADXL372_I2C` | `is not set` |
| `CONFIG_ADXL372_SPI` | `is not set` |
| `CONFIG_ADXL380_I2C` | `is not set` |
| `CONFIG_ADXL380_SPI` | `is not set` |
| `CONFIG_ADXRS290` | `is not set` |
| `CONFIG_ADXRS450` | `is not set` |
| `CONFIG_AF8133J` | `is not set` |
| `CONFIG_AFE4403` | `is not set` |
| `CONFIG_AFE4404` | `is not set` |
| `CONFIG_AF_UNIX_OOB` | `y` |
| `CONFIG_AHCI_DWC` | `is not set` |
| `CONFIG_AHCI_SUNXI` | `is not set` |
| `CONFIG_AIR_EN8811H_PHY` | `is not set` |
| `CONFIG_AK09911` | `is not set` |
| `CONFIG_AK8974` | `is not set` |
| `CONFIG_AK8975` | `is not set` |
| `CONFIG_AL3000A` | `is not set` |
| `CONFIG_AL3010` | `is not set` |
| `CONFIG_AL3320A` | `is not set` |
| `CONFIG_ALIM7101_WDT` | `is not set` |
| `CONFIG_ALS31300` | `is not set` |
| `CONFIG_ALTERA_MBOX` | `is not set` |
| `CONFIG_ALTERA_MSGDMA` | `is not set` |
| `CONFIG_AM2315` | `is not set` |
| `CONFIG_AMBA_PL08X` | `is not set` |
| `CONFIG_AMD_QDMA` | `is not set` |
| `CONFIG_AMT` | `is not set` |
| `CONFIG_ANDROID_BINDER_IPC` | `is not set` |
| `CONFIG_ANON_VMA_NAME` | `is not set` |
| `CONFIG_AOSONG_AGS02MA` | `is not set` |
| `CONFIG_APDS9160` | `is not set` |
| `CONFIG_APDS9300` | `is not set` |
| `CONFIG_APDS9306` | `is not set` |
| `CONFIG_APDS9960` | `is not set` |
| `CONFIG_AQTION` | `is not set` |
| `CONFIG_ARCH_ANDES` | `y` |
| `CONFIG_ARCH_ANLOGIC` | `y` |
| `CONFIG_ARCH_CANAAN` | `y` |
| `CONFIG_ARCH_DEFAULT_CRASH_DUMP` | `y` |
| `CONFIG_ARCH_DMA_DEFAULT_COHERENT` | `y` |
| `CONFIG_ARCH_ENABLE_HUGEPAGE_MIGRATION` | `y` |
| `CONFIG_ARCH_ENABLE_MEMORY_HOTPLUG` | `y` |
| `CONFIG_ARCH_ENABLE_SPLIT_PMD_PTLOCK` | `y` |
| `CONFIG_ARCH_ESWIN` | `is not set` |
| `CONFIG_ARCH_HAS_CURRENT_STACK_POINTER` | `y` |
| `CONFIG_ARCH_HAS_DMA_PREP_COHERENT` | `y` |
| `CONFIG_ARCH_HAS_ELF_CORE_EFLAGS` | `y` |
| `CONFIG_ARCH_HAS_FAST_MULTIPLIER` | `y` |
| `CONFIG_ARCH_HAS_FORTIFY_SOURCE` | `y` |
| `CONFIG_ARCH_HAS_HW_PTE_YOUNG` | `y` |
| `CONFIG_ARCH_HAS_KERNEL_FPU_SUPPORT` | `y` |
| `CONFIG_ARCH_HAS_MEMBARRIER_CALLBACKS` | `y` |
| `CONFIG_ARCH_HAS_MEMBARRIER_SYNC_CORE` | `y` |
| `CONFIG_ARCH_HAS_NON_OVERLAPPING_ADDRESS_SPACE` | `y` |
| `CONFIG_ARCH_HAS_PMEM_API` | `y` |
| `CONFIG_ARCH_HAS_PREEMPT_LAZY` | `y` |
| `CONFIG_ARCH_HAS_PREPARE_SYNC_CORE_CMD` | `y` |
| `CONFIG_ARCH_HAS_PTDUMP` | `y` |
| `CONFIG_ARCH_HAS_SETUP_DMA_OPS` | `y` |
| `CONFIG_ARCH_HAS_STRICT_MODULE_RWX` | `y` |
| `CONFIG_ARCH_HAS_SYNC_CORE_BEFORE_USERMODE` | `y` |
| `CONFIG_ARCH_HAS_SYNC_DMA_FOR_CPU` | `y` |
| `CONFIG_ARCH_HAS_SYNC_DMA_FOR_DEVICE` | `y` |
| `CONFIG_ARCH_HAS_SYSCALL_WRAPPER` | `y` |
| `CONFIG_ARCH_HAS_TICK_BROADCAST` | `y` |
| `CONFIG_ARCH_HAS_UBSAN` | `y` |
| `CONFIG_ARCH_HAS_VDSO_ARCH_DATA` | `y` |
| `CONFIG_ARCH_HAVE_NMI_SAFE_CMPXCHG` | `y` |
| `CONFIG_ARCH_HIBERNATION_POSSIBLE` | `y` |
| `CONFIG_ARCH_KEEP_MEMBLOCK` | `y` |
| `CONFIG_ARCH_MHP_MEMMAP_ON_MEMORY_ENABLE` | `y` |
| `CONFIG_ARCH_MICROCHIP` | `y` |
| `CONFIG_ARCH_MICROCHIP_POLARFIRE` | `y` |
| `CONFIG_ARCH_MMAP_RND_COMPAT_BITS` | `8` |
| `CONFIG_ARCH_MMAP_RND_COMPAT_BITS_MAX` | `17` |
| `CONFIG_ARCH_MMAP_RND_COMPAT_BITS_MIN` | `8` |
| `CONFIG_ARCH_PROC_KCORE_TEXT` | `y` |
| `CONFIG_ARCH_RENESAS` | `is not set` |
| `CONFIG_ARCH_SIFIVE` | `y` |
| `CONFIG_ARCH_SOPHGO` | `y` |
| `CONFIG_ARCH_SPACEMIT` | `y` |
| `CONFIG_ARCH_STACKWALK` | `y` |
| `CONFIG_ARCH_STARFIVE` | `y` |
| `CONFIG_ARCH_SUNXI` | `y` |
| `CONFIG_ARCH_SUPPORTS_ACPI` | `y` |
| `CONFIG_ARCH_SUPPORTS_CFI` | `y` |
| `CONFIG_ARCH_SUPPORTS_CRASH_DUMP` | `y` |
| `CONFIG_ARCH_SUPPORTS_HUGETLBFS` | `y` |
| `CONFIG_ARCH_SUPPORTS_KEXEC` | `y` |
| `CONFIG_ARCH_SUPPORTS_KEXEC_FILE` | `y` |
| `CONFIG_ARCH_SUPPORTS_KEXEC_PURGATORY` | `y` |
| `CONFIG_ARCH_SUPPORTS_LTO_CLANG` | `y` |
| `CONFIG_ARCH_SUPPORTS_LTO_CLANG_THIN` | `y` |
| `CONFIG_ARCH_SUPPORTS_MSEAL_SYSTEM_MAPPINGS` | `y` |
| `CONFIG_ARCH_SUPPORTS_PAGE_TABLE_CHECK` | `y` |
| `CONFIG_ARCH_SUPPORTS_PER_VMA_LOCK` | `y` |
| `CONFIG_ARCH_SUPPORTS_RT` | `y` |
| `CONFIG_ARCH_SUPPORTS_SCHED_MC` | `y` |
| `CONFIG_ARCH_SUPPORTS_UPROBES` | `y` |
| `CONFIG_ARCH_SUSPEND_POSSIBLE` | `y` |
| `CONFIG_ARCH_TENSTORRENT` | `y` |
| `CONFIG_ARCH_THEAD` | `y` |
| `CONFIG_ARCH_USE_CMPXCHG_LOCKREF` | `y` |
| `CONFIG_ARCH_USE_MEMTEST` | `y` |
| `CONFIG_ARCH_USE_QUEUED_RWLOCKS` | `y` |
| `CONFIG_ARCH_USE_QUEUED_SPINLOCKS` | `y` |
| `CONFIG_ARCH_USE_SYM_ANNOTATIONS` | `y` |
| `CONFIG_ARCH_VIRT` | `y` |
| `CONFIG_ARCH_WANTS_NO_INSTR` | `y` |
| `CONFIG_ARCH_WANTS_THP_SWAP` | `y` |
| `CONFIG_ARCH_WANT_BATCHED_UNMAP_TLB_FLUSH` | `y` |
| `CONFIG_ARCH_WANT_LD_ORPHAN_WARN` | `y` |
| `CONFIG_ARCH_WANT_OPTIMIZE_DAX_VMEMMAP` | `y` |
| `CONFIG_ARCH_WANT_OPTIMIZE_HUGETLB_VMEMMAP` | `y` |
| `CONFIG_ARCH_WEAK_RELEASE_ACQUIRE` | `y` |
| `CONFIG_ARM_ALLWINNER_SUN50I_CPUFREQ_NVMEM` | `is not set` |
| `CONFIG_ARM_AMBA` | `y` |
| `CONFIG_ARM_HISI_UNCORE_DEVFREQ` | `is not set` |
| `CONFIG_ARM_MHU` | `is not set` |
| `CONFIG_ARM_MHU_V2` | `is not set` |
| `CONFIG_ARM_SUN8I_A33_MBUS_DEVFREQ` | `is not set` |
| `CONFIG_AS21XXX_PHY` | `is not set` |
| `CONFIG_AS3935` | `is not set` |
| `CONFIG_AS73211` | `is not set` |
| `CONFIG_ASN1` | `m` |
| `CONFIG_ASYNC_TX_DMA` | `is not set` |
| `CONFIG_AS_HAS_INSN` | `y` |
| `CONFIG_AS_HAS_NON_CONST_ULEB128` | `y` |
| `CONFIG_AS_HAS_OPTION_ARCH` | `y` |
| `CONFIG_AS_HAS_ULEB128` | `y` |
| `CONFIG_AS_IS_GNU` | `y` |
| `CONFIG_AS_VERSION` | `24301` |
| `CONFIG_AT803X_PHY` | `is not set` |
| `CONFIG_ATA_ACPI` | `y` |
| `CONFIG_ATLAS_EZO_SENSOR` | `is not set` |
| `CONFIG_ATLAS_PH_SENSOR` | `is not set` |
| `CONFIG_AT_XDMAC` | `is not set` |
| `CONFIG_AUDITSYSCALL` | `y` |
| `CONFIG_AUDIT_GENERIC` | `y` |
| `CONFIG_AUXILIARY_BUS` | `y` |
| `CONFIG_AW96103` | `is not set` |
| `CONFIG_AX45MP_L2_CACHE` | `is not set` |
| `CONFIG_AXP20X_ADC` | `is not set` |
| `CONFIG_AXP20X_POWER` | `is not set` |
| `CONFIG_AXP288_ADC` | `is not set` |
| `CONFIG_BACKLIGHT_AW99706` | `is not set` |
| `CONFIG_BACKLIGHT_GPIO` | `is not set` |
| `CONFIG_BACKLIGHT_KTD253` | `is not set` |
| `CONFIG_BACKLIGHT_KTD2801` | `is not set` |
| `CONFIG_BACKLIGHT_KTZ8866` | `is not set` |
| `CONFIG_BACKLIGHT_LM3509` | `is not set` |
| `CONFIG_BACKLIGHT_LM3630A` | `is not set` |
| `CONFIG_BACKLIGHT_LP855X` | `is not set` |
| `CONFIG_BACKLIGHT_MP3309C` | `is not set` |
| `CONFIG_BACKLIGHT_PWM` | `is not set` |
| `CONFIG_BATTERY_MAX1720X` | `is not set` |
| `CONFIG_BATTERY_RT5033` | `is not set` |
| `CONFIG_BATTERY_SAMSUNG_SDI` | `is not set` |
| `CONFIG_BATTERY_UG3105` | `is not set` |
| `CONFIG_BCACHE` | `is not set` |
| `CONFIG_BCM_VK` | `is not set` |
| `CONFIG_BD79703` | `is not set` |
| `CONFIG_BFQ_CGROUP_DEBUG` | `is not set` |
| `CONFIG_BFQ_GROUP_IOSCHED` | `y` |
| `CONFIG_BH1745` | `is not set` |
| `CONFIG_BH1750` | `is not set` |
| `CONFIG_BH1780` | `is not set` |
| `CONFIG_BIG_KEYS` | `is not set` |
| `CONFIG_BINARY_PRINTF` | `y` |
| `CONFIG_BLK_CGROUP_IOCOST` | `is not set` |
| `CONFIG_BLK_CGROUP_IOLATENCY` | `is not set` |
| `CONFIG_BLK_CGROUP_IOPRIO` | `is not set` |
| `CONFIG_BLK_CGROUP_PUNT_BIO` | `y` |
| `CONFIG_BLK_CGROUP_RWSTAT` | `y` |
| `CONFIG_BLK_DEV_BSG_COMMON` | `y` |
| `CONFIG_BLK_DEV_DM` | `m` |
| `CONFIG_BLK_DEV_DM_BUILTIN` | `y` |
| `CONFIG_BLK_DEV_IO_TRACE` | `is not set` |
| `CONFIG_BLK_DEV_MD` | `is not set` |
| `CONFIG_BLK_DEV_PMEM` | `y` |
| `CONFIG_BLK_DEV_THROTTLING` | `y` |
| `CONFIG_BLK_DEV_UBLK` | `is not set` |
| `CONFIG_BLK_DEV_WRITE_MOUNTED` | `y` |
| `CONFIG_BLK_ICQ` | `y` |
| `CONFIG_BLK_MQ_STACKING` | `y` |
| `CONFIG_BLK_PM` | `y` |
| `CONFIG_BLOCK_HOLDER_DEPRECATED` | `y` |
| `CONFIG_BLOCK_LEGACY_AUTOLOAD` | `y` |
| `CONFIG_BMA180` | `is not set` |
| `CONFIG_BMA220` | `is not set` |
| `CONFIG_BMA400` | `is not set` |
| `CONFIG_BMC150_ACCEL` | `is not set` |
| `CONFIG_BMC150_MAGN_I2C` | `is not set` |
| `CONFIG_BMC150_MAGN_SPI` | `is not set` |
| `CONFIG_BME680` | `is not set` |
| `CONFIG_BMG160` | `is not set` |
| `CONFIG_BMI088_ACCEL` | `is not set` |
| `CONFIG_BMI160_I2C` | `is not set` |
| `CONFIG_BMI160_SPI` | `is not set` |
| `CONFIG_BMI270_I2C` | `is not set` |
| `CONFIG_BMI270_SPI` | `is not set` |
| `CONFIG_BMI323_I2C` | `is not set` |
| `CONFIG_BMI323_SPI` | `is not set` |
| `CONFIG_BMP280` | `is not set` |
| `CONFIG_BNGE` | `is not set` |
| `CONFIG_BOOTTIME_TRACING` | `is not set` |
| `CONFIG_BOSCH_BNO055_I2C` | `is not set` |
| `CONFIG_BPF_EVENTS` | `y` |
| `CONFIG_BPF_UNPRIV_DEFAULT_OFF` | `y` |
| `CONFIG_BRANCH_PROFILE_NONE` | `y` |
| `CONFIG_BRIDGE_CFM` | `is not set` |
| `CONFIG_BRIDGE_IGMP_SNOOPING` | `y` |
| `CONFIG_BRIDGE_MRP` | `is not set` |
| `CONFIG_BRIDGE_NETFILTER` | `m` |
| `CONFIG_BRIDGE_NF_EBTABLES` | `is not set` |
| `CONFIG_BRIDGE_VLAN_FILTERING` | `y` |
| `CONFIG_BTRFS_ASSERT` | `is not set` |
| `CONFIG_BTRFS_DEBUG` | `is not set` |
| `CONFIG_BTRFS_EXPERIMENTAL` | `is not set` |
| `CONFIG_BTRFS_FS_POSIX_ACL` | `y` |
| `CONFIG_BTRFS_FS_RUN_SANITY_TESTS` | `is not set` |
| `CONFIG_BTT` | `y` |
| `CONFIG_BUFFER_HEAD` | `y` |
| `CONFIG_BUILDTIME_TABLE_SORT` | `y` |
| `CONFIG_CACHEMAINT_FOR_DMA` | `y` |
| `CONFIG_CACHESTAT_SYSCALL` | `y` |
| `CONFIG_CADENCE_WATCHDOG` | `is not set` |
| `CONFIG_CAN_8DEV_USB` | `is not set` |
| `CONFIG_CAN_BCM` | `m` |
| `CONFIG_CAN_CALC_BITTIMING` | `y` |
| `CONFIG_CAN_CAN327` | `is not set` |
| `CONFIG_CAN_CC770` | `is not set` |
| `CONFIG_CAN_CTUCANFD_PCI` | `is not set` |
| `CONFIG_CAN_CTUCANFD_PLATFORM` | `is not set` |
| `CONFIG_CAN_C_CAN` | `is not set` |
| `CONFIG_CAN_DEBUG_DEVICES` | `is not set` |
| `CONFIG_CAN_DEV` | `m` |
| `CONFIG_CAN_DUMMY` | `is not set` |
| `CONFIG_CAN_EMS_USB` | `is not set` |
| `CONFIG_CAN_ESD_402_PCI` | `is not set` |
| `CONFIG_CAN_ESD_USB` | `is not set` |
| `CONFIG_CAN_ETAS_ES58X` | `is not set` |
| `CONFIG_CAN_F81604` | `is not set` |
| `CONFIG_CAN_FLEXCAN` | `is not set` |
| `CONFIG_CAN_GRCAN` | `is not set` |
| `CONFIG_CAN_GS_USB` | `is not set` |
| `CONFIG_CAN_GW` | `m` |
| `CONFIG_CAN_HI311X` | `is not set` |
| `CONFIG_CAN_IFI_CANFD` | `is not set` |
| `CONFIG_CAN_ISOTP` | `is not set` |
| `CONFIG_CAN_J1939` | `is not set` |
| `CONFIG_CAN_KVASER_PCIEFD` | `is not set` |
| `CONFIG_CAN_KVASER_USB` | `is not set` |
| `CONFIG_CAN_MCBA_USB` | `is not set` |
| `CONFIG_CAN_MCP251X` | `is not set` |
| `CONFIG_CAN_MCP251XFD` | `is not set` |
| `CONFIG_CAN_M_CAN` | `is not set` |
| `CONFIG_CAN_NETLINK` | `y` |
| `CONFIG_CAN_PEAK_PCIEFD` | `is not set` |
| `CONFIG_CAN_PEAK_USB` | `is not set` |
| `CONFIG_CAN_RAW` | `m` |
| `CONFIG_CAN_SJA1000` | `is not set` |
| `CONFIG_CAN_SLCAN` | `is not set` |
| `CONFIG_CAN_SOFTING` | `is not set` |
| `CONFIG_CAN_SUN4I` | `is not set` |
| `CONFIG_CAN_UCAN` | `is not set` |
| `CONFIG_CAN_VCAN` | `is not set` |
| `CONFIG_CAN_VXCAN` | `is not set` |
| `CONFIG_CC10001_ADC` | `is not set` |
| `CONFIG_CCS811` | `is not set` |
| `CONFIG_CC_HAS_ASM_GOTO_TIED_OUTPUT` | `y` |
| `CONFIG_CC_HAS_ASSUME` | `y` |
| `CONFIG_CC_HAS_AUTO_VAR_INIT_ZERO_BARE` | `y` |
| `CONFIG_CC_HAS_MIN_FUNCTION_ALIGNMENT` | `y` |
| `CONFIG_CC_HAS_NO_PROFILE_FN_ATTR` | `y` |
| `CONFIG_CC_HAS_SANE_FUNCTION_ALIGNMENT` | `y` |
| `CONFIG_CC_HAS_ZERO_CALL_USED_REGS` | `y` |
| `CONFIG_CC_HAVE_STACKPROTECTOR_TLS` | `y` |
| `CONFIG_CC_IMPLICIT_FALLTHROUGH` | `"-Wimplicit-fallthrough=5"` |
| `CONFIG_CC_NO_ARRAY_BOUNDS` | `y` |
| `CONFIG_CC_NO_STRINGOP_OVERFLOW` | `y` |
| `CONFIG_CGROUP_DMEM` | `is not set` |
| `CONFIG_CGROUP_FAVOR_DYNMODS` | `is not set` |
| `CONFIG_CGROUP_HUGETLB` | `y` |
| `CONFIG_CGROUP_MISC` | `is not set` |
| `CONFIG_CGROUP_PERF` | `y` |
| `CONFIG_CGROUP_WRITEBACK` | `y` |
| `CONFIG_CHARGER_BQ24190` | `is not set` |
| `CONFIG_CHARGER_BQ24257` | `is not set` |
| `CONFIG_CHARGER_BQ24735` | `is not set` |
| `CONFIG_CHARGER_BQ2515X` | `is not set` |
| `CONFIG_CHARGER_BQ256XX` | `is not set` |
| `CONFIG_CHARGER_BQ25890` | `is not set` |
| `CONFIG_CHARGER_BQ25980` | `is not set` |
| `CONFIG_CHARGER_GPIO` | `is not set` |
| `CONFIG_CHARGER_ISP1704` | `is not set` |
| `CONFIG_CHARGER_LT3651` | `is not set` |
| `CONFIG_CHARGER_LTC4162L` | `is not set` |
| `CONFIG_CHARGER_MANAGER` | `is not set` |
| `CONFIG_CHARGER_MAX77976` | `is not set` |
| `CONFIG_CHARGER_MAX8971` | `is not set` |
| `CONFIG_CHARGER_RT9455` | `is not set` |
| `CONFIG_CHARGER_RT9467` | `is not set` |
| `CONFIG_CHARGER_RT9471` | `is not set` |
| `CONFIG_CHARGER_RT9756` | `is not set` |
| `CONFIG_CHARGER_UCS1002` | `is not set` |
| `CONFIG_CLKSRC_MMIO` | `y` |
| `CONFIG_CLK_SIFIVE_PRCI` | `y` |
| `CONFIG_CLK_SOPHGO_CV1800` | `y` |
| `CONFIG_CLK_SOPHGO_SG2042_CLKGEN` | `y` |
| `CONFIG_CLK_SOPHGO_SG2042_PLL` | `y` |
| `CONFIG_CLK_SOPHGO_SG2042_RPGATE` | `y` |
| `CONFIG_CLK_SOPHGO_SG2044` | `is not set` |
| `CONFIG_CLK_SOPHGO_SG2044_PLL` | `is not set` |
| `CONFIG_CLK_STARFIVE_JH7100` | `y` |
| `CONFIG_CLK_STARFIVE_JH7100_AUDIO` | `m` |
| `CONFIG_CLK_STARFIVE_JH7110_AON` | `m` |
| `CONFIG_CLK_STARFIVE_JH7110_ISP` | `m` |
| `CONFIG_CLK_STARFIVE_JH7110_PLL` | `y` |
| `CONFIG_CLK_STARFIVE_JH7110_STG` | `m` |
| `CONFIG_CLK_STARFIVE_JH7110_SYS` | `y` |
| `CONFIG_CLK_STARFIVE_JH7110_VOUT` | `m` |
| `CONFIG_CLK_STARFIVE_JH71X0` | `y` |
| `CONFIG_CLK_THEAD_TH1520_AP` | `y` |
| `CONFIG_CLZ_TAB` | `y` |
| `CONFIG_CM32181` | `is not set` |
| `CONFIG_CM3232` | `is not set` |
| `CONFIG_CM3323` | `is not set` |
| `CONFIG_CM3605` | `is not set` |
| `CONFIG_CM36651` | `is not set` |
| `CONFIG_CMDLINE_LOG_WRAP_IDEAL_LEN` | `1021` |
| `CONFIG_COMEDI` | `is not set` |
| `CONFIG_COMMON_CLK_AXI_CLKGEN` | `is not set` |
| `CONFIG_COMMON_CLK_PWM` | `is not set` |
| `CONFIG_COMMON_CLK_RPMI` | `y` |
| `CONFIG_COMMON_CLK_RS9_PCIE` | `is not set` |
| `CONFIG_COMMON_CLK_SI521XX` | `is not set` |
| `CONFIG_COMMON_CLK_VC3` | `is not set` |
| `CONFIG_COMMON_CLK_VC7` | `is not set` |
| `CONFIG_COMMON_CLK_XLNX_CLKWZRD` | `is not set` |
| `CONFIG_COMPACT_UNEVICTABLE_DEFAULT` | `1` |
| `CONFIG_COMPAT` | `y` |
| `CONFIG_COMPAT_BINFMT_ELF` | `y` |
| `CONFIG_CONTEXT_SWITCH_TRACER` | `y` |
| `CONFIG_CONTEXT_TRACKING` | `y` |
| `CONFIG_CONTEXT_TRACKING_IDLE` | `y` |
| `CONFIG_CPUFREQ_DT` | `y` |
| `CONFIG_CPUFREQ_DT_PLATDEV` | `y` |
| `CONFIG_CPUFREQ_VIRT` | `is not set` |
| `CONFIG_CPUSETS_V1` | `is not set` |
| `CONFIG_CPU_FREQ` | `y` |
| `CONFIG_CPU_FREQ_DEFAULT_GOV_CONSERVATIVE` | `is not set` |
| `CONFIG_CPU_FREQ_DEFAULT_GOV_ONDEMAND` | `is not set` |
| `CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE` | `y` |
| `CONFIG_CPU_FREQ_DEFAULT_GOV_POWERSAVE` | `is not set` |
| `CONFIG_CPU_FREQ_DEFAULT_GOV_SCHEDUTIL` | `is not set` |
| `CONFIG_CPU_FREQ_DEFAULT_GOV_USERSPACE` | `is not set` |
| `CONFIG_CPU_FREQ_GOV_ATTR_SET` | `y` |
| `CONFIG_CPU_FREQ_GOV_COMMON` | `y` |
| `CONFIG_CPU_FREQ_GOV_CONSERVATIVE` | `m` |
| `CONFIG_CPU_FREQ_GOV_ONDEMAND` | `y` |
| `CONFIG_CPU_FREQ_GOV_PERFORMANCE` | `y` |
| `CONFIG_CPU_FREQ_GOV_POWERSAVE` | `m` |
| `CONFIG_CPU_FREQ_GOV_SCHEDUTIL` | `is not set` |
| `CONFIG_CPU_FREQ_GOV_USERSPACE` | `y` |
| `CONFIG_CPU_FREQ_STAT` | `y` |
| `CONFIG_CPU_FREQ_THERMAL` | `y` |
| `CONFIG_CPU_HOTPLUG_STATE_CONTROL` | `is not set` |
| `CONFIG_CPU_IDLE` | `y` |
| `CONFIG_CPU_IDLE_GOV_LADDER` | `is not set` |
| `CONFIG_CPU_IDLE_GOV_MENU` | `y` |
| `CONFIG_CPU_IDLE_GOV_TEO` | `is not set` |
| `CONFIG_CPU_IDLE_MULTIPLE_DRIVERS` | `y` |
| `CONFIG_CPU_MITIGATIONS` | `y` |
| `CONFIG_CPU_PM` | `y` |
| `CONFIG_CPU_THERMAL` | `y` |
| `CONFIG_CRC32_ARCH` | `y` |
| `CONFIG_CRC_OPTIMIZATIONS` | `y` |
| `CONFIG_CRYPTO_ACOMP2` | `m` |
| `CONFIG_CRYPTO_AKCIPHER` | `m` |
| `CONFIG_CRYPTO_AKCIPHER2` | `m` |
| `CONFIG_CRYPTO_ARIA` | `is not set` |
| `CONFIG_CRYPTO_BENCHMARK` | `is not set` |
| `CONFIG_CRYPTO_DEV_ALLWINNER` | `y` |
| `CONFIG_CRYPTO_DEV_ATMEL_AES` | `is not set` |
| `CONFIG_CRYPTO_DEV_QAT_420XX` | `is not set` |
| `CONFIG_CRYPTO_DEV_QAT_4XXX` | `is not set` |
| `CONFIG_CRYPTO_DEV_QAT_C3XXX` | `is not set` |
| `CONFIG_CRYPTO_DEV_QAT_C3XXXVF` | `is not set` |
| `CONFIG_CRYPTO_DEV_QAT_C62X` | `is not set` |
| `CONFIG_CRYPTO_DEV_QAT_C62XVF` | `is not set` |
| `CONFIG_CRYPTO_DEV_QAT_DH895xCC` | `is not set` |
| `CONFIG_CRYPTO_DEV_QAT_DH895xCCVF` | `is not set` |
| `CONFIG_CRYPTO_DEV_SUN4I_SS` | `is not set` |
| `CONFIG_CRYPTO_DEV_SUN8I_CE` | `is not set` |
| `CONFIG_CRYPTO_DEV_SUN8I_SS` | `is not set` |
| `CONFIG_CRYPTO_DRBG` | `m` |
| `CONFIG_CRYPTO_DRBG_CTR` | `is not set` |
| `CONFIG_CRYPTO_DRBG_HASH` | `is not set` |
| `CONFIG_CRYPTO_DRBG_HMAC` | `y` |
| `CONFIG_CRYPTO_ECDSA` | `is not set` |
| `CONFIG_CRYPTO_GENIV` | `m` |
| `CONFIG_CRYPTO_HCTR2` | `is not set` |
| `CONFIG_CRYPTO_JITTERENTROPY_MEMORY_BLOCKS` | `64` |
| `CONFIG_CRYPTO_JITTERENTROPY_MEMORY_BLOCKSIZE` | `32` |
| `CONFIG_CRYPTO_JITTERENTROPY_OSR` | `1` |
| `CONFIG_CRYPTO_KPP2` | `m` |
| `CONFIG_CRYPTO_KRB5` | `is not set` |
| `CONFIG_CRYPTO_KRB5ENC` | `is not set` |
| `CONFIG_CRYPTO_LIB_AES` | `m` |
| `CONFIG_CRYPTO_LIB_BLAKE2B` | `m` |
| `CONFIG_CRYPTO_LIB_GF128MUL` | `m` |
| `CONFIG_CRYPTO_LIB_SHA1` | `y` |
| `CONFIG_CRYPTO_LIB_SHA256` | `y` |
| `CONFIG_CRYPTO_LIB_SHA3` | `m` |
| `CONFIG_CRYPTO_LIB_SHA512` | `m` |
| `CONFIG_CRYPTO_LIB_UTILS` | `m` |
| `CONFIG_CRYPTO_MANAGER2` | `m` |
| `CONFIG_CRYPTO_RNG` | `m` |
| `CONFIG_CRYPTO_RNG_DEFAULT` | `m` |
| `CONFIG_CRYPTO_SIG` | `m` |
| `CONFIG_CRYPTO_SIG2` | `m` |
| `CONFIG_CRYPTO_SM3_GENERIC` | `is not set` |
| `CONFIG_CRYPTO_SM4_GENERIC` | `is not set` |
| `CONFIG_CV1800_MBOX` | `is not set` |
| `CONFIG_CXL_BUS` | `is not set` |
| `CONFIG_D3323AA` | `is not set` |
| `CONFIG_DA280` | `is not set` |
| `CONFIG_DA311` | `is not set` |
| `CONFIG_DAMON` | `is not set` |
| `CONFIG_DEBUG_BUGVERBOSE_DETAILED` | `is not set` |
| `CONFIG_DEBUG_GPIO` | `is not set` |
| `CONFIG_DEBUG_INFO_DWARF4` | `is not set` |
| `CONFIG_DEBUG_INFO_DWARF5` | `is not set` |
| `CONFIG_DEBUG_INFO_DWARF_TOOLCHAIN_DEFAULT` | `is not set` |
| `CONFIG_DEBUG_INFO_NONE` | `y` |
| `CONFIG_DEBUG_IRQFLAGS` | `is not set` |
| `CONFIG_DEBUG_MAPLE_TREE` | `is not set` |
| `CONFIG_DEBUG_NET` | `is not set` |
| `CONFIG_DEBUG_NET_SMALL_RTNL` | `is not set` |
| `CONFIG_DEBUG_PAGE_REF` | `is not set` |
| `CONFIG_DEBUG_PERF_USE_VMALLOC` | `is not set` |
| `CONFIG_DEBUG_PINCTRL` | `is not set` |
| `CONFIG_DEBUG_VFS` | `is not set` |
| `CONFIG_DEFAULT_SECURITY_APPARMOR` | `is not set` |
| `CONFIG_DEFAULT_SECURITY_SELINUX` | `is not set` |
| `CONFIG_DEFERRED_STRUCT_PAGE_INIT` | `is not set` |
| `CONFIG_DEVFREQ_GOV_PASSIVE` | `is not set` |
| `CONFIG_DEVFREQ_GOV_PERFORMANCE` | `is not set` |
| `CONFIG_DEVFREQ_GOV_POWERSAVE` | `is not set` |
| `CONFIG_DEVFREQ_GOV_SIMPLE_ONDEMAND` | `is not set` |
| `CONFIG_DEVFREQ_GOV_USERSPACE` | `is not set` |
| `CONFIG_DEVFREQ_THERMAL` | `y` |
| `CONFIG_DEVTMPFS_SAFE` | `is not set` |
| `CONFIG_DHT11` | `is not set` |
| `CONFIG_DIBS` | `is not set` |
| `CONFIG_DIMLIB` | `y` |
| `CONFIG_DLHL60D` | `is not set` |
| `CONFIG_DLM` | `is not set` |
| `CONFIG_DM9051` | `is not set` |
| `CONFIG_DMABUF_DEBUG` | `is not set` |
| `CONFIG_DMABUF_SYSFS_STATS` | `is not set` |
| `CONFIG_DMADEVICES_DEBUG` | `is not set` |
| `CONFIG_DMAPOOL_TEST` | `is not set` |
| `CONFIG_DMARD06` | `is not set` |
| `CONFIG_DMARD09` | `is not set` |
| `CONFIG_DMARD10` | `is not set` |
| `CONFIG_DMATEST` | `is not set` |
| `CONFIG_DMA_ACPI` | `y` |
| `CONFIG_DMA_BOUNCE_UNALIGNED_KMALLOC` | `y` |
| `CONFIG_DMA_COHERENT_POOL` | `y` |
| `CONFIG_DMA_DIRECT_REMAP` | `y` |
| `CONFIG_DMA_ENGINE` | `y` |
| `CONFIG_DMA_MAP_BENCHMARK` | `is not set` |
| `CONFIG_DMA_NEED_SYNC` | `y` |
| `CONFIG_DMA_NONCOHERENT_MMAP` | `y` |
| `CONFIG_DMA_OF` | `y` |
| `CONFIG_DMA_RESTRICTED_POOL` | `is not set` |
| `CONFIG_DMA_SUN6I` | `m` |
| `CONFIG_DMA_VIRTUAL_CHANNELS` | `y` |
| `CONFIG_DMI` | `y` |
| `CONFIG_DMIID` | `y` |
| `CONFIG_DMI_SYSFS` | `is not set` |
| `CONFIG_DM_AUDIT` | `is not set` |
| `CONFIG_DM_BIO_PRISON` | `m` |
| `CONFIG_DM_BUFIO` | `m` |
| `CONFIG_DM_CACHE` | `is not set` |
| `CONFIG_DM_CLONE` | `is not set` |
| `CONFIG_DM_CRYPT` | `is not set` |
| `CONFIG_DM_DEBUG` | `is not set` |
| `CONFIG_DM_DEBUG_BLOCK_MANAGER_LOCKING` | `is not set` |
| `CONFIG_DM_DELAY` | `is not set` |
| `CONFIG_DM_DUST` | `is not set` |
| `CONFIG_DM_EBS` | `is not set` |
| `CONFIG_DM_ERA` | `is not set` |
| `CONFIG_DM_FLAKEY` | `is not set` |
| `CONFIG_DM_INTEGRITY` | `is not set` |
| `CONFIG_DM_LOG_WRITES` | `is not set` |
| `CONFIG_DM_MIRROR` | `is not set` |
| `CONFIG_DM_MULTIPATH` | `is not set` |
| `CONFIG_DM_PERSISTENT_DATA` | `m` |
| `CONFIG_DM_RAID` | `is not set` |
| `CONFIG_DM_SNAPSHOT` | `is not set` |
| `CONFIG_DM_SWITCH` | `is not set` |
| `CONFIG_DM_THIN_PROVISIONING` | `m` |
| `CONFIG_DM_UEVENT` | `is not set` |
| `CONFIG_DM_UNSTRIPED` | `is not set` |
| `CONFIG_DM_VDO` | `is not set` |
| `CONFIG_DM_VERITY` | `is not set` |
| `CONFIG_DM_WRITECACHE` | `is not set` |
| `CONFIG_DM_ZERO` | `is not set` |
| `CONFIG_DP83TD510_PHY` | `is not set` |
| `CONFIG_DP83TG720_PHY` | `is not set` |
| `CONFIG_DPOT_DAC` | `is not set` |
| `CONFIG_DPS310` | `is not set` |
| `CONFIG_DRM_ACCEL` | `is not set` |
| `CONFIG_DRM_ANALOGIX_ANX7625` | `is not set` |
| `CONFIG_DRM_CHIPONE_ICN6211` | `is not set` |
| `CONFIG_DRM_CLIENT` | `y` |
| `CONFIG_DRM_CLIENT_DEFAULT` | `"fbdev"` |
| `CONFIG_DRM_CLIENT_DEFAULT_FBDEV` | `y` |
| `CONFIG_DRM_CLIENT_LIB` | `m` |
| `CONFIG_DRM_CLIENT_LOG` | `is not set` |
| `CONFIG_DRM_CLIENT_SELECTION` | `m` |
| `CONFIG_DRM_CLIENT_SETUP` | `y` |
| `CONFIG_DRM_DISPLAY_DP_AUX_CEC` | `is not set` |
| `CONFIG_DRM_DISPLAY_DP_AUX_CHARDEV` | `is not set` |
| `CONFIG_DRM_DISPLAY_DP_HELPER` | `y` |
| `CONFIG_DRM_DISPLAY_HDMI_HELPER` | `y` |
| `CONFIG_DRM_DISPLAY_HELPER` | `m` |
| `CONFIG_DRM_DW_HDMI` | `m` |
| `CONFIG_DRM_DW_HDMI_AHB_AUDIO` | `is not set` |
| `CONFIG_DRM_DW_HDMI_CEC` | `is not set` |
| `CONFIG_DRM_DW_HDMI_GP_AUDIO` | `is not set` |
| `CONFIG_DRM_DW_HDMI_I2S_AUDIO` | `is not set` |
| `CONFIG_DRM_EFIDRM` | `is not set` |
| `CONFIG_DRM_EXEC` | `m` |
| `CONFIG_DRM_GEM_DMA_HELPER` | `m` |
| `CONFIG_DRM_GPUVM` | `m` |
| `CONFIG_DRM_GUD` | `is not set` |
| `CONFIG_DRM_HISI_HIBMC` | `is not set` |
| `CONFIG_DRM_ITE_IT6263` | `is not set` |
| `CONFIG_DRM_ITE_IT6505` | `is not set` |
| `CONFIG_DRM_ITE_IT66121` | `is not set` |
| `CONFIG_DRM_LOGICVC` | `is not set` |
| `CONFIG_DRM_LONTIUM_LT8912B` | `is not set` |
| `CONFIG_DRM_LONTIUM_LT9211` | `is not set` |
| `CONFIG_DRM_LONTIUM_LT9611UXC` | `is not set` |
| `CONFIG_DRM_MIPI_DSI` | `y` |
| `CONFIG_DRM_NOUVEAU_BACKLIGHT` | `y` |
| `CONFIG_DRM_NOUVEAU_CH7006` | `m` |
| `CONFIG_DRM_NOUVEAU_SIL164` | `m` |
| `CONFIG_DRM_PANEL_ABT_Y030XX067A` | `is not set` |
| `CONFIG_DRM_PANEL_ASUS_Z00T_TM5P5_NT35596` | `is not set` |
| `CONFIG_DRM_PANEL_AUO_A030JTN01` | `is not set` |
| `CONFIG_DRM_PANEL_BOE_BF060Y8M_AJ0` | `is not set` |
| `CONFIG_DRM_PANEL_BOE_HIMAX8279D` | `is not set` |
| `CONFIG_DRM_PANEL_BOE_TD4320` | `is not set` |
| `CONFIG_DRM_PANEL_BOE_TH101MB31UIG002_28A` | `is not set` |
| `CONFIG_DRM_PANEL_BOE_TV101WUM_LL2` | `is not set` |
| `CONFIG_DRM_PANEL_BOE_TV101WUM_NL6` | `is not set` |
| `CONFIG_DRM_PANEL_DSI_CM` | `is not set` |
| `CONFIG_DRM_PANEL_EBBG_FT8719` | `is not set` |
| `CONFIG_DRM_PANEL_EDP` | `is not set` |
| `CONFIG_DRM_PANEL_ELIDA_KD35T133` | `is not set` |
| `CONFIG_DRM_PANEL_FEIXIN_K101_IM2BA02` | `is not set` |
| `CONFIG_DRM_PANEL_FEIYANG_FY07024DI26A30D` | `is not set` |
| `CONFIG_DRM_PANEL_HIMAX_HX8279` | `is not set` |
| `CONFIG_DRM_PANEL_HIMAX_HX83102` | `is not set` |
| `CONFIG_DRM_PANEL_HIMAX_HX83112A` | `is not set` |
| `CONFIG_DRM_PANEL_HIMAX_HX83112B` | `is not set` |
| `CONFIG_DRM_PANEL_HIMAX_HX8394` | `is not set` |
| `CONFIG_DRM_PANEL_HYDIS_HV101HD1` | `is not set` |
| `CONFIG_DRM_PANEL_ILITEK_ILI9341` | `is not set` |
| `CONFIG_DRM_PANEL_ILITEK_ILI9805` | `is not set` |
| `CONFIG_DRM_PANEL_ILITEK_ILI9806E` | `is not set` |
| `CONFIG_DRM_PANEL_ILITEK_ILI9881C` | `is not set` |
| `CONFIG_DRM_PANEL_ILITEK_ILI9882T` | `is not set` |
| `CONFIG_DRM_PANEL_INNOLUX_EJ030NA` | `is not set` |
| `CONFIG_DRM_PANEL_INNOLUX_P079ZCA` | `is not set` |
| `CONFIG_DRM_PANEL_JADARD_JD9365DA_H3` | `is not set` |
| `CONFIG_DRM_PANEL_JDI_LPM102A188A` | `is not set` |
| `CONFIG_DRM_PANEL_JDI_LT070ME05000` | `is not set` |
| `CONFIG_DRM_PANEL_JDI_R63452` | `is not set` |
| `CONFIG_DRM_PANEL_KHADAS_TS050` | `is not set` |
| `CONFIG_DRM_PANEL_KINGDISPLAY_KD097D04` | `is not set` |
| `CONFIG_DRM_PANEL_LEADTEK_LTK050H3146W` | `is not set` |
| `CONFIG_DRM_PANEL_LEADTEK_LTK500HD1829` | `is not set` |
| `CONFIG_DRM_PANEL_LG_LB035Q02` | `is not set` |
| `CONFIG_DRM_PANEL_LG_LD070WX3` | `is not set` |
| `CONFIG_DRM_PANEL_LG_SW43408` | `is not set` |
| `CONFIG_DRM_PANEL_LINCOLNTECH_LCD197` | `is not set` |
| `CONFIG_DRM_PANEL_MAGNACHIP_D53E6EA8966` | `is not set` |
| `CONFIG_DRM_PANEL_MANTIX_MLAF057WE51` | `is not set` |
| `CONFIG_DRM_PANEL_MIPI_DBI` | `is not set` |
| `CONFIG_DRM_PANEL_NEC_NL8048HL11` | `is not set` |
| `CONFIG_DRM_PANEL_NEWVISION_NV3051D` | `is not set` |
| `CONFIG_DRM_PANEL_NEWVISION_NV3052C` | `is not set` |
| `CONFIG_DRM_PANEL_NOVATEK_NT35510` | `is not set` |
| `CONFIG_DRM_PANEL_NOVATEK_NT35560` | `is not set` |
| `CONFIG_DRM_PANEL_NOVATEK_NT35950` | `is not set` |
| `CONFIG_DRM_PANEL_NOVATEK_NT36523` | `is not set` |
| `CONFIG_DRM_PANEL_NOVATEK_NT36672A` | `is not set` |
| `CONFIG_DRM_PANEL_NOVATEK_NT36672E` | `is not set` |
| `CONFIG_DRM_PANEL_NOVATEK_NT37801` | `is not set` |
| `CONFIG_DRM_PANEL_ORISETECH_OTA5601A` | `is not set` |
| `CONFIG_DRM_PANEL_ORISETECH_OTM8009A` | `is not set` |
| `CONFIG_DRM_PANEL_OSD_OSD101T2587_53TS` | `is not set` |
| `CONFIG_DRM_PANEL_PANASONIC_VVX10F034N00` | `is not set` |
| `CONFIG_DRM_PANEL_RASPBERRYPI_TOUCHSCREEN` | `is not set` |
| `CONFIG_DRM_PANEL_RAYDIUM_RM67191` | `is not set` |
| `CONFIG_DRM_PANEL_RAYDIUM_RM67200` | `is not set` |
| `CONFIG_DRM_PANEL_RAYDIUM_RM68200` | `is not set` |
| `CONFIG_DRM_PANEL_RAYDIUM_RM692E5` | `is not set` |
| `CONFIG_DRM_PANEL_RAYDIUM_RM69380` | `is not set` |
| `CONFIG_DRM_PANEL_RENESAS_R61307` | `is not set` |
| `CONFIG_DRM_PANEL_RENESAS_R69328` | `is not set` |
| `CONFIG_DRM_PANEL_RONBO_RB070D30` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_AMS581VF01` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_AMS639RQ08` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_ATNA33XC20` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_DB7430` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6D16D0` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6D27A1` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6D7AA0` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6E3FA7` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6E3FC2X01` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6E3HA2` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6E3HA8` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6E63J0X03` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6E88A0_AMS427AP24` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_S6E8AA5X01_AMS561RA01` | `is not set` |
| `CONFIG_DRM_PANEL_SAMSUNG_SOFEF00` | `is not set` |
| `CONFIG_DRM_PANEL_SHARP_LQ079L1SX01` | `is not set` |
| `CONFIG_DRM_PANEL_SHARP_LQ101R1SX01` | `is not set` |
| `CONFIG_DRM_PANEL_SHARP_LS037V7DW01` | `is not set` |
| `CONFIG_DRM_PANEL_SHARP_LS043T1LE01` | `is not set` |
| `CONFIG_DRM_PANEL_SHARP_LS060T1SX01` | `is not set` |
| `CONFIG_DRM_PANEL_SITRONIX_ST7701` | `is not set` |
| `CONFIG_DRM_PANEL_SITRONIX_ST7703` | `is not set` |
| `CONFIG_DRM_PANEL_SONY_ACX565AKM` | `is not set` |
| `CONFIG_DRM_PANEL_SONY_TD4353_JDI` | `is not set` |
| `CONFIG_DRM_PANEL_SONY_TULIP_TRULY_NT35521` | `is not set` |
| `CONFIG_DRM_PANEL_STARTEK_KD070FHFID015` | `is not set` |
| `CONFIG_DRM_PANEL_SUMMIT` | `is not set` |
| `CONFIG_DRM_PANEL_SYNAPTICS_R63353` | `is not set` |
| `CONFIG_DRM_PANEL_SYNAPTICS_TDDI` | `is not set` |
| `CONFIG_DRM_PANEL_TDO_TL070WSH30` | `is not set` |
| `CONFIG_DRM_PANEL_TPO_TD043MTEA1` | `is not set` |
| `CONFIG_DRM_PANEL_TPO_TPG110` | `is not set` |
| `CONFIG_DRM_PANEL_TRULY_NT35597_WQXGA` | `is not set` |
| `CONFIG_DRM_PANEL_VISIONOX_G2647FB105` | `is not set` |
| `CONFIG_DRM_PANEL_VISIONOX_R66451` | `is not set` |
| `CONFIG_DRM_PANEL_VISIONOX_RM69299` | `is not set` |
| `CONFIG_DRM_PANEL_VISIONOX_RM692E5` | `is not set` |
| `CONFIG_DRM_PANEL_VISIONOX_VTDR6130` | `is not set` |
| `CONFIG_DRM_PANEL_WIDECHIPS_WS2401` | `is not set` |
| `CONFIG_DRM_PANEL_XINPENG_XPP055C272` | `is not set` |
| `CONFIG_DRM_PANIC` | `is not set` |
| `CONFIG_DRM_PIXPAPER` | `is not set` |
| `CONFIG_DRM_POWERVR` | `is not set` |
| `CONFIG_DRM_SAMSUNG_DSIM` | `is not set` |
| `CONFIG_DRM_SCHED` | `m` |
| `CONFIG_DRM_SIMPLEDRM` | `is not set` |
| `CONFIG_DRM_SOLOMON_SSD2825` | `is not set` |
| `CONFIG_DRM_SSD130X` | `is not set` |
| `CONFIG_DRM_ST7571_I2C` | `is not set` |
| `CONFIG_DRM_ST7586` | `is not set` |
| `CONFIG_DRM_ST7735R` | `is not set` |
| `CONFIG_DRM_SUBALLOC_HELPER` | `m` |
| `CONFIG_DRM_SUN4I` | `m` |
| `CONFIG_DRM_SUN6I_DSI` | `m` |
| `CONFIG_DRM_SUN8I_DW_HDMI` | `m` |
| `CONFIG_DRM_SUN8I_MIXER` | `m` |
| `CONFIG_DRM_SUN8I_TCON_TOP` | `m` |
| `CONFIG_DRM_TI_DLPC3433` | `is not set` |
| `CONFIG_DRM_TI_SN65DSI83` | `is not set` |
| `CONFIG_DRM_TI_TDP158` | `is not set` |
| `CONFIG_DRM_TTM_HELPER` | `m` |
| `CONFIG_DRM_VIRTIO_GPU_KMS` | `y` |
| `CONFIG_DRM_WAVESHARE_BRIDGE` | `is not set` |
| `CONFIG_DRM_XE` | `is not set` |
| `CONFIG_DS1803` | `is not set` |
| `CONFIG_DS4424` | `is not set` |
| `CONFIG_DT_IDLE_GENPD` | `y` |
| `CONFIG_DT_IDLE_STATES` | `y` |
| `CONFIG_DWC_PCIE_PMU` | `is not set` |
| `CONFIG_DWMAC_DWC_QOS_ETH` | `is not set` |
| `CONFIG_DWMAC_GENERIC` | `m` |
| `CONFIG_DWMAC_INTEL_PLAT` | `is not set` |
| `CONFIG_DWMAC_SOPHGO` | `m` |
| `CONFIG_DWMAC_STARFIVE` | `m` |
| `CONFIG_DWMAC_SUN55I` | `m` |
| `CONFIG_DWMAC_SUN8I` | `m` |
| `CONFIG_DWMAC_SUNXI` | `m` |
| `CONFIG_DWMAC_THEAD` | `m` |
| `CONFIG_DW_AXI_DMAC` | `y` |
| `CONFIG_DW_DMAC` | `is not set` |
| `CONFIG_DW_DMAC_PCI` | `is not set` |
| `CONFIG_DW_EDMA` | `is not set` |
| `CONFIG_DW_WATCHDOG` | `is not set` |
| `CONFIG_DW_XDATA_PCIE` | `is not set` |
| `CONFIG_DYNAMIC_EVENTS` | `y` |
| `CONFIG_DYNAMIC_SIGFRAME` | `y` |
| `CONFIG_EEPROM_M24LR` | `is not set` |
| `CONFIG_EFI_COCO_SECRET` | `is not set` |
| `CONFIG_EFI_CUSTOM_SSDT_OVERLAYS` | `is not set` |
| `CONFIG_EFI_DISABLE_RUNTIME` | `is not set` |
| `CONFIG_EFI_ZBOOT` | `is not set` |
| `CONFIG_ENABLE_DEFAULT_TRACERS` | `is not set` |
| `CONFIG_ENERGY_MODEL` | `is not set` |
| `CONFIG_ENS160` | `is not set` |
| `CONFIG_ENS210` | `is not set` |
| `CONFIG_ENVELOPE_DETECTOR` | `is not set` |
| `CONFIG_EPROBE_EVENTS` | `y` |
| `CONFIG_ERRATA_ANDES` | `y` |
| `CONFIG_ERRATA_MIPS` | `is not set` |
| `CONFIG_ERRATA_SIFIVE` | `y` |
| `CONFIG_ERRATA_SIFIVE_CIP_1200` | `y` |
| `CONFIG_ERRATA_SIFIVE_CIP_453` | `y` |
| `CONFIG_ERRATA_THEAD` | `y` |
| `CONFIG_ERRATA_THEAD_CMO` | `y` |
| `CONFIG_ERRATA_THEAD_GHOSTWRITE` | `y` |
| `CONFIG_ERRATA_THEAD_MAE` | `y` |
| `CONFIG_ERRATA_THEAD_PMU` | `y` |
| `CONFIG_EVENT_TRACING` | `y` |
| `CONFIG_EVM` | `is not set` |
| `CONFIG_EXECMEM` | `y` |
| `CONFIG_EXTCON_ADC_JACK` | `is not set` |
| `CONFIG_EXTCON_FSA9480` | `is not set` |
| `CONFIG_EXTCON_GPIO` | `is not set` |
| `CONFIG_EXTCON_LC824206XA` | `is not set` |
| `CONFIG_EXTCON_MAX14526` | `is not set` |
| `CONFIG_EXTCON_MAX3355` | `is not set` |
| `CONFIG_EXTCON_PTN5150` | `is not set` |
| `CONFIG_EXTCON_RT8973A` | `is not set` |
| `CONFIG_EXTCON_SM5502` | `is not set` |
| `CONFIG_EXTCON_USB_GPIO` | `is not set` |
| `CONFIG_FBNIC` | `is not set` |
| `CONFIG_FB_CORE` | `y` |
| `CONFIG_FB_DEVICE` | `y` |
| `CONFIG_FB_DMAMEM_HELPERS` | `y` |
| `CONFIG_FB_DMAMEM_HELPERS_DEFERRED` | `y` |
| `CONFIG_FB_IOMEM_FOPS` | `y` |
| `CONFIG_FB_IOMEM_HELPERS` | `y` |
| `CONFIG_FB_SSD1307` | `is not set` |
| `CONFIG_FB_SYSMEM_FOPS` | `y` |
| `CONFIG_FB_SYSMEM_HELPERS` | `y` |
| `CONFIG_FB_SYSMEM_HELPERS_DEFERRED` | `y` |
| `CONFIG_FIRMWARE_TABLE` | `y` |
| `CONFIG_FORTIFY_SOURCE` | `is not set` |
| `CONFIG_FRAMEBUFFER_CONSOLE_LEGACY_ACCELERATION` | `is not set` |
| `CONFIG_FREEZER` | `y` |
| `CONFIG_FSL_EDMA` | `is not set` |
| `CONFIG_FS_STACK` | `y` |
| `CONFIG_FTL` | `is not set` |
| `CONFIG_FTRACE_SYSCALLS` | `is not set` |
| `CONFIG_FUEL_GAUGE_MM8013` | `is not set` |
| `CONFIG_FUJITSU_ES` | `is not set` |
| `CONFIG_FUNCTION_ALIGNMENT` | `4` |
| `CONFIG_FUNCTION_ALIGNMENT_4B` | `y` |
| `CONFIG_FUNCTION_TRACER` | `is not set` |
| `CONFIG_FUN_ETH` | `is not set` |
| `CONFIG_FUTEX_PRIVATE_HASH` | `y` |
| `CONFIG_FWCTL` | `is not set` |
| `CONFIG_FWNODE_MDIO` | `y` |
| `CONFIG_FW_CACHE` | `y` |
| `CONFIG_FW_CFG_SYSFS` | `is not set` |
| `CONFIG_FW_DEVLINK_SYNC_STATE_TIMEOUT` | `is not set` |
| `CONFIG_FW_UPLOAD` | `is not set` |
| `CONFIG_FXAS21002C` | `is not set` |
| `CONFIG_FXLS8962AF_I2C` | `is not set` |
| `CONFIG_FXLS8962AF_SPI` | `is not set` |
| `CONFIG_FXOS8700_I2C` | `is not set` |
| `CONFIG_FXOS8700_SPI` | `is not set` |
| `CONFIG_GCC10_NO_ARRAY_BOUNDS` | `y` |
| `CONFIG_GCC_NO_STRINGOP_OVERFLOW` | `y` |
| `CONFIG_GCC_PLUGINS` | `y` |
| `CONFIG_GCC_PLUGIN_LATENT_ENTROPY` | `is not set` |
| `CONFIG_GCC_SUPPORTS_DYNAMIC_FTRACE` | `y` |
| `CONFIG_GEHC_PMC_ADC` | `is not set` |
| `CONFIG_GENERIC_ADC_BATTERY` | `is not set` |
| `CONFIG_GENERIC_ADC_THERMAL` | `is not set` |
| `CONFIG_GENERIC_CLOCKEVENTS_BROADCAST` | `y` |
| `CONFIG_GENERIC_CPU_DEVICES` | `y` |
| `CONFIG_GENERIC_CPU_VULNERABILITIES` | `y` |
| `CONFIG_GENERIC_ENTRY` | `y` |
| `CONFIG_GENERIC_IDLE_POLL_SETUP` | `y` |
| `CONFIG_GENERIC_IRQ_EFFECTIVE_AFF_MASK` | `y` |
| `CONFIG_GENERIC_IRQ_ENTRY` | `y` |
| `CONFIG_GENERIC_IRQ_IPI` | `y` |
| `CONFIG_GENERIC_IRQ_IPI_MUX` | `y` |
| `CONFIG_GENERIC_IRQ_MATRIX_ALLOCATOR` | `y` |
| `CONFIG_GENERIC_IRQ_MIGRATION` | `y` |
| `CONFIG_GENERIC_IRQ_SHOW_LEVEL` | `y` |
| `CONFIG_GENERIC_LIB_DEVMEM_IS_ALLOWED` | `y` |
| `CONFIG_GENERIC_PENDING_IRQ` | `y` |
| `CONFIG_GENERIC_PHY_MIPI_DPHY` | `y` |
| `CONFIG_GENERIC_PINCONF` | `y` |
| `CONFIG_GENERIC_PINCTRL_GROUPS` | `y` |
| `CONFIG_GENERIC_PINMUX_FUNCTIONS` | `y` |
| `CONFIG_GENERIC_SYSCALL` | `y` |
| `CONFIG_GP2AP002` | `is not set` |
| `CONFIG_GP2AP020A00F` | `is not set` |
| `CONFIG_GPIB` | `is not set` |
| `CONFIG_GPIOLIB_FASTPATH_LIMIT` | `512` |
| `CONFIG_GPIOLIB_IRQCHIP` | `y` |
| `CONFIG_GPIOLIB_LEGACY` | `y` |
| `CONFIG_GPIO_74X164` | `is not set` |
| `CONFIG_GPIO_74XX_MMIO` | `is not set` |
| `CONFIG_GPIO_ACPI` | `y` |
| `CONFIG_GPIO_ADNP` | `is not set` |
| `CONFIG_GPIO_AGGREGATOR` | `is not set` |
| `CONFIG_GPIO_ALTERA` | `is not set` |
| `CONFIG_GPIO_AMDPT` | `is not set` |
| `CONFIG_GPIO_AMD_FCH` | `is not set` |
| `CONFIG_GPIO_BT8XX` | `is not set` |
| `CONFIG_GPIO_CADENCE` | `is not set` |
| `CONFIG_GPIO_CDEV` | `y` |
| `CONFIG_GPIO_CDEV_V1` | `y` |
| `CONFIG_GPIO_DS4520` | `is not set` |
| `CONFIG_GPIO_DWAPB` | `y` |
| `CONFIG_GPIO_EXAR` | `is not set` |
| `CONFIG_GPIO_FTGPIO010` | `is not set` |
| `CONFIG_GPIO_FXL6408` | `is not set` |
| `CONFIG_GPIO_GENERIC` | `y` |
| `CONFIG_GPIO_GENERIC_PLATFORM` | `is not set` |
| `CONFIG_GPIO_GRGPIO` | `is not set` |
| `CONFIG_GPIO_GW_PLD` | `is not set` |
| `CONFIG_GPIO_HLWD` | `is not set` |
| `CONFIG_GPIO_LATCH` | `is not set` |
| `CONFIG_GPIO_LOGICVC` | `is not set` |
| `CONFIG_GPIO_MAX3191X` | `is not set` |
| `CONFIG_GPIO_MAX7300` | `is not set` |
| `CONFIG_GPIO_MAX7301` | `is not set` |
| `CONFIG_GPIO_MAX732X` | `is not set` |
| `CONFIG_GPIO_MB86S7X` | `is not set` |
| `CONFIG_GPIO_MC33880` | `is not set` |
| `CONFIG_GPIO_MOCKUP` | `is not set` |
| `CONFIG_GPIO_MPSSE` | `is not set` |
| `CONFIG_GPIO_PCA953X` | `is not set` |
| `CONFIG_GPIO_PCA9570` | `is not set` |
| `CONFIG_GPIO_PCF857X` | `is not set` |
| `CONFIG_GPIO_PCIE_IDIO_24` | `is not set` |
| `CONFIG_GPIO_PCI_IDIO_16` | `is not set` |
| `CONFIG_GPIO_PISOSR` | `is not set` |
| `CONFIG_GPIO_PL061` | `is not set` |
| `CONFIG_GPIO_POLARFIRE_SOC` | `is not set` |
| `CONFIG_GPIO_QIXIS_FPGA` | `is not set` |
| `CONFIG_GPIO_RDC321X` | `is not set` |
| `CONFIG_GPIO_SIFIVE` | `y` |
| `CONFIG_GPIO_SIM` | `is not set` |
| `CONFIG_GPIO_SPACEMIT_K1` | `y` |
| `CONFIG_GPIO_SYSCON` | `is not set` |
| `CONFIG_GPIO_TPIC2810` | `is not set` |
| `CONFIG_GPIO_VIRTIO` | `is not set` |
| `CONFIG_GPIO_VIRTUSER` | `is not set` |
| `CONFIG_GPIO_WATCHDOG` | `is not set` |
| `CONFIG_GPIO_XILINX` | `is not set` |
| `CONFIG_GPIO_XRA1403` | `is not set` |
| `CONFIG_GP_PCI1XXXX` | `is not set` |
| `CONFIG_GROUP_SCHED_BANDWIDTH` | `y` |
| `CONFIG_GROUP_SCHED_WEIGHT` | `y` |
| `CONFIG_GUEST_PERF_EVENTS` | `y` |
| `CONFIG_GUP_TEST` | `is not set` |
| `CONFIG_HARDIRQS_SW_RESEND` | `y` |
| `CONFIG_HARDLOCKUP_DETECTOR` | `is not set` |
| `CONFIG_HAS_IOPORT` | `y` |
| `CONFIG_HAS_SECURITY_AUDIT` | `y` |
| `CONFIG_HAVE_64BIT_ALIGNED_ACCESS` | `y` |
| `CONFIG_HAVE_ALIGNED_STRUCT_PAGE` | `y` |
| `CONFIG_HAVE_ARCH_HUGE_VMALLOC` | `y` |
| `CONFIG_HAVE_ARCH_HUGE_VMAP` | `y` |
| `CONFIG_HAVE_ARCH_KASAN_VMALLOC` | `y` |
| `CONFIG_HAVE_ARCH_KFENCE` | `y` |
| `CONFIG_HAVE_ARCH_KSTACK_ERASE` | `y` |
| `CONFIG_HAVE_ARCH_MMAP_RND_COMPAT_BITS` | `y` |
| `CONFIG_HAVE_ARCH_RANDOMIZE_KSTACK_OFFSET` | `y` |
| `CONFIG_HAVE_ARCH_SOFT_DIRTY` | `y` |
| `CONFIG_HAVE_ARCH_THREAD_STRUCT_WHITELIST` | `y` |
| `CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE` | `y` |
| `CONFIG_HAVE_ARCH_TRANSPARENT_HUGEPAGE_PUD` | `y` |
| `CONFIG_HAVE_ARCH_VMAP_STACK` | `y` |
| `CONFIG_HAVE_CONTEXT_TRACKING_USER` | `y` |
| `CONFIG_HAVE_DYNAMIC_FTRACE_WITH_ARGS` | `y` |
| `CONFIG_HAVE_FTRACE_GRAPH_FUNC` | `y` |
| `CONFIG_HAVE_FUNCTION_ARG_ACCESS_API` | `y` |
| `CONFIG_HAVE_FUNCTION_ERROR_INJECTION` | `y` |
| `CONFIG_HAVE_FUNCTION_GRAPH_FREGS` | `y` |
| `CONFIG_HAVE_GENERIC_TIF_BITS` | `y` |
| `CONFIG_HAVE_GIGANTIC_FOLIOS` | `y` |
| `CONFIG_HAVE_GUP_FAST` | `y` |
| `CONFIG_HAVE_HARDLOCKUP_DETECTOR_BUDDY` | `y` |
| `CONFIG_HAVE_IRQ_EXIT_ON_IRQ_STACK` | `y` |
| `CONFIG_HAVE_IRQ_TIME_ACCOUNTING` | `y` |
| `CONFIG_HAVE_KERNEL_BZIP2` | `y` |
| `CONFIG_HAVE_KERNEL_GZIP` | `y` |
| `CONFIG_HAVE_KERNEL_LZ4` | `y` |
| `CONFIG_HAVE_KERNEL_LZMA` | `y` |
| `CONFIG_HAVE_KERNEL_LZO` | `y` |
| `CONFIG_HAVE_KERNEL_UNCOMPRESSED` | `y` |
| `CONFIG_HAVE_KERNEL_XZ` | `y` |
| `CONFIG_HAVE_KERNEL_ZSTD` | `y` |
| `CONFIG_HAVE_KPROBES` | `y` |
| `CONFIG_HAVE_KRETPROBES` | `y` |
| `CONFIG_HAVE_KVM_DIRTY_RING` | `y` |
| `CONFIG_HAVE_KVM_DIRTY_RING_ACQ_REL` | `y` |
| `CONFIG_HAVE_KVM_IRQCHIP` | `y` |
| `CONFIG_HAVE_KVM_IRQ_ROUTING` | `y` |
| `CONFIG_HAVE_KVM_MSI` | `y` |
| `CONFIG_HAVE_KVM_READONLY_MEM` | `y` |
| `CONFIG_HAVE_LD_DEAD_CODE_DATA_ELIMINATION` | `y` |
| `CONFIG_HAVE_MOVE_PMD` | `y` |
| `CONFIG_HAVE_MOVE_PUD` | `y` |
| `CONFIG_HAVE_PAGE_SIZE_4KB` | `y` |
| `CONFIG_HAVE_POSIX_CPU_TIMERS_TASK_WORK` | `y` |
| `CONFIG_HAVE_PREEMPT_DYNAMIC` | `y` |
| `CONFIG_HAVE_PREEMPT_DYNAMIC_KEY` | `y` |
| `CONFIG_HAVE_REGS_AND_STACK_ACCESS_API` | `y` |
| `CONFIG_HAVE_RETHOOK` | `y` |
| `CONFIG_HAVE_RSEQ` | `y` |
| `CONFIG_HAVE_SAMPLE_FTRACE_DIRECT` | `y` |
| `CONFIG_HAVE_SAMPLE_FTRACE_DIRECT_MULTI` | `y` |
| `CONFIG_HAVE_SOFTIRQ_ON_OWN_STACK` | `y` |
| `CONFIG_HDC100X` | `is not set` |
| `CONFIG_HDC2010` | `is not set` |
| `CONFIG_HDC3020` | `is not set` |
| `CONFIG_HI8435` | `is not set` |
| `CONFIG_HIBERNATION` | `is not set` |
| `CONFIG_HIBMCGE` | `is not set` |
| `CONFIG_HID_EVISION` | `is not set` |
| `CONFIG_HID_GOODIX_SPI` | `is not set` |
| `CONFIG_HID_GOOGLE_STADIA_FF` | `is not set` |
| `CONFIG_HID_HAPTIC` | `is not set` |
| `CONFIG_HID_KYSONA` | `is not set` |
| `CONFIG_HID_LETSKETCH` | `is not set` |
| `CONFIG_HID_MCP2200` | `is not set` |
| `CONFIG_HID_MCP2221` | `is not set` |
| `CONFIG_HID_MEGAWORLD_FF` | `is not set` |
| `CONFIG_HID_PRODIKEYS` | `is not set` |
| `CONFIG_HID_PXRC` | `is not set` |
| `CONFIG_HID_RAZER` | `is not set` |
| `CONFIG_HID_SEMITEK` | `is not set` |
| `CONFIG_HID_SIGMAMICRO` | `is not set` |
| `CONFIG_HID_SUPPORT` | `y` |
| `CONFIG_HID_TOPRE` | `is not set` |
| `CONFIG_HID_VRC2` | `is not set` |
| `CONFIG_HID_XIAOMI` | `is not set` |
| `CONFIG_HISI_HIKEY_USB` | `is not set` |
| `CONFIG_HIST_TRIGGERS` | `is not set` |
| `CONFIG_HMC425` | `is not set` |
| `CONFIG_HOTPLUG_CORE_SYNC` | `y` |
| `CONFIG_HOTPLUG_CORE_SYNC_DEAD` | `y` |
| `CONFIG_HOTPLUG_CORE_SYNC_FULL` | `y` |
| `CONFIG_HOTPLUG_PARALLEL` | `y` |
| `CONFIG_HOTPLUG_SPLIT_STARTUP` | `y` |
| `CONFIG_HP03` | `is not set` |
| `CONFIG_HP206C` | `is not set` |
| `CONFIG_HSC030PA` | `is not set` |
| `CONFIG_HTE` | `is not set` |
| `CONFIG_HTS221` | `is not set` |
| `CONFIG_HTU21` | `is not set` |
| `CONFIG_HUGETLB_PAGE` | `y` |
| `CONFIG_HUGETLB_PAGE_OPTIMIZE_VMEMMAP` | `y` |
| `CONFIG_HUGETLB_PAGE_OPTIMIZE_VMEMMAP_DEFAULT_ON` | `is not set` |
| `CONFIG_HUGETLB_PMD_PAGE_TABLE_SHARING` | `y` |
| `CONFIG_HWLAT_TRACER` | `is not set` |
| `CONFIG_HW_RANDOM_ATMEL` | `y` |
| `CONFIG_HW_RANDOM_JH7110` | `m` |
| `CONFIG_HX711` | `is not set` |
| `CONFIG_HX9023S` | `is not set` |
| `CONFIG_I2C_AMD_MP2` | `is not set` |
| `CONFIG_I2C_ARB_GPIO_CHALLENGE` | `is not set` |
| `CONFIG_I2C_AT91` | `is not set` |
| `CONFIG_I2C_CADENCE` | `is not set` |
| `CONFIG_I2C_CBUS_GPIO` | `is not set` |
| `CONFIG_I2C_CP2615` | `is not set` |
| `CONFIG_I2C_DEMUX_PINCTRL` | `is not set` |
| `CONFIG_I2C_DESIGNWARE_CORE` | `y` |
| `CONFIG_I2C_DESIGNWARE_SLAVE` | `is not set` |
| `CONFIG_I2C_GPIO` | `is not set` |
| `CONFIG_I2C_HID_ACPI` | `is not set` |
| `CONFIG_I2C_HID_OF` | `is not set` |
| `CONFIG_I2C_HID_OF_ELAN` | `is not set` |
| `CONFIG_I2C_HID_OF_GOODIX` | `is not set` |
| `CONFIG_I2C_K1` | `is not set` |
| `CONFIG_I2C_MICROCHIP_CORE` | `is not set` |
| `CONFIG_I2C_MUX_GPIO` | `is not set` |
| `CONFIG_I2C_MUX_GPMUX` | `is not set` |
| `CONFIG_I2C_MUX_LTC4306` | `is not set` |
| `CONFIG_I2C_MUX_MLXCPLD` | `is not set` |
| `CONFIG_I2C_MUX_PCA9541` | `is not set` |
| `CONFIG_I2C_MUX_PCA954x` | `is not set` |
| `CONFIG_I2C_MUX_PINCTRL` | `is not set` |
| `CONFIG_I2C_MUX_REG` | `is not set` |
| `CONFIG_I2C_MV64XXX` | `m` |
| `CONFIG_I2C_NOMADIK` | `is not set` |
| `CONFIG_I2C_PCI1XXXX` | `is not set` |
| `CONFIG_I2C_SCMI` | `is not set` |
| `CONFIG_I2C_VIRTIO` | `is not set` |
| `CONFIG_I2C_ZHAOXIN` | `is not set` |
| `CONFIG_I6300ESB_WDT` | `is not set` |
| `CONFIG_IAQCORE` | `is not set` |
| `CONFIG_ICP10100` | `is not set` |
| `CONFIG_IDPF` | `is not set` |
| `CONFIG_IIO_BUFFER` | `is not set` |
| `CONFIG_IIO_CONFIGFS` | `is not set` |
| `CONFIG_IIO_KX022A_I2C` | `is not set` |
| `CONFIG_IIO_KX022A_SPI` | `is not set` |
| `CONFIG_IIO_MUX` | `is not set` |
| `CONFIG_IIO_RESCALE` | `is not set` |
| `CONFIG_IIO_SSP_SENSORHUB` | `is not set` |
| `CONFIG_IIO_ST_ACCEL_3AXIS` | `is not set` |
| `CONFIG_IIO_ST_GYRO_3AXIS` | `is not set` |
| `CONFIG_IIO_ST_LSM6DSX` | `is not set` |
| `CONFIG_IIO_ST_LSM9DS0` | `is not set` |
| `CONFIG_IIO_ST_MAGN_3AXIS` | `is not set` |
| `CONFIG_IIO_ST_PRESS` | `is not set` |
| `CONFIG_IIO_SW_DEVICE` | `is not set` |
| `CONFIG_IIO_SW_TRIGGER` | `is not set` |
| `CONFIG_IIO_TRIGGER` | `is not set` |
| `CONFIG_IIO_TRIGGERED_EVENT` | `is not set` |
| `CONFIG_ILLEGAL_POINTER_VALUE` | `0xdead000000000000` |
| `CONFIG_IMA` | `is not set` |
| `CONFIG_INA2XX_ADC` | `is not set` |
| `CONFIG_INET_ESPINTCP` | `is not set` |
| `CONFIG_INET_ESP_OFFLOAD` | `is not set` |
| `CONFIG_INET_PSP` | `is not set` |
| `CONFIG_INET_TABLE_PERTURB_ORDER` | `16` |
| `CONFIG_INFINEON_TLV493D` | `is not set` |
| `CONFIG_INFTL` | `is not set` |
| `CONFIG_INITRAMFS_PRESERVE_MTIME` | `y` |
| `CONFIG_INLINE_READ_UNLOCK` | `y` |
| `CONFIG_INLINE_READ_UNLOCK_IRQ` | `y` |
| `CONFIG_INLINE_SPIN_UNLOCK_IRQ` | `y` |
| `CONFIG_INLINE_WRITE_UNLOCK` | `y` |
| `CONFIG_INLINE_WRITE_UNLOCK_IRQ` | `y` |
| `CONFIG_INPUT_VIVALDIFMAP` | `y` |
| `CONFIG_INTEGRITY` | `y` |
| `CONFIG_INTEGRITY_AUDIT` | `y` |
| `CONFIG_INTEGRITY_SIGNATURE` | `is not set` |
| `CONFIG_INTEL_IDMA64` | `is not set` |
| `CONFIG_INV_ICM42600_I2C` | `is not set` |
| `CONFIG_INV_ICM42600_SPI` | `is not set` |
| `CONFIG_INV_ICM45600_I2C` | `is not set` |
| `CONFIG_INV_ICM45600_SPI` | `is not set` |
| `CONFIG_INV_MPU6050_I2C` | `is not set` |
| `CONFIG_INV_MPU6050_SPI` | `is not set` |
| `CONFIG_IOMMUFD` | `is not set` |
| `CONFIG_IOMMU_API` | `y` |
| `CONFIG_IOMMU_DEFAULT_DMA_LAZY` | `is not set` |
| `CONFIG_IOMMU_DEFAULT_DMA_STRICT` | `y` |
| `CONFIG_IOMMU_DEFAULT_PASSTHROUGH` | `is not set` |
| `CONFIG_IO_URING_ZCRX` | `y` |
| `CONFIG_IP5XXX_POWER` | `is not set` |
| `CONFIG_IP6_NF_IPTABLES` | `m` |
| `CONFIG_IP6_NF_MATCH_AH` | `is not set` |
| `CONFIG_IP6_NF_MATCH_EUI64` | `is not set` |
| `CONFIG_IP6_NF_MATCH_FRAG` | `is not set` |
| `CONFIG_IP6_NF_MATCH_HL` | `is not set` |
| `CONFIG_IP6_NF_MATCH_IPV6HEADER` | `m` |
| `CONFIG_IP6_NF_MATCH_MH` | `is not set` |
| `CONFIG_IP6_NF_MATCH_OPTS` | `is not set` |
| `CONFIG_IP6_NF_MATCH_RT` | `is not set` |
| `CONFIG_IP6_NF_MATCH_SRH` | `is not set` |
| `CONFIG_IP6_NF_TARGET_SYNPROXY` | `is not set` |
| `CONFIG_IPV6_ILA` | `is not set` |
| `CONFIG_IPV6_IOAM6_LWTUNNEL` | `is not set` |
| `CONFIG_IPVLAN_L3S` | `y` |
| `CONFIG_IPVTAP` | `is not set` |
| `CONFIG_IP_NF_IPTABLES` | `m` |
| `CONFIG_IP_NF_MATCH_AH` | `is not set` |
| `CONFIG_IP_NF_MATCH_ECN` | `is not set` |
| `CONFIG_IP_NF_MATCH_TTL` | `is not set` |
| `CONFIG_IP_NF_TARGET_SYNPROXY` | `is not set` |
| `CONFIG_IP_SET` | `is not set` |
| `CONFIG_IP_VS` | `m` |
| `CONFIG_IP_VS_DEBUG` | `is not set` |
| `CONFIG_IP_VS_DH` | `is not set` |
| `CONFIG_IP_VS_FO` | `is not set` |
| `CONFIG_IP_VS_IPV6` | `is not set` |
| `CONFIG_IP_VS_LBLC` | `is not set` |
| `CONFIG_IP_VS_LBLCR` | `is not set` |
| `CONFIG_IP_VS_LC` | `is not set` |
| `CONFIG_IP_VS_MH` | `is not set` |
| `CONFIG_IP_VS_MH_TAB_INDEX` | `12` |
| `CONFIG_IP_VS_NFCT` | `y` |
| `CONFIG_IP_VS_NQ` | `is not set` |
| `CONFIG_IP_VS_OVF` | `is not set` |
| `CONFIG_IP_VS_PROTO_AH` | `is not set` |
| `CONFIG_IP_VS_PROTO_ESP` | `is not set` |
| `CONFIG_IP_VS_PROTO_SCTP` | `is not set` |
| `CONFIG_IP_VS_PROTO_TCP` | `y` |
| `CONFIG_IP_VS_PROTO_UDP` | `y` |
| `CONFIG_IP_VS_RR` | `m` |
| `CONFIG_IP_VS_SED` | `is not set` |
| `CONFIG_IP_VS_SH` | `is not set` |
| `CONFIG_IP_VS_SH_TAB_BITS` | `8` |
| `CONFIG_IP_VS_TAB_BITS` | `12` |
| `CONFIG_IP_VS_TWOS` | `is not set` |
| `CONFIG_IP_VS_WLC` | `is not set` |
| `CONFIG_IP_VS_WRR` | `is not set` |
| `CONFIG_IRQSOFF_TRACER` | `is not set` |
| `CONFIG_IRQ_FORCED_THREADING` | `y` |
| `CONFIG_IRQ_MSI_LIB` | `y` |
| `CONFIG_IRQ_STACKS` | `y` |
| `CONFIG_IRQ_TIME_ACCOUNTING` | `is not set` |
| `CONFIG_IRSD200` | `is not set` |
| `CONFIG_ISCSI_IBFT` | `is not set` |
| `CONFIG_ISL29125` | `is not set` |
| `CONFIG_ISL29501` | `is not set` |
| `CONFIG_ISL76682` | `is not set` |
| `CONFIG_ITG3200` | `is not set` |
| `CONFIG_JFFS2_FS` | `is not set` |
| `CONFIG_JH71XX_PMU` | `y` |
| `CONFIG_JOLIET` | `y` |
| `CONFIG_JSA1212` | `is not set` |
| `CONFIG_KALLSYMS_SELFTEST` | `is not set` |
| `CONFIG_KCMP` | `y` |
| `CONFIG_KERNEL_BZIP2` | `is not set` |
| `CONFIG_KERNEL_GZIP` | `y` |
| `CONFIG_KERNEL_LZ4` | `is not set` |
| `CONFIG_KERNEL_LZMA` | `is not set` |
| `CONFIG_KERNEL_LZO` | `is not set` |
| `CONFIG_KERNEL_UNCOMPRESSED` | `is not set` |
| `CONFIG_KERNEL_XZ` | `is not set` |
| `CONFIG_KERNEL_ZSTD` | `is not set` |
| `CONFIG_KEXEC` | `is not set` |
| `CONFIG_KEXEC_FILE` | `is not set` |
| `CONFIG_KEYBOARD_ADC` | `is not set` |
| `CONFIG_KEYBOARD_CYPRESS_SF` | `is not set` |
| `CONFIG_KEYBOARD_GPIO` | `y` |
| `CONFIG_KEYBOARD_GPIO_POLLED` | `is not set` |
| `CONFIG_KEYBOARD_MATRIX` | `is not set` |
| `CONFIG_KEYBOARD_PINEPHONE` | `is not set` |
| `CONFIG_KEYBOARD_SUN4I_LRADC` | `m` |
| `CONFIG_KFENCE` | `is not set` |
| `CONFIG_KMX61` | `is not set` |
| `CONFIG_KPROBES` | `is not set` |
| `CONFIG_KS8842` | `is not set` |
| `CONFIG_KSTACK_ERASE` | `is not set` |
| `CONFIG_KVFREE_RCU_BATCHED` | `y` |
| `CONFIG_KVM` | `m` |
| `CONFIG_KVM_COMMON` | `y` |
| `CONFIG_KVM_GENERIC_DIRTYLOG_READ_PROTECT` | `y` |
| `CONFIG_KVM_GENERIC_HARDWARE_ENABLING` | `y` |
| `CONFIG_KVM_GENERIC_MMU_NOTIFIER` | `y` |
| `CONFIG_KVM_MMIO` | `y` |
| `CONFIG_KXCJK1013` | `is not set` |
| `CONFIG_KXSD9` | `is not set` |
| `CONFIG_LAN865X` | `is not set` |
| `CONFIG_LD_CAN_USE_KEEP_IN_OVERLAY` | `y` |
| `CONFIG_LD_IS_BFD` | `y` |
| `CONFIG_LD_ORPHAN_WARN` | `y` |
| `CONFIG_LD_ORPHAN_WARN_LEVEL` | `"warn"` |
| `CONFIG_LEGACY_DIRECT_IO` | `y` |
| `CONFIG_LEGACY_TIOCSTI` | `y` |
| `CONFIG_LIDAR_LITE_V2` | `is not set` |
| `CONFIG_LINEAR_RANGES` | `y` |
| `CONFIG_LIST_HARDENED` | `is not set` |
| `CONFIG_LITEX_LITEETH` | `is not set` |
| `CONFIG_LITEX_SOC_CONTROLLER` | `is not set` |
| `CONFIG_LLC` | `m` |
| `CONFIG_LMK04832` | `is not set` |
| `CONFIG_LMP91000` | `is not set` |
| `CONFIG_LOCK_MM_AND_FIND_VMA` | `y` |
| `CONFIG_LRU_GEN` | `is not set` |
| `CONFIG_LSM_MMAP_MIN_ADDR` | `65536` |
| `CONFIG_LTC1660` | `is not set` |
| `CONFIG_LTC2309` | `is not set` |
| `CONFIG_LTC2471` | `is not set` |
| `CONFIG_LTC2485` | `is not set` |
| `CONFIG_LTC2496` | `is not set` |
| `CONFIG_LTC2497` | `is not set` |
| `CONFIG_LTC2632` | `is not set` |
| `CONFIG_LTC2664` | `is not set` |
| `CONFIG_LTC2688` | `is not set` |
| `CONFIG_LTC2983` | `is not set` |
| `CONFIG_LTO_NONE` | `y` |
| `CONFIG_LTR390` | `is not set` |
| `CONFIG_LTR501` | `is not set` |
| `CONFIG_LTRF216A` | `is not set` |
| `CONFIG_LV0104CS` | `is not set` |
| `CONFIG_LWQ_TEST` | `is not set` |
| `CONFIG_LZO_COMPRESS` | `m` |
| `CONFIG_M62332` | `is not set` |
| `CONFIG_MACVTAP` | `is not set` |
| `CONFIG_MAG3110` | `is not set` |
| `CONFIG_MAILBOX_TEST` | `is not set` |
| `CONFIG_MANAGER_SBS` | `is not set` |
| `CONFIG_MARVELL_88Q2XXX_PHY` | `is not set` |
| `CONFIG_MARVELL_88X2222_PHY` | `is not set` |
| `CONFIG_MAX1027` | `is not set` |
| `CONFIG_MAX11100` | `is not set` |
| `CONFIG_MAX1118` | `is not set` |
| `CONFIG_MAX11205` | `is not set` |
| `CONFIG_MAX11410` | `is not set` |
| `CONFIG_MAX1241` | `is not set` |
| `CONFIG_MAX1363` | `is not set` |
| `CONFIG_MAX14001` | `is not set` |
| `CONFIG_MAX30100` | `is not set` |
| `CONFIG_MAX30102` | `is not set` |
| `CONFIG_MAX30208` | `is not set` |
| `CONFIG_MAX31827` | `is not set` |
| `CONFIG_MAX31856` | `is not set` |
| `CONFIG_MAX31865` | `is not set` |
| `CONFIG_MAX34408` | `is not set` |
| `CONFIG_MAX44000` | `is not set` |
| `CONFIG_MAX44009` | `is not set` |
| `CONFIG_MAX517` | `is not set` |
| `CONFIG_MAX5432` | `is not set` |
| `CONFIG_MAX5481` | `is not set` |
| `CONFIG_MAX5487` | `is not set` |
| `CONFIG_MAX5522` | `is not set` |
| `CONFIG_MAX5821` | `is not set` |
| `CONFIG_MAX63XX_WATCHDOG` | `is not set` |
| `CONFIG_MAX9611` | `is not set` |
| `CONFIG_MAXIM_THERMOCOUPLE` | `is not set` |
| `CONFIG_MAXLINEAR_86110_PHY` | `is not set` |
| `CONFIG_MAXLINEAR_GPHY` | `is not set` |
| `CONFIG_MAX_SKB_FRAGS` | `17` |
| `CONFIG_MB1232` | `is not set` |
| `CONFIG_MC3230` | `is not set` |
| `CONFIG_MCHP_CLK_MPFS` | `y` |
| `CONFIG_MCHP_SBI_IPC_MBOX` | `is not set` |
| `CONFIG_MCP320X` | `is not set` |
| `CONFIG_MCP3422` | `is not set` |
| `CONFIG_MCP3564` | `is not set` |
| `CONFIG_MCP3911` | `is not set` |
| `CONFIG_MCP4018` | `is not set` |
| `CONFIG_MCP41010` | `is not set` |
| `CONFIG_MCP4131` | `is not set` |
| `CONFIG_MCP4531` | `is not set` |
| `CONFIG_MCP4725` | `is not set` |
| `CONFIG_MCP4728` | `is not set` |
| `CONFIG_MCP4821` | `is not set` |
| `CONFIG_MCP4922` | `is not set` |
| `CONFIG_MCP9600` | `is not set` |
| `CONFIG_MCTP` | `is not set` |
| `CONFIG_MDIO_BUS_MUX` | `m` |
| `CONFIG_MDIO_BUS_MUX_GPIO` | `is not set` |
| `CONFIG_MDIO_SUN4I` | `is not set` |
| `CONFIG_MEDIATEK_GE_PHY` | `is not set` |
| `CONFIG_MEDIA_ANALOG_TV_SUPPORT` | `is not set` |
| `CONFIG_MEDIA_CAMERA_SUPPORT` | `is not set` |
| `CONFIG_MEDIA_CONTROLLER` | `y` |
| `CONFIG_MEDIA_DIGITAL_TV_SUPPORT` | `is not set` |
| `CONFIG_MEDIA_HIDE_ANCILLARY_SUBDRV` | `y` |
| `CONFIG_MEDIA_PCI_SUPPORT` | `is not set` |
| `CONFIG_MEDIA_PLATFORM_DRIVERS` | `y` |
| `CONFIG_MEDIA_PLATFORM_SUPPORT` | `y` |
| `CONFIG_MEDIA_RADIO_SUPPORT` | `is not set` |
| `CONFIG_MEDIA_SDR_SUPPORT` | `is not set` |
| `CONFIG_MEDIA_SUBDRV_AUTOSELECT` | `y` |
| `CONFIG_MEDIA_SUPPORT_FILTER` | `y` |
| `CONFIG_MEDIA_TEST_SUPPORT` | `is not set` |
| `CONFIG_MEDIA_USB_SUPPORT` | `is not set` |
| `CONFIG_MEMCG_V1` | `is not set` |
| `CONFIG_MEMORY_HOTPLUG` | `is not set` |
| `CONFIG_MEMREGION` | `y` |
| `CONFIG_MEM_ALLOC_PROFILING` | `is not set` |
| `CONFIG_MEM_SOFT_DIRTY` | `is not set` |
| `CONFIG_MEN_A21_WDT` | `is not set` |
| `CONFIG_MFD_88PM886_PMIC` | `is not set` |
| `CONFIG_MFD_AAT2870_CORE` | `is not set` |
| `CONFIG_MFD_ADP5585` | `is not set` |
| `CONFIG_MFD_AT91_USART` | `is not set` |
| `CONFIG_MFD_ATC260X_I2C` | `is not set` |
| `CONFIG_MFD_AXP20X` | `y` |
| `CONFIG_MFD_BQ257XX` | `is not set` |
| `CONFIG_MFD_CORE` | `y` |
| `CONFIG_MFD_CS40L50_I2C` | `is not set` |
| `CONFIG_MFD_CS40L50_SPI` | `is not set` |
| `CONFIG_MFD_CS42L43_I2C` | `is not set` |
| `CONFIG_MFD_INTEL_M10_BMC_SPI` | `is not set` |
| `CONFIG_MFD_LS2K_BMC_CORE` | `is not set` |
| `CONFIG_MFD_MAX5970` | `is not set` |
| `CONFIG_MFD_MAX7360` | `is not set` |
| `CONFIG_MFD_MAX77541` | `is not set` |
| `CONFIG_MFD_MAX77705` | `is not set` |
| `CONFIG_MFD_MAX77714` | `is not set` |
| `CONFIG_MFD_MAX77759` | `is not set` |
| `CONFIG_MFD_MT6370` | `is not set` |
| `CONFIG_MFD_NCT6694` | `is not set` |
| `CONFIG_MFD_NTXEC` | `is not set` |
| `CONFIG_MFD_OCELOT` | `is not set` |
| `CONFIG_MFD_PF1550` | `is not set` |
| `CONFIG_MFD_QCOM_PM8008` | `is not set` |
| `CONFIG_MFD_RK8XX_I2C` | `is not set` |
| `CONFIG_MFD_RK8XX_SPI` | `is not set` |
| `CONFIG_MFD_ROHM_BD957XMUF` | `is not set` |
| `CONFIG_MFD_ROHM_BD96801` | `is not set` |
| `CONFIG_MFD_RSMU_I2C` | `is not set` |
| `CONFIG_MFD_RSMU_SPI` | `is not set` |
| `CONFIG_MFD_RT4831` | `is not set` |
| `CONFIG_MFD_RT5120` | `is not set` |
| `CONFIG_MFD_SEC_I2C` | `is not set` |
| `CONFIG_MFD_SIMPLE_MFD_I2C` | `y` |
| `CONFIG_MFD_SMPRO` | `is not set` |
| `CONFIG_MFD_SPACEMIT_P1` | `y` |
| `CONFIG_MFD_SUN4I_GPADC` | `is not set` |
| `CONFIG_MFD_SUN6I_PRCM` | `is not set` |
| `CONFIG_MFD_SY7636A` | `is not set` |
| `CONFIG_MFD_TPS65219` | `is not set` |
| `CONFIG_MFD_TPS65910` | `is not set` |
| `CONFIG_MFD_TPS6594_I2C` | `is not set` |
| `CONFIG_MFD_TPS6594_SPI` | `is not set` |
| `CONFIG_MGBE` | `is not set` |
| `CONFIG_MHI_BUS_EP` | `is not set` |
| `CONFIG_MICROCHIP_T1S_PHY` | `is not set` |
| `CONFIG_MII` | `m` |
| `CONFIG_MISC_RP1` | `is not set` |
| `CONFIG_MLX90614` | `is not set` |
| `CONFIG_MLX90632` | `is not set` |
| `CONFIG_MLX90635` | `is not set` |
| `CONFIG_MMA7455_I2C` | `is not set` |
| `CONFIG_MMA7455_SPI` | `is not set` |
| `CONFIG_MMA7660` | `is not set` |
| `CONFIG_MMA8452` | `is not set` |
| `CONFIG_MMA9551` | `is not set` |
| `CONFIG_MMA9553` | `is not set` |
| `CONFIG_MMC35240` | `is not set` |
| `CONFIG_MMC_ARMMMCI` | `is not set` |
| `CONFIG_MMC_DW_BLUEFIELD` | `is not set` |
| `CONFIG_MMC_DW_EXYNOS` | `is not set` |
| `CONFIG_MMC_DW_HI3798CV200` | `is not set` |
| `CONFIG_MMC_DW_HI3798MV200` | `is not set` |
| `CONFIG_MMC_DW_K3` | `is not set` |
| `CONFIG_MMC_DW_PCI` | `is not set` |
| `CONFIG_MMC_DW_PLTFM` | `y` |
| `CONFIG_MMC_DW_STARFIVE` | `y` |
| `CONFIG_MMC_SDHCI_ACPI` | `is not set` |
| `CONFIG_MMC_SDHCI_CADENCE` | `y` |
| `CONFIG_MMC_SDHCI_F_SDH30` | `is not set` |
| `CONFIG_MMC_SDHCI_IO_ACCESSORS` | `y` |
| `CONFIG_MMC_SDHCI_MILBEAUT` | `is not set` |
| `CONFIG_MMC_SDHCI_OF_ARASAN` | `is not set` |
| `CONFIG_MMC_SDHCI_OF_AT91` | `is not set` |
| `CONFIG_MMC_SDHCI_OF_DWCMSHC` | `y` |
| `CONFIG_MMC_SDHCI_OF_K1` | `y` |
| `CONFIG_MMC_SDHCI_PCI` | `is not set` |
| `CONFIG_MMC_SDHCI_PLTFM` | `y` |
| `CONFIG_MMC_SDHCI_XENON` | `is not set` |
| `CONFIG_MMC_SUNXI` | `y` |
| `CONFIG_MMP_PDMA` | `m` |
| `CONFIG_MMU_GATHER_RCU_TABLE_FREE` | `y` |
| `CONFIG_MMU_GATHER_TABLE_FREE` | `y` |
| `CONFIG_MMU_LAZY_TLB_REFCOUNT` | `y` |
| `CONFIG_MMU_NOTIFIER` | `y` |
| `CONFIG_MODPROBE_PATH` | `"/sbin/modprobe"` |
| `CONFIG_MODULES_TREE_LOOKUP` | `y` |
| `CONFIG_MODULE_DEBUG` | `is not set` |
| `CONFIG_MODULE_UNLOAD_TAINT_TRACKING` | `is not set` |
| `CONFIG_MOST` | `is not set` |
| `CONFIG_MOTORCOMM_PHY` | `y` |
| `CONFIG_MOUSE_GPIO` | `is not set` |
| `CONFIG_MPILIB` | `m` |
| `CONFIG_MPL115_I2C` | `is not set` |
| `CONFIG_MPL115_SPI` | `is not set` |
| `CONFIG_MPL3115` | `is not set` |
| `CONFIG_MPRLS0025PA` | `is not set` |
| `CONFIG_MPU3050_I2C` | `is not set` |
| `CONFIG_MS5611` | `is not set` |
| `CONFIG_MS5637` | `is not set` |
| `CONFIG_MSA311` | `is not set` |
| `CONFIG_MSE102X` | `is not set` |
| `CONFIG_MTD_ABSENT` | `is not set` |
| `CONFIG_MTD_BLKDEVS` | `y` |
| `CONFIG_MTD_BLOCK` | `y` |
| `CONFIG_MTD_BLOCK2MTD` | `is not set` |
| `CONFIG_MTD_CFI` | `y` |
| `CONFIG_MTD_CFI_ADV_OPTIONS` | `y` |
| `CONFIG_MTD_CFI_AMDSTD` | `is not set` |
| `CONFIG_MTD_CFI_BE_BYTE_SWAP` | `is not set` |
| `CONFIG_MTD_CFI_GEOMETRY` | `is not set` |
| `CONFIG_MTD_CFI_I1` | `y` |
| `CONFIG_MTD_CFI_I2` | `y` |
| `CONFIG_MTD_CFI_INTELEXT` | `is not set` |
| `CONFIG_MTD_CFI_LE_BYTE_SWAP` | `is not set` |
| `CONFIG_MTD_CFI_NOSWAP` | `y` |
| `CONFIG_MTD_CFI_STAA` | `is not set` |
| `CONFIG_MTD_CFI_UTIL` | `y` |
| `CONFIG_MTD_CMDLINE_PARTS` | `is not set` |
| `CONFIG_MTD_COMPLEX_MAPPINGS` | `is not set` |
| `CONFIG_MTD_DATAFLASH` | `is not set` |
| `CONFIG_MTD_DOCG3` | `is not set` |
| `CONFIG_MTD_GEN_PROBE` | `y` |
| `CONFIG_MTD_HYPERBUS` | `is not set` |
| `CONFIG_MTD_JEDECPROBE` | `is not set` |
| `CONFIG_MTD_LPDDR` | `is not set` |
| `CONFIG_MTD_MAP_BANK_WIDTH_1` | `y` |
| `CONFIG_MTD_MAP_BANK_WIDTH_2` | `y` |
| `CONFIG_MTD_MAP_BANK_WIDTH_4` | `y` |
| `CONFIG_MTD_MCHP23K256` | `is not set` |
| `CONFIG_MTD_MCHP48L640` | `is not set` |
| `CONFIG_MTD_MTDRAM` | `is not set` |
| `CONFIG_MTD_NAND_ECC_MXIC` | `is not set` |
| `CONFIG_MTD_NAND_ECC_SW_BCH` | `is not set` |
| `CONFIG_MTD_NAND_ECC_SW_HAMMING` | `is not set` |
| `CONFIG_MTD_OF_PARTS` | `y` |
| `CONFIG_MTD_ONENAND` | `is not set` |
| `CONFIG_MTD_OOPS` | `is not set` |
| `CONFIG_MTD_OTP` | `is not set` |
| `CONFIG_MTD_PARTITIONED_MASTER` | `is not set` |
| `CONFIG_MTD_PHRAM` | `is not set` |
| `CONFIG_MTD_PHYSMAP` | `is not set` |
| `CONFIG_MTD_PLATRAM` | `is not set` |
| `CONFIG_MTD_PMC551` | `is not set` |
| `CONFIG_MTD_RAM` | `is not set` |
| `CONFIG_MTD_RAW_NAND` | `is not set` |
| `CONFIG_MTD_REDBOOT_PARTS` | `is not set` |
| `CONFIG_MTD_ROM` | `is not set` |
| `CONFIG_MTD_SLRAM` | `is not set` |
| `CONFIG_MTD_SPI_NAND` | `is not set` |
| `CONFIG_MTD_SPI_NOR` | `y` |
| `CONFIG_MTD_SPI_NOR_SWP_DISABLE` | `is not set` |
| `CONFIG_MTD_SPI_NOR_SWP_DISABLE_ON_VOLATILE` | `y` |
| `CONFIG_MTD_SPI_NOR_SWP_KEEP` | `is not set` |
| `CONFIG_MTD_SPI_NOR_USE_4K_SECTORS` | `y` |
| `CONFIG_MTD_SST25L` | `is not set` |
| `CONFIG_MTD_SWAP` | `is not set` |
| `CONFIG_MTD_TESTS` | `is not set` |
| `CONFIG_MTD_UBI` | `is not set` |
| `CONFIG_MUSB_PIO_ONLY` | `is not set` |
| `CONFIG_MXC4005` | `is not set` |
| `CONFIG_MXC6255` | `is not set` |
| `CONFIG_NAU7802` | `is not set` |
| `CONFIG_NCN26000_PHY` | `is not set` |
| `CONFIG_NCT7201` | `is not set` |
| `CONFIG_ND_BTT` | `y` |
| `CONFIG_ND_CLAIM` | `y` |
| `CONFIG_NEED_TASKS_RCU` | `y` |
| `CONFIG_NETFILTER_ADVANCED` | `y` |
| `CONFIG_NETFILTER_BPF_LINK` | `y` |
| `CONFIG_NETFILTER_EGRESS` | `y` |
| `CONFIG_NETFILTER_FAMILY_BRIDGE` | `y` |
| `CONFIG_NETFILTER_INGRESS` | `y` |
| `CONFIG_NETFILTER_NETLINK_ACCT` | `is not set` |
| `CONFIG_NETFILTER_NETLINK_LOG` | `is not set` |
| `CONFIG_NETFILTER_NETLINK_OSF` | `is not set` |
| `CONFIG_NETFILTER_NETLINK_QUEUE` | `is not set` |
| `CONFIG_NETFILTER_XTABLES` | `m` |
| `CONFIG_NETFILTER_XTABLES_COMPAT` | `is not set` |
| `CONFIG_NETFILTER_XTABLES_LEGACY` | `is not set` |
| `CONFIG_NETFILTER_XT_CONNMARK` | `is not set` |
| `CONFIG_NETFILTER_XT_MARK` | `m` |
| `CONFIG_NETFILTER_XT_MATCH_ADDRTYPE` | `m` |
| `CONFIG_NETFILTER_XT_MATCH_BPF` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_CGROUP` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_CLUSTER` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_COMMENT` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_CONNBYTES` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_CONNLABEL` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_CONNLIMIT` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_CONNMARK` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_CONNTRACK` | `m` |
| `CONFIG_NETFILTER_XT_MATCH_CPU` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_DCCP` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_DEVGROUP` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_DSCP` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_ECN` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_ESP` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_HASHLIMIT` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_HELPER` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_HL` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_IPCOMP` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_IPRANGE` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_IPVS` | `m` |
| `CONFIG_NETFILTER_XT_MATCH_L2TP` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_LENGTH` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_LIMIT` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_MAC` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_MARK` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_MULTIPORT` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_NFACCT` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_OSF` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_OWNER` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_PHYSDEV` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_PKTTYPE` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_POLICY` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_QUOTA` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_RATEEST` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_REALM` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_RECENT` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_SCTP` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_SOCKET` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_STATE` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_STATISTIC` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_STRING` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_TCPMSS` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_TIME` | `is not set` |
| `CONFIG_NETFILTER_XT_MATCH_U32` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_AUDIT` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_CLASSIFY` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_CONNMARK` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_HMARK` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_IDLETIMER` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_LOG` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_MARK` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_NFLOG` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_NFQUEUE` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_RATEEST` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_SECMARK` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_TCPMSS` | `is not set` |
| `CONFIG_NETFILTER_XT_TARGET_TEE` | `is not set` |
| `CONFIG_NETFS_DEBUG` | `is not set` |
| `CONFIG_NETFS_STATS` | `is not set` |
| `CONFIG_NETFS_SUPPORT` | `y` |
| `CONFIG_NETKIT` | `is not set` |
| `CONFIG_NETLABEL` | `is not set` |
| `CONFIG_NET_9P_FD` | `y` |
| `CONFIG_NET_9P_USBG` | `is not set` |
| `CONFIG_NET_CLS` | `y` |
| `CONFIG_NET_CLS_ACT` | `is not set` |
| `CONFIG_NET_CLS_BASIC` | `is not set` |
| `CONFIG_NET_CLS_BPF` | `is not set` |
| `CONFIG_NET_CLS_CGROUP` | `m` |
| `CONFIG_NET_CLS_FLOW` | `is not set` |
| `CONFIG_NET_CLS_FLOWER` | `is not set` |
| `CONFIG_NET_CLS_FW` | `is not set` |
| `CONFIG_NET_CLS_MATCHALL` | `is not set` |
| `CONFIG_NET_CLS_ROUTE4` | `is not set` |
| `CONFIG_NET_CLS_U32` | `is not set` |
| `CONFIG_NET_CRC32C` | `y` |
| `CONFIG_NET_DEVLINK` | `y` |
| `CONFIG_NET_DEVMEM` | `y` |
| `CONFIG_NET_DEV_REFCNT_TRACKER` | `is not set` |
| `CONFIG_NET_DROP_MONITOR` | `is not set` |
| `CONFIG_NET_EGRESS` | `y` |
| `CONFIG_NET_EMATCH` | `is not set` |
| `CONFIG_NET_HANDSHAKE` | `y` |
| `CONFIG_NET_INGRESS` | `y` |
| `CONFIG_NET_NS_REFCNT_TRACKER` | `is not set` |
| `CONFIG_NET_SCH_CAKE` | `is not set` |
| `CONFIG_NET_SCH_CBS` | `is not set` |
| `CONFIG_NET_SCH_CHOKE` | `is not set` |
| `CONFIG_NET_SCH_CODEL` | `is not set` |
| `CONFIG_NET_SCH_DEFAULT` | `is not set` |
| `CONFIG_NET_SCH_DRR` | `is not set` |
| `CONFIG_NET_SCH_DUALPI2` | `is not set` |
| `CONFIG_NET_SCH_ETF` | `is not set` |
| `CONFIG_NET_SCH_ETS` | `is not set` |
| `CONFIG_NET_SCH_FIFO` | `y` |
| `CONFIG_NET_SCH_FQ` | `is not set` |
| `CONFIG_NET_SCH_FQ_CODEL` | `is not set` |
| `CONFIG_NET_SCH_GRED` | `is not set` |
| `CONFIG_NET_SCH_HFSC` | `is not set` |
| `CONFIG_NET_SCH_HHF` | `is not set` |
| `CONFIG_NET_SCH_HTB` | `is not set` |
| `CONFIG_NET_SCH_MQPRIO` | `is not set` |
| `CONFIG_NET_SCH_MULTIQ` | `is not set` |
| `CONFIG_NET_SCH_NETEM` | `is not set` |
| `CONFIG_NET_SCH_PIE` | `is not set` |
| `CONFIG_NET_SCH_PLUG` | `is not set` |
| `CONFIG_NET_SCH_PRIO` | `is not set` |
| `CONFIG_NET_SCH_QFQ` | `is not set` |
| `CONFIG_NET_SCH_RED` | `is not set` |
| `CONFIG_NET_SCH_SFB` | `is not set` |
| `CONFIG_NET_SCH_SFQ` | `is not set` |
| `CONFIG_NET_SCH_SKBPRIO` | `is not set` |
| `CONFIG_NET_SCH_TAPRIO` | `is not set` |
| `CONFIG_NET_SCH_TBF` | `is not set` |
| `CONFIG_NET_SCH_TEQL` | `is not set` |
| `CONFIG_NET_SELFTESTS` | `y` |
| `CONFIG_NET_SOCK_MSG` | `y` |
| `CONFIG_NET_UDP_TUNNEL` | `m` |
| `CONFIG_NET_VENDOR_ADI` | `y` |
| `CONFIG_NET_VENDOR_ALLWINNER` | `y` |
| `CONFIG_NET_VENDOR_ASIX` | `y` |
| `CONFIG_NET_VENDOR_DAVICOM` | `y` |
| `CONFIG_NET_VENDOR_ENGLEDER` | `y` |
| `CONFIG_NET_VENDOR_FUNGIBLE` | `y` |
| `CONFIG_NET_VENDOR_HISILICON` | `y` |
| `CONFIG_NET_VENDOR_LITEX` | `y` |
| `CONFIG_NET_VENDOR_META` | `y` |
| `CONFIG_NET_VENDOR_MICROSOFT` | `y` |
| `CONFIG_NET_VENDOR_MUCSE` | `y` |
| `CONFIG_NET_VENDOR_SPACEMIT` | `y` |
| `CONFIG_NET_VENDOR_VERTEXCOM` | `y` |
| `CONFIG_NET_VENDOR_WANGXUN` | `y` |
| `CONFIG_NET_XGRESS` | `y` |
| `CONFIG_NFS_FSCACHE` | `is not set` |
| `CONFIG_NFS_V4_2_SSC_HELPER` | `y` |
| `CONFIG_NFS_V4_SECURITY_LABEL` | `y` |
| `CONFIG_NFTL` | `is not set` |
| `CONFIG_NF_CONNTRACK` | `m` |
| `CONFIG_NF_CONNTRACK_AMANDA` | `is not set` |
| `CONFIG_NF_CONNTRACK_BRIDGE` | `is not set` |
| `CONFIG_NF_CONNTRACK_EVENTS` | `is not set` |
| `CONFIG_NF_CONNTRACK_FTP` | `m` |
| `CONFIG_NF_CONNTRACK_H323` | `is not set` |
| `CONFIG_NF_CONNTRACK_IRC` | `is not set` |
| `CONFIG_NF_CONNTRACK_LABELS` | `is not set` |
| `CONFIG_NF_CONNTRACK_MARK` | `is not set` |
| `CONFIG_NF_CONNTRACK_NETBIOS_NS` | `is not set` |
| `CONFIG_NF_CONNTRACK_PPTP` | `is not set` |
| `CONFIG_NF_CONNTRACK_PROCFS` | `is not set` |
| `CONFIG_NF_CONNTRACK_SANE` | `is not set` |
| `CONFIG_NF_CONNTRACK_SECMARK` | `is not set` |
| `CONFIG_NF_CONNTRACK_SIP` | `is not set` |
| `CONFIG_NF_CONNTRACK_SNMP` | `is not set` |
| `CONFIG_NF_CONNTRACK_TFTP` | `m` |
| `CONFIG_NF_CONNTRACK_TIMEOUT` | `is not set` |
| `CONFIG_NF_CONNTRACK_TIMESTAMP` | `is not set` |
| `CONFIG_NF_CONNTRACK_ZONES` | `is not set` |
| `CONFIG_NF_CT_NETLINK` | `is not set` |
| `CONFIG_NF_CT_PROTO_SCTP` | `y` |
| `CONFIG_NF_CT_PROTO_UDPLITE` | `y` |
| `CONFIG_NF_DEFRAG_IPV4` | `m` |
| `CONFIG_NF_DEFRAG_IPV6` | `m` |
| `CONFIG_NF_DUP_IPV4` | `is not set` |
| `CONFIG_NF_DUP_IPV6` | `is not set` |
| `CONFIG_NF_LOG_ARP` | `m` |
| `CONFIG_NF_LOG_IPV4` | `m` |
| `CONFIG_NF_LOG_IPV6` | `m` |
| `CONFIG_NF_LOG_SYSLOG` | `m` |
| `CONFIG_NF_NAT` | `is not set` |
| `CONFIG_NF_REJECT_IPV4` | `is not set` |
| `CONFIG_NF_REJECT_IPV6` | `is not set` |
| `CONFIG_NF_SOCKET_IPV4` | `is not set` |
| `CONFIG_NF_SOCKET_IPV6` | `is not set` |
| `CONFIG_NF_TABLES` | `is not set` |
| `CONFIG_NF_TPROXY_IPV4` | `is not set` |
| `CONFIG_NF_TPROXY_IPV6` | `is not set` |
| `CONFIG_NGBE` | `is not set` |
| `CONFIG_NGBEVF` | `is not set` |
| `CONFIG_NIC7018_WDT` | `is not set` |
| `CONFIG_NOA1305` | `is not set` |
| `CONFIG_NONPORTABLE` | `is not set` |
| `CONFIG_NOP_TRACER` | `y` |
| `CONFIG_NOUVEAU_DEBUG` | `5` |
| `CONFIG_NOUVEAU_DEBUG_DEFAULT` | `3` |
| `CONFIG_NOUVEAU_DEBUG_MMU` | `is not set` |
| `CONFIG_NOUVEAU_DEBUG_PUSH` | `is not set` |
| `CONFIG_NSM` | `is not set` |
| `CONFIG_NTFS3_FS` | `is not set` |
| `CONFIG_NTSYNC` | `is not set` |
| `CONFIG_NUMA` | `is not set` |
| `CONFIG_NVMEM_LAYOUTS` | `y` |
| `CONFIG_NVMEM_LAYOUT_ONIE_TLV` | `is not set` |
| `CONFIG_NVMEM_LAYOUT_SL28_VPD` | `is not set` |
| `CONFIG_NVMEM_LAYOUT_U_BOOT_ENV` | `is not set` |
| `CONFIG_NVMEM_RMEM` | `is not set` |
| `CONFIG_NVMEM_SUNXI_SID` | `y` |
| `CONFIG_NVMEM_U_BOOT_ENV` | `is not set` |
| `CONFIG_NVME_CORE` | `m` |
| `CONFIG_NVME_HOST_AUTH` | `is not set` |
| `CONFIG_NVME_HWMON` | `is not set` |
| `CONFIG_NVME_MULTIPATH` | `is not set` |
| `CONFIG_NVME_TARGET` | `is not set` |
| `CONFIG_NVME_TCP` | `is not set` |
| `CONFIG_NVME_VERBOSE_ERRORS` | `is not set` |
| `CONFIG_NXP_C45_TJA11XX_PHY` | `is not set` |
| `CONFIG_NXP_CBTX_PHY` | `is not set` |
| `CONFIG_OCFS2_FS` | `is not set` |
| `CONFIG_OCTEON_EP` | `is not set` |
| `CONFIG_OCTEON_EP_VF` | `is not set` |
| `CONFIG_OF_GPIO` | `y` |
| `CONFIG_OF_IOMMU` | `y` |
| `CONFIG_OF_PMEM` | `y` |
| `CONFIG_OPEN_DICE` | `is not set` |
| `CONFIG_OPT3001` | `is not set` |
| `CONFIG_OPT4001` | `is not set` |
| `CONFIG_OPT4060` | `is not set` |
| `CONFIG_OSNOISE_TRACER` | `is not set` |
| `CONFIG_OVERLAY_FS_DEBUG` | `is not set` |
| `CONFIG_OVERLAY_FS_INDEX` | `is not set` |
| `CONFIG_OVERLAY_FS_METACOPY` | `is not set` |
| `CONFIG_OVERLAY_FS_REDIRECT_ALWAYS_FOLLOW` | `y` |
| `CONFIG_OVERLAY_FS_REDIRECT_DIR` | `is not set` |
| `CONFIG_OVERLAY_FS_XINO_AUTO` | `is not set` |
| `CONFIG_OVMF_DEBUG_LOG` | `is not set` |
| `CONFIG_OVPN` | `is not set` |
| `CONFIG_PA12203001` | `is not set` |
| `CONFIG_PAC1921` | `is not set` |
| `CONFIG_PAC1934` | `is not set` |
| `CONFIG_PADATA` | `y` |
| `CONFIG_PAGE_BLOCK_MAX_ORDER` | `10` |
| `CONFIG_PAGE_COUNTER` | `y` |
| `CONFIG_PAGE_MAPCOUNT` | `y` |
| `CONFIG_PAGE_POOL` | `y` |
| `CONFIG_PAGE_POOL_STATS` | `is not set` |
| `CONFIG_PAGE_SHIFT` | `12` |
| `CONFIG_PAGE_SIZE_4KB` | `y` |
| `CONFIG_PAGE_SIZE_LESS_THAN_256KB` | `y` |
| `CONFIG_PAGE_SIZE_LESS_THAN_64KB` | `y` |
| `CONFIG_PAHOLE_VERSION` | `125` |
| `CONFIG_PARAVIRT` | `is not set` |
| `CONFIG_PATA_ACPI` | `is not set` |
| `CONFIG_PATA_OF_PLATFORM` | `is not set` |
| `CONFIG_PATA_TIMINGS` | `y` |
| `CONFIG_PCC` | `y` |
| `CONFIG_PCIE_DW` | `y` |
| `CONFIG_PCIE_DW_DEBUGFS` | `is not set` |
| `CONFIG_PCIE_DW_HOST` | `y` |
| `CONFIG_PCIE_FU740` | `y` |
| `CONFIG_PCIE_MICROCHIP_HOST` | `is not set` |
| `CONFIG_PCIE_PLDA_HOST` | `y` |
| `CONFIG_PCIE_PME` | `y` |
| `CONFIG_PCIE_SG2042_HOST` | `is not set` |
| `CONFIG_PCIE_SOPHGO_DW` | `is not set` |
| `CONFIG_PCIE_SPACEMIT_K1` | `y` |
| `CONFIG_PCIE_STARFIVE_HOST` | `m` |
| `CONFIG_PCIE_THERMAL` | `is not set` |
| `CONFIG_PCIE_TPH` | `is not set` |
| `CONFIG_PCIPCWATCHDOG` | `is not set` |
| `CONFIG_PCI_DOE` | `is not set` |
| `CONFIG_PCI_DYNAMIC_OF_NODES` | `is not set` |
| `CONFIG_PCI_LABEL` | `y` |
| `CONFIG_PCI_PWRCTRL` | `y` |
| `CONFIG_PCI_PWRCTRL_SLOT` | `y` |
| `CONFIG_PCI_PWRCTRL_TC9563` | `is not set` |
| `CONFIG_PCI_TSM` | `is not set` |
| `CONFIG_PCPU_DEV_REFCNT` | `y` |
| `CONFIG_PCP_BATCH_SCALE_MAX` | `5` |
| `CONFIG_PDS_CORE` | `is not set` |
| `CONFIG_PECI` | `is not set` |
| `CONFIG_PER_VMA_LOCK` | `y` |
| `CONFIG_PER_VMA_LOCK_STATS` | `is not set` |
| `CONFIG_PFCP` | `is not set` |
| `CONFIG_PGTABLE_HAS_HUGE_LEAVES` | `y` |
| `CONFIG_PHY_CADENCE_DPHY_RX` | `is not set` |
| `CONFIG_PHY_CADENCE_SIERRA` | `is not set` |
| `CONFIG_PHY_CAN_TRANSCEIVER` | `is not set` |
| `CONFIG_PHY_CPCAP_USB` | `is not set` |
| `CONFIG_PHY_MAPPHONE_MDM6600` | `is not set` |
| `CONFIG_PHY_NXP_PTN3222` | `is not set` |
| `CONFIG_PHY_PACKAGE` | `y` |
| `CONFIG_PHY_SOPHGO_CV1800_USB2` | `is not set` |
| `CONFIG_PHY_STARFIVE_JH7110_DPHY_RX` | `m` |
| `CONFIG_PHY_STARFIVE_JH7110_DPHY_TX` | `is not set` |
| `CONFIG_PHY_STARFIVE_JH7110_PCIE` | `m` |
| `CONFIG_PHY_STARFIVE_JH7110_USB` | `m` |
| `CONFIG_PHY_SUN4I_USB` | `m` |
| `CONFIG_PHY_SUN50I_USB3` | `is not set` |
| `CONFIG_PHY_SUN6I_MIPI_DPHY` | `m` |
| `CONFIG_PHY_SUN9I_USB` | `is not set` |
| `CONFIG_PINCONF` | `y` |
| `CONFIG_PINCTRL_AMD` | `is not set` |
| `CONFIG_PINCTRL_AW9523` | `is not set` |
| `CONFIG_PINCTRL_AXP209` | `is not set` |
| `CONFIG_PINCTRL_CY8C95X0` | `is not set` |
| `CONFIG_PINCTRL_K230` | `is not set` |
| `CONFIG_PINCTRL_MCP23S08` | `is not set` |
| `CONFIG_PINCTRL_MICROCHIP_SGPIO` | `is not set` |
| `CONFIG_PINCTRL_OCELOT` | `is not set` |
| `CONFIG_PINCTRL_PIC64GX` | `y` |
| `CONFIG_PINCTRL_POLARFIRE_SOC` | `y` |
| `CONFIG_PINCTRL_SINGLE` | `is not set` |
| `CONFIG_PINCTRL_SOPHGO_COMMON` | `y` |
| `CONFIG_PINCTRL_SOPHGO_CV1800B` | `y` |
| `CONFIG_PINCTRL_SOPHGO_CV1812H` | `y` |
| `CONFIG_PINCTRL_SOPHGO_CV18XX_OPS` | `y` |
| `CONFIG_PINCTRL_SOPHGO_SG2000` | `y` |
| `CONFIG_PINCTRL_SOPHGO_SG2002` | `y` |
| `CONFIG_PINCTRL_SOPHGO_SG2042` | `is not set` |
| `CONFIG_PINCTRL_SOPHGO_SG2044` | `is not set` |
| `CONFIG_PINCTRL_SPACEMIT_K1` | `y` |
| `CONFIG_PINCTRL_STARFIVE_JH7100` | `y` |
| `CONFIG_PINCTRL_STARFIVE_JH7110` | `y` |
| `CONFIG_PINCTRL_STARFIVE_JH7110_AON` | `y` |
| `CONFIG_PINCTRL_STARFIVE_JH7110_SYS` | `y` |
| `CONFIG_PINCTRL_STMFX` | `is not set` |
| `CONFIG_PINCTRL_SUN20I_D1` | `y` |
| `CONFIG_PINCTRL_SUN4I_A10` | `is not set` |
| `CONFIG_PINCTRL_SUN50I_A100` | `is not set` |
| `CONFIG_PINCTRL_SUN50I_A100_R` | `is not set` |
| `CONFIG_PINCTRL_SUN50I_A64` | `is not set` |
| `CONFIG_PINCTRL_SUN50I_A64_R` | `is not set` |
| `CONFIG_PINCTRL_SUN50I_H5` | `is not set` |
| `CONFIG_PINCTRL_SUN50I_H6` | `is not set` |
| `CONFIG_PINCTRL_SUN50I_H616` | `is not set` |
| `CONFIG_PINCTRL_SUN50I_H616_R` | `is not set` |
| `CONFIG_PINCTRL_SUN50I_H6_R` | `is not set` |
| `CONFIG_PINCTRL_SUN55I_A523` | `is not set` |
| `CONFIG_PINCTRL_SUN55I_A523_R` | `is not set` |
| `CONFIG_PINCTRL_SUN5I` | `is not set` |
| `CONFIG_PINCTRL_SUN6I_A31` | `is not set` |
| `CONFIG_PINCTRL_SUN6I_A31_R` | `is not set` |
| `CONFIG_PINCTRL_SUN8I_A23` | `is not set` |
| `CONFIG_PINCTRL_SUN8I_A23_R` | `is not set` |
| `CONFIG_PINCTRL_SUN8I_A33` | `is not set` |
| `CONFIG_PINCTRL_SUN8I_A83T` | `is not set` |
| `CONFIG_PINCTRL_SUN8I_A83T_R` | `is not set` |
| `CONFIG_PINCTRL_SUN8I_H3` | `is not set` |
| `CONFIG_PINCTRL_SUN8I_H3_R` | `is not set` |
| `CONFIG_PINCTRL_SUN8I_V3S` | `is not set` |
| `CONFIG_PINCTRL_SUN9I_A80` | `is not set` |
| `CONFIG_PINCTRL_SUN9I_A80_R` | `is not set` |
| `CONFIG_PINCTRL_SUNXI` | `y` |
| `CONFIG_PINCTRL_SX150X` | `is not set` |
| `CONFIG_PINCTRL_TH1520` | `y` |
| `CONFIG_PING` | `is not set` |
| `CONFIG_PINMUX` | `y` |
| `CONFIG_PL320_MBOX` | `is not set` |
| `CONFIG_PL330_DMA` | `is not set` |
| `CONFIG_PLATFORM_MHU` | `is not set` |
| `CONFIG_PLX_DMA` | `is not set` |
| `CONFIG_PMIC_OPREGION` | `is not set` |
| `CONFIG_PM_AUTOSLEEP` | `is not set` |
| `CONFIG_PM_CLK` | `y` |
| `CONFIG_PM_DEBUG` | `is not set` |
| `CONFIG_PM_DEVFREQ_EVENT` | `is not set` |
| `CONFIG_PM_GENERIC_DOMAINS` | `y` |
| `CONFIG_PM_GENERIC_DOMAINS_OF` | `y` |
| `CONFIG_PM_GENERIC_DOMAINS_SLEEP` | `y` |
| `CONFIG_PM_OPP` | `y` |
| `CONFIG_PM_QOS_CPU_SYSTEM_WAKEUP` | `is not set` |
| `CONFIG_PM_SLEEP` | `y` |
| `CONFIG_PM_SLEEP_SMP` | `y` |
| `CONFIG_PM_USERSPACE_AUTOSLEEP` | `is not set` |
| `CONFIG_PM_WAKELOCKS` | `is not set` |
| `CONFIG_PNFS_BLOCK` | `m` |
| `CONFIG_PNP` | `y` |
| `CONFIG_PNPACPI` | `y` |
| `CONFIG_PNP_DEBUG_MESSAGES` | `y` |
| `CONFIG_POLARFIRE_SOC_MAILBOX` | `is not set` |
| `CONFIG_POLARFIRE_SOC_SYSCONS` | `y` |
| `CONFIG_PORTABLE` | `y` |
| `CONFIG_POSIX_AUX_CLOCKS` | `is not set` |
| `CONFIG_POSIX_CPU_TIMERS_TASK_WORK` | `y` |
| `CONFIG_POWER_RESET_GPIO` | `is not set` |
| `CONFIG_POWER_RESET_GPIO_RESTART` | `y` |
| `CONFIG_POWER_RESET_LTC2952` | `is not set` |
| `CONFIG_POWER_RESET_REGULATOR` | `is not set` |
| `CONFIG_POWER_RESET_SPACEMIT_P1` | `y` |
| `CONFIG_POWER_SEQUENCING` | `is not set` |
| `CONFIG_PREEMPTIRQ_DELAY_TEST` | `is not set` |
| `CONFIG_PREEMPT_DYNAMIC` | `is not set` |
| `CONFIG_PREEMPT_LAZY` | `is not set` |
| `CONFIG_PREEMPT_NONE_BUILD` | `y` |
| `CONFIG_PREEMPT_NOTIFIERS` | `y` |
| `CONFIG_PRINTK_INDEX` | `is not set` |
| `CONFIG_PROBE_EVENTS` | `y` |
| `CONFIG_PROC_MEM_ALWAYS_FORCE` | `y` |
| `CONFIG_PROC_MEM_FORCE_PTRACE` | `is not set` |
| `CONFIG_PROC_MEM_NO_FORCE` | `is not set` |
| `CONFIG_PROFILE_ALL_BRANCHES` | `is not set` |
| `CONFIG_PROFILE_ANNOTATED_BRANCHES` | `is not set` |
| `CONFIG_PSE_CONTROLLER` | `is not set` |
| `CONFIG_PTP_1588_CLOCK_OPTIONAL` | `y` |
| `CONFIG_PWM_ARGON_FAN_HAT` | `is not set` |
| `CONFIG_PWM_ATMEL_TCB` | `is not set` |
| `CONFIG_PWM_CLK` | `is not set` |
| `CONFIG_PWM_DEBUG` | `is not set` |
| `CONFIG_PWM_DWC` | `is not set` |
| `CONFIG_PWM_FSL_FTM` | `is not set` |
| `CONFIG_PWM_GPIO` | `is not set` |
| `CONFIG_PWM_MC33XS2410` | `is not set` |
| `CONFIG_PWM_MICROCHIP_CORE` | `is not set` |
| `CONFIG_PWM_PCA9685` | `is not set` |
| `CONFIG_PWM_PROVIDE_GPIO` | `is not set` |
| `CONFIG_PWM_PXA` | `m` |
| `CONFIG_PWM_SIFIVE` | `is not set` |
| `CONFIG_PWM_SOPHGO_SG2042` | `is not set` |
| `CONFIG_PWM_SUN4I` | `is not set` |
| `CONFIG_PWM_XILINX` | `is not set` |
| `CONFIG_QCA807X_PHY` | `is not set` |
| `CONFIG_QCA808X_PHY` | `is not set` |
| `CONFIG_QCA83XX_PHY` | `is not set` |
| `CONFIG_QCOM_HIDMA` | `is not set` |
| `CONFIG_QCOM_HIDMA_MGMT` | `is not set` |
| `CONFIG_QCOM_PMIC_PDCHARGER_ULOG` | `is not set` |
| `CONFIG_QUEUED_RWLOCKS` | `y` |
| `CONFIG_QUEUED_SPINLOCKS` | `y` |
| `CONFIG_RAID6_PQ` | `m` |
| `CONFIG_RAID6_PQ_BENCHMARK` | `y` |
| `CONFIG_RAMDAX` | `y` |
| `CONFIG_RANDOMIZE_BASE` | `is not set` |
| `CONFIG_RANDOMIZE_KSTACK_OFFSET` | `y` |
| `CONFIG_RANDOMIZE_KSTACK_OFFSET_DEFAULT` | `is not set` |
| `CONFIG_RANDOM_KMALLOC_CACHES` | `is not set` |
| `CONFIG_RANDSTRUCT_FULL` | `is not set` |
| `CONFIG_RANDSTRUCT_NONE` | `y` |
| `CONFIG_RANDSTRUCT_PERFORMANCE` | `is not set` |
| `CONFIG_RCU_CPU_STALL_CPUTIME` | `is not set` |
| `CONFIG_RCU_EXP_CPU_STALL_TIMEOUT` | `0` |
| `CONFIG_REALTEK_PHY_HWMON` | `is not set` |
| `CONFIG_REGMAP_I2C` | `y` |
| `CONFIG_REGMAP_IRQ` | `y` |
| `CONFIG_REGMAP_SPI` | `y` |
| `CONFIG_REGULATOR_88PG86X` | `is not set` |
| `CONFIG_REGULATOR_ACT8865` | `is not set` |
| `CONFIG_REGULATOR_AD5398` | `is not set` |
| `CONFIG_REGULATOR_ADP5055` | `is not set` |
| `CONFIG_REGULATOR_AW37503` | `is not set` |
| `CONFIG_REGULATOR_AXP20X` | `y` |
| `CONFIG_REGULATOR_DA9121` | `is not set` |
| `CONFIG_REGULATOR_DA9210` | `is not set` |
| `CONFIG_REGULATOR_DA9211` | `is not set` |
| `CONFIG_REGULATOR_DEBUG` | `is not set` |
| `CONFIG_REGULATOR_FAN53555` | `is not set` |
| `CONFIG_REGULATOR_FAN53880` | `is not set` |
| `CONFIG_REGULATOR_FIXED_VOLTAGE` | `y` |
| `CONFIG_REGULATOR_FP9931` | `is not set` |
| `CONFIG_REGULATOR_GPIO` | `y` |
| `CONFIG_REGULATOR_ISL6271A` | `is not set` |
| `CONFIG_REGULATOR_ISL9305` | `is not set` |
| `CONFIG_REGULATOR_LP3971` | `is not set` |
| `CONFIG_REGULATOR_LP3972` | `is not set` |
| `CONFIG_REGULATOR_LP872X` | `is not set` |
| `CONFIG_REGULATOR_LP8755` | `is not set` |
| `CONFIG_REGULATOR_LTC3589` | `is not set` |
| `CONFIG_REGULATOR_LTC3676` | `is not set` |
| `CONFIG_REGULATOR_MAX1586` | `is not set` |
| `CONFIG_REGULATOR_MAX20086` | `is not set` |
| `CONFIG_REGULATOR_MAX20411` | `is not set` |
| `CONFIG_REGULATOR_MAX77503` | `is not set` |
| `CONFIG_REGULATOR_MAX77826` | `is not set` |
| `CONFIG_REGULATOR_MAX77838` | `is not set` |
| `CONFIG_REGULATOR_MAX77857` | `is not set` |
| `CONFIG_REGULATOR_MAX8649` | `is not set` |
| `CONFIG_REGULATOR_MAX8660` | `is not set` |
| `CONFIG_REGULATOR_MAX8893` | `is not set` |
| `CONFIG_REGULATOR_MAX8952` | `is not set` |
| `CONFIG_REGULATOR_MAX8973` | `is not set` |
| `CONFIG_REGULATOR_MCP16502` | `is not set` |
| `CONFIG_REGULATOR_MP5416` | `is not set` |
| `CONFIG_REGULATOR_MP8859` | `is not set` |
| `CONFIG_REGULATOR_MP886X` | `is not set` |
| `CONFIG_REGULATOR_MPQ7920` | `is not set` |
| `CONFIG_REGULATOR_MT6311` | `is not set` |
| `CONFIG_REGULATOR_NETLINK_EVENTS` | `is not set` |
| `CONFIG_REGULATOR_PCA9450` | `is not set` |
| `CONFIG_REGULATOR_PF0900` | `is not set` |
| `CONFIG_REGULATOR_PF530X` | `is not set` |
| `CONFIG_REGULATOR_PF8X00` | `is not set` |
| `CONFIG_REGULATOR_PF9453` | `is not set` |
| `CONFIG_REGULATOR_PFUZE100` | `is not set` |
| `CONFIG_REGULATOR_PV88060` | `is not set` |
| `CONFIG_REGULATOR_PV88080` | `is not set` |
| `CONFIG_REGULATOR_PV88090` | `is not set` |
| `CONFIG_REGULATOR_PWM` | `is not set` |
| `CONFIG_REGULATOR_RAA215300` | `is not set` |
| `CONFIG_REGULATOR_RT4801` | `is not set` |
| `CONFIG_REGULATOR_RT4803` | `is not set` |
| `CONFIG_REGULATOR_RT5133` | `is not set` |
| `CONFIG_REGULATOR_RT5190A` | `is not set` |
| `CONFIG_REGULATOR_RT5739` | `is not set` |
| `CONFIG_REGULATOR_RT5759` | `is not set` |
| `CONFIG_REGULATOR_RT6160` | `is not set` |
| `CONFIG_REGULATOR_RT6190` | `is not set` |
| `CONFIG_REGULATOR_RT6245` | `is not set` |
| `CONFIG_REGULATOR_RTMV20` | `is not set` |
| `CONFIG_REGULATOR_RTQ2134` | `is not set` |
| `CONFIG_REGULATOR_RTQ2208` | `is not set` |
| `CONFIG_REGULATOR_RTQ6752` | `is not set` |
| `CONFIG_REGULATOR_SLG51000` | `is not set` |
| `CONFIG_REGULATOR_SPACEMIT_P1` | `y` |
| `CONFIG_REGULATOR_SUN20I` | `y` |
| `CONFIG_REGULATOR_SY8106A` | `is not set` |
| `CONFIG_REGULATOR_SY8824X` | `is not set` |
| `CONFIG_REGULATOR_SY8827N` | `is not set` |
| `CONFIG_REGULATOR_TPS51632` | `is not set` |
| `CONFIG_REGULATOR_TPS62360` | `is not set` |
| `CONFIG_REGULATOR_TPS6286X` | `is not set` |
| `CONFIG_REGULATOR_TPS6287X` | `is not set` |
| `CONFIG_REGULATOR_TPS65023` | `is not set` |
| `CONFIG_REGULATOR_TPS6507X` | `is not set` |
| `CONFIG_REGULATOR_TPS65132` | `is not set` |
| `CONFIG_REGULATOR_TPS6524X` | `is not set` |
| `CONFIG_REGULATOR_USERSPACE_CONSUMER` | `is not set` |
| `CONFIG_REGULATOR_VCTRL` | `is not set` |
| `CONFIG_REGULATOR_VIRTUAL_CONSUMER` | `is not set` |
| `CONFIG_RELOCATABLE` | `is not set` |
| `CONFIG_RESET_GPIO` | `is not set` |
| `CONFIG_RESET_K230` | `is not set` |
| `CONFIG_RESET_POLARFIRE_SOC` | `y` |
| `CONFIG_RESET_SIMPLE` | `y` |
| `CONFIG_RESET_SPACEMIT` | `y` |
| `CONFIG_RESET_STARFIVE_JH7100` | `y` |
| `CONFIG_RESET_STARFIVE_JH7110` | `y` |
| `CONFIG_RESET_STARFIVE_JH71X0` | `y` |
| `CONFIG_RESET_SUNXI` | `y` |
| `CONFIG_RESET_TH1520` | `is not set` |
| `CONFIG_RESET_TI_SYSCON` | `is not set` |
| `CONFIG_RESET_TI_TPS380X` | `is not set` |
| `CONFIG_RFD77402` | `is not set` |
| `CONFIG_RFD_FTL` | `is not set` |
| `CONFIG_RICHTEK_RTQ6056` | `is not set` |
| `CONFIG_RING_BUFFER` | `y` |
| `CONFIG_RING_BUFFER_BENCHMARK` | `is not set` |
| `CONFIG_RING_BUFFER_STARTUP_TEST` | `is not set` |
| `CONFIG_RING_BUFFER_VALIDATE_TIME_DELTAS` | `is not set` |
| `CONFIG_RISCV_ALTERNATIVE` | `y` |
| `CONFIG_RISCV_ALTERNATIVE_EARLY` | `y` |
| `CONFIG_RISCV_APLIC` | `y` |
| `CONFIG_RISCV_APLIC_MSI` | `y` |
| `CONFIG_RISCV_BOOT_SPINWAIT` | `is not set` |
| `CONFIG_RISCV_COMBO_SPINLOCKS` | `y` |
| `CONFIG_RISCV_DMA_NONCOHERENT` | `y` |
| `CONFIG_RISCV_EMULATED_UNALIGNED_ACCESS` | `is not set` |
| `CONFIG_RISCV_IMSIC` | `y` |
| `CONFIG_RISCV_IOMMU` | `y` |
| `CONFIG_RISCV_IOMMU_PCI` | `y` |
| `CONFIG_RISCV_ISA_FALLBACK` | `y` |
| `CONFIG_RISCV_ISA_SUPM` | `y` |
| `CONFIG_RISCV_ISA_SVNAPOT` | `y` |
| `CONFIG_RISCV_ISA_SVPBMT` | `y` |
| `CONFIG_RISCV_ISA_SVRSW60T59B` | `y` |
| `CONFIG_RISCV_ISA_V` | `y` |
| `CONFIG_RISCV_ISA_VENDOR_EXT` | `y` |
| `CONFIG_RISCV_ISA_VENDOR_EXT_ANDES` | `y` |
| `CONFIG_RISCV_ISA_VENDOR_EXT_MIPS` | `y` |
| `CONFIG_RISCV_ISA_VENDOR_EXT_SIFIVE` | `y` |
| `CONFIG_RISCV_ISA_VENDOR_EXT_THEAD` | `y` |
| `CONFIG_RISCV_ISA_V_DEFAULT_ENABLE` | `y` |
| `CONFIG_RISCV_ISA_V_UCOPY_THRESHOLD` | `768` |
| `CONFIG_RISCV_ISA_XTHEADVECTOR` | `y` |
| `CONFIG_RISCV_ISA_ZACAS` | `y` |
| `CONFIG_RISCV_ISA_ZAWRS` | `y` |
| `CONFIG_RISCV_ISA_ZBA` | `y` |
| `CONFIG_RISCV_ISA_ZBB` | `y` |
| `CONFIG_RISCV_ISA_ZBC` | `y` |
| `CONFIG_RISCV_ISA_ZBKB` | `y` |
| `CONFIG_RISCV_ISA_ZICBOM` | `y` |
| `CONFIG_RISCV_ISA_ZICBOP` | `y` |
| `CONFIG_RISCV_ISA_ZICBOZ` | `y` |
| `CONFIG_RISCV_MISALIGNED` | `y` |
| `CONFIG_RISCV_NONSTANDARD_CACHE_OPS` | `y` |
| `CONFIG_RISCV_PMU` | `y` |
| `CONFIG_RISCV_PMU_LEGACY` | `y` |
| `CONFIG_RISCV_PMU_SBI` | `y` |
| `CONFIG_RISCV_PROBE_UNALIGNED_ACCESS` | `y` |
| `CONFIG_RISCV_PROBE_VECTOR_UNALIGNED_ACCESS` | `y` |
| `CONFIG_RISCV_RPMI_SYSMSI` | `y` |
| `CONFIG_RISCV_SBI_CPUIDLE` | `y` |
| `CONFIG_RISCV_SBI_MPXY_MBOX` | `y` |
| `CONFIG_RISCV_SCALAR_MISALIGNED` | `y` |
| `CONFIG_RISCV_TICKET_SPINLOCKS` | `is not set` |
| `CONFIG_RISCV_VECTOR_MISALIGNED` | `y` |
| `CONFIG_ROHM_BD79112` | `is not set` |
| `CONFIG_ROHM_BD79124` | `is not set` |
| `CONFIG_ROHM_BM1390` | `is not set` |
| `CONFIG_ROHM_BU27034` | `is not set` |
| `CONFIG_RPCSEC_GSS_KRB5` | `m` |
| `CONFIG_RPMB` | `is not set` |
| `CONFIG_RPMSG_CTRL` | `y` |
| `CONFIG_RPMSG_NS` | `y` |
| `CONFIG_RPMSG_QCOM_GLINK_RPM` | `is not set` |
| `CONFIG_RPMSG_TTY` | `is not set` |
| `CONFIG_RPR0521` | `is not set` |
| `CONFIG_RSEQ` | `y` |
| `CONFIG_RTASE` | `is not set` |
| `CONFIG_RTC_DRV_ATCRTC100` | `is not set` |
| `CONFIG_RTC_DRV_MAX31335` | `is not set` |
| `CONFIG_RTC_DRV_NCT3018Y` | `is not set` |
| `CONFIG_RTC_DRV_NVIDIA_VRS10` | `is not set` |
| `CONFIG_RTC_DRV_PL030` | `is not set` |
| `CONFIG_RTC_DRV_PL031` | `is not set` |
| `CONFIG_RTC_DRV_POLARFIRE_SOC` | `is not set` |
| `CONFIG_RTC_DRV_RX8111` | `is not set` |
| `CONFIG_RTC_DRV_SD2405AL` | `is not set` |
| `CONFIG_RTC_DRV_SPACEMIT_P1` | `y` |
| `CONFIG_RTC_DRV_SUN6I` | `y` |
| `CONFIG_RUNTIME_KERNEL_TESTING_MENU` | `y` |
| `CONFIG_RUSTC_LLVM_VERSION` | `0` |
| `CONFIG_RUSTC_SUPPORTS_RISCV` | `y` |
| `CONFIG_RUSTC_VERSION` | `0` |
| `CONFIG_RV` | `is not set` |
| `CONFIG_SATA_DWC` | `is not set` |
| `CONFIG_SATA_ZPODD` | `is not set` |
| `CONFIG_SCA3000` | `is not set` |
| `CONFIG_SCA3300` | `is not set` |
| `CONFIG_SCD30_CORE` | `is not set` |
| `CONFIG_SCD4X` | `is not set` |
| `CONFIG_SCHED_INFO` | `y` |
| `CONFIG_SCHED_MC` | `y` |
| `CONFIG_SCHED_MM_CID` | `y` |
| `CONFIG_SCHED_TRACER` | `is not set` |
| `CONFIG_SCSI_BUSLOGIC` | `is not set` |
| `CONFIG_SCSI_COMMON` | `y` |
| `CONFIG_SCSI_MPI3MR` | `is not set` |
| `CONFIG_SDP500` | `is not set` |
| `CONFIG_SD_ADC_MODULATOR` | `is not set` |
| `CONFIG_SECCOMP_CACHE_DEBUG` | `is not set` |
| `CONFIG_SECRETMEM` | `y` |
| `CONFIG_SECURITY_APPARMOR` | `y` |
| `CONFIG_SECURITY_APPARMOR_DEBUG` | `is not set` |
| `CONFIG_SECURITY_APPARMOR_EXPORT_BINARY` | `y` |
| `CONFIG_SECURITY_APPARMOR_HASH` | `y` |
| `CONFIG_SECURITY_APPARMOR_HASH_DEFAULT` | `y` |
| `CONFIG_SECURITY_APPARMOR_INTROSPECT_POLICY` | `y` |
| `CONFIG_SECURITY_APPARMOR_PARANOID_LOAD` | `y` |
| `CONFIG_SECURITY_IPE` | `is not set` |
| `CONFIG_SECURITY_LANDLOCK` | `is not set` |
| `CONFIG_SECURITY_LOADPIN` | `is not set` |
| `CONFIG_SECURITY_LOCKDOWN_LSM` | `is not set` |
| `CONFIG_SECURITY_NETWORK` | `y` |
| `CONFIG_SECURITY_NETWORK_XFRM` | `is not set` |
| `CONFIG_SECURITY_PATH` | `y` |
| `CONFIG_SECURITY_SAFESETID` | `is not set` |
| `CONFIG_SECURITY_SELINUX` | `y` |
| `CONFIG_SECURITY_SELINUX_AVC_HASH_BITS` | `9` |
| `CONFIG_SECURITY_SELINUX_AVC_STATS` | `y` |
| `CONFIG_SECURITY_SELINUX_BOOTPARAM` | `is not set` |
| `CONFIG_SECURITY_SELINUX_DEBUG` | `is not set` |
| `CONFIG_SECURITY_SELINUX_DEVELOP` | `y` |
| `CONFIG_SECURITY_SELINUX_SID2STR_CACHE_SIZE` | `256` |
| `CONFIG_SECURITY_SELINUX_SIDTAB_HASH_BITS` | `9` |
| `CONFIG_SECURITY_SMACK` | `is not set` |
| `CONFIG_SECURITY_TOMOYO` | `is not set` |
| `CONFIG_SECURITY_YAMA` | `is not set` |
| `CONFIG_SEN0322` | `is not set` |
| `CONFIG_SENSEAIR_SUNRISE_CO2` | `is not set` |
| `CONFIG_SENSIRION_SGP30` | `is not set` |
| `CONFIG_SENSIRION_SGP40` | `is not set` |
| `CONFIG_SENSORS_ACPI_POWER` | `is not set` |
| `CONFIG_SENSORS_AHT10` | `is not set` |
| `CONFIG_SENSORS_AQUACOMPUTER_D5NEXT` | `is not set` |
| `CONFIG_SENSORS_ASUS_ROG_RYUJIN` | `is not set` |
| `CONFIG_SENSORS_CHIPCAP2` | `is not set` |
| `CONFIG_SENSORS_CORSAIR_PSU` | `is not set` |
| `CONFIG_SENSORS_EMC2305` | `is not set` |
| `CONFIG_SENSORS_FTSTEUTATES` | `is not set` |
| `CONFIG_SENSORS_GIGABYTE_WATERFORCE` | `is not set` |
| `CONFIG_SENSORS_GPIO_FAN` | `is not set` |
| `CONFIG_SENSORS_HMC5843_I2C` | `is not set` |
| `CONFIG_SENSORS_HMC5843_SPI` | `is not set` |
| `CONFIG_SENSORS_HS3001` | `is not set` |
| `CONFIG_SENSORS_HTU31` | `is not set` |
| `CONFIG_SENSORS_IIO_HWMON` | `is not set` |
| `CONFIG_SENSORS_INA238` | `is not set` |
| `CONFIG_SENSORS_ISL28022` | `is not set` |
| `CONFIG_SENSORS_ISL29018` | `is not set` |
| `CONFIG_SENSORS_ISL29028` | `is not set` |
| `CONFIG_SENSORS_LTC2991` | `is not set` |
| `CONFIG_SENSORS_LTC2992` | `is not set` |
| `CONFIG_SENSORS_LTC4282` | `is not set` |
| `CONFIG_SENSORS_MAX127` | `is not set` |
| `CONFIG_SENSORS_MAX31760` | `is not set` |
| `CONFIG_SENSORS_MAX6620` | `is not set` |
| `CONFIG_SENSORS_MC34VR500` | `is not set` |
| `CONFIG_SENSORS_NCT6775_I2C` | `is not set` |
| `CONFIG_SENSORS_NCT7363` | `is not set` |
| `CONFIG_SENSORS_NCT7904` | `is not set` |
| `CONFIG_SENSORS_NZXT_KRAKEN2` | `is not set` |
| `CONFIG_SENSORS_NZXT_KRAKEN3` | `is not set` |
| `CONFIG_SENSORS_NZXT_SMART2` | `is not set` |
| `CONFIG_SENSORS_OCC_P8_I2C` | `is not set` |
| `CONFIG_SENSORS_POWERZ` | `is not set` |
| `CONFIG_SENSORS_PT5161L` | `is not set` |
| `CONFIG_SENSORS_PWM_FAN` | `is not set` |
| `CONFIG_SENSORS_RM3100_I2C` | `is not set` |
| `CONFIG_SENSORS_RM3100_SPI` | `is not set` |
| `CONFIG_SENSORS_SBTSI` | `is not set` |
| `CONFIG_SENSORS_SCH5627` | `is not set` |
| `CONFIG_SENSORS_SCH5636` | `is not set` |
| `CONFIG_SENSORS_SFCTEMP` | `m` |
| `CONFIG_SENSORS_SG2042_MCU` | `is not set` |
| `CONFIG_SENSORS_SHT15` | `is not set` |
| `CONFIG_SENSORS_SHT4x` | `is not set` |
| `CONFIG_SENSORS_SPD5118` | `is not set` |
| `CONFIG_SENSORS_TMP464` | `is not set` |
| `CONFIG_SENSORS_TPS23861` | `is not set` |
| `CONFIG_SENSORS_TSC1641` | `is not set` |
| `CONFIG_SENSORS_TSL2563` | `is not set` |
| `CONFIG_SENSORS_XGENE` | `is not set` |
| `CONFIG_SERIAL_8250_DMA` | `y` |
| `CONFIG_SERIAL_8250_DWLIB` | `y` |
| `CONFIG_SERIAL_8250_PCI1XXXX` | `is not set` |
| `CONFIG_SERIAL_8250_PCILIB` | `y` |
| `CONFIG_SERIAL_8250_PERICOM` | `y` |
| `CONFIG_SERIAL_8250_PNP` | `y` |
| `CONFIG_SERIAL_AMBA_PL010` | `is not set` |
| `CONFIG_SERIAL_AMBA_PL011` | `is not set` |
| `CONFIG_SERIAL_ATMEL` | `is not set` |
| `CONFIG_SERIAL_EARLYCON_SEMIHOST` | `is not set` |
| `CONFIG_SERIAL_MCTRL_GPIO` | `y` |
| `CONFIG_SERIO_AMBAKMI` | `is not set` |
| `CONFIG_SERIO_GPIO_PS2` | `is not set` |
| `CONFIG_SERIO_SUN4I_PS2` | `is not set` |
| `CONFIG_SF_PDMA` | `is not set` |
| `CONFIG_SGL_ALLOC` | `y` |
| `CONFIG_SHRINKER_DEBUG` | `is not set` |
| `CONFIG_SI1133` | `is not set` |
| `CONFIG_SI1145` | `is not set` |
| `CONFIG_SI7005` | `is not set` |
| `CONFIG_SI7020` | `is not set` |
| `CONFIG_SI7210` | `is not set` |
| `CONFIG_SIFIVE_CCACHE` | `y` |
| `CONFIG_SKB_EXTENSIONS` | `y` |
| `CONFIG_SLAB_BUCKETS` | `is not set` |
| `CONFIG_SLAB_OBJ_EXT` | `y` |
| `CONFIG_SMB_SERVER` | `is not set` |
| `CONFIG_SMI240` | `is not set` |
| `CONFIG_SMI330_I2C` | `is not set` |
| `CONFIG_SMI330_SPI` | `is not set` |
| `CONFIG_SM_FTL` | `is not set` |
| `CONFIG_SND` | `y` |
| `CONFIG_SND_AD1889` | `is not set` |
| `CONFIG_SND_ALOOP` | `is not set` |
| `CONFIG_SND_AMD_ACP_CONFIG` | `is not set` |
| `CONFIG_SND_ATIIXP` | `is not set` |
| `CONFIG_SND_ATIIXP_MODEM` | `is not set` |
| `CONFIG_SND_AU8810` | `is not set` |
| `CONFIG_SND_AU8820` | `is not set` |
| `CONFIG_SND_AU8830` | `is not set` |
| `CONFIG_SND_AUDIO_GRAPH_CARD` | `is not set` |
| `CONFIG_SND_AUDIO_GRAPH_CARD2` | `is not set` |
| `CONFIG_SND_AW2` | `is not set` |
| `CONFIG_SND_BCD2000` | `is not set` |
| `CONFIG_SND_BCM63XX_I2S_WHISTLER` | `is not set` |
| `CONFIG_SND_BT87X` | `is not set` |
| `CONFIG_SND_CA0106` | `is not set` |
| `CONFIG_SND_CMIPCI` | `is not set` |
| `CONFIG_SND_CS4281` | `is not set` |
| `CONFIG_SND_CS46XX` | `is not set` |
| `CONFIG_SND_CTL_FAST_LOOKUP` | `y` |
| `CONFIG_SND_CTL_INPUT_VALIDATION` | `is not set` |
| `CONFIG_SND_CTXFI` | `is not set` |
| `CONFIG_SND_DARLA20` | `is not set` |
| `CONFIG_SND_DARLA24` | `is not set` |
| `CONFIG_SND_DEBUG` | `is not set` |
| `CONFIG_SND_DESIGNWARE_I2S` | `m` |
| `CONFIG_SND_DESIGNWARE_PCM` | `is not set` |
| `CONFIG_SND_DMAENGINE_PCM` | `y` |
| `CONFIG_SND_DRIVERS` | `y` |
| `CONFIG_SND_DUMMY` | `is not set` |
| `CONFIG_SND_DYNAMIC_MINORS` | `is not set` |
| `CONFIG_SND_ECHO3G` | `is not set` |
| `CONFIG_SND_ENS1370` | `is not set` |
| `CONFIG_SND_ENS1371` | `is not set` |
| `CONFIG_SND_FM801` | `is not set` |
| `CONFIG_SND_GINA20` | `is not set` |
| `CONFIG_SND_GINA24` | `is not set` |
| `CONFIG_SND_HDA_ACPI` | `is not set` |
| `CONFIG_SND_HDA_INTEL` | `is not set` |
| `CONFIG_SND_HDSP` | `is not set` |
| `CONFIG_SND_HDSPM` | `is not set` |
| `CONFIG_SND_HRTIMER` | `is not set` |
| `CONFIG_SND_I2S_HI6210_I2S` | `is not set` |
| `CONFIG_SND_ICE1724` | `is not set` |
| `CONFIG_SND_INDIGO` | `is not set` |
| `CONFIG_SND_INDIGODJ` | `is not set` |
| `CONFIG_SND_INDIGODJX` | `is not set` |
| `CONFIG_SND_INDIGOIO` | `is not set` |
| `CONFIG_SND_INDIGOIOX` | `is not set` |
| `CONFIG_SND_INTEL8X0` | `is not set` |
| `CONFIG_SND_INTEL8X0M` | `is not set` |
| `CONFIG_SND_JACK` | `y` |
| `CONFIG_SND_JACK_INPUT_DEV` | `y` |
| `CONFIG_SND_KORG1212` | `is not set` |
| `CONFIG_SND_LAYLA20` | `is not set` |
| `CONFIG_SND_LAYLA24` | `is not set` |
| `CONFIG_SND_LOLA` | `is not set` |
| `CONFIG_SND_LX6464ES` | `is not set` |
| `CONFIG_SND_MIA` | `is not set` |
| `CONFIG_SND_MIXART` | `is not set` |
| `CONFIG_SND_MONA` | `is not set` |
| `CONFIG_SND_MPU401` | `is not set` |
| `CONFIG_SND_MTPAV` | `is not set` |
| `CONFIG_SND_NM256` | `is not set` |
| `CONFIG_SND_OSSEMUL` | `is not set` |
| `CONFIG_SND_OXYGEN` | `is not set` |
| `CONFIG_SND_PCI` | `y` |
| `CONFIG_SND_PCM` | `y` |
| `CONFIG_SND_PCMTEST` | `is not set` |
| `CONFIG_SND_PCM_TIMER` | `y` |
| `CONFIG_SND_PCXHR` | `is not set` |
| `CONFIG_SND_PROC_FS` | `y` |
| `CONFIG_SND_RIPTIDE` | `is not set` |
| `CONFIG_SND_RME32` | `is not set` |
| `CONFIG_SND_RME96` | `is not set` |
| `CONFIG_SND_RME9652` | `is not set` |
| `CONFIG_SND_SE6X` | `is not set` |
| `CONFIG_SND_SEQUENCER` | `is not set` |
| `CONFIG_SND_SERIAL_U16550` | `is not set` |
| `CONFIG_SND_SIMPLE_CARD` | `m` |
| `CONFIG_SND_SIMPLE_CARD_UTILS` | `m` |
| `CONFIG_SND_SOC` | `y` |
| `CONFIG_SND_SOC_AC97_CODEC` | `is not set` |
| `CONFIG_SND_SOC_ADAU1372_I2C` | `is not set` |
| `CONFIG_SND_SOC_ADAU1372_SPI` | `is not set` |
| `CONFIG_SND_SOC_ADAU1373` | `is not set` |
| `CONFIG_SND_SOC_ADAU1701` | `is not set` |
| `CONFIG_SND_SOC_ADAU1761_I2C` | `is not set` |
| `CONFIG_SND_SOC_ADAU1761_SPI` | `is not set` |
| `CONFIG_SND_SOC_ADAU7002` | `is not set` |
| `CONFIG_SND_SOC_ADAU7118_HW` | `is not set` |
| `CONFIG_SND_SOC_ADAU7118_I2C` | `is not set` |
| `CONFIG_SND_SOC_ADI_AXI_I2S` | `is not set` |
| `CONFIG_SND_SOC_ADI_AXI_SPDIF` | `is not set` |
| `CONFIG_SND_SOC_AK4104` | `is not set` |
| `CONFIG_SND_SOC_AK4118` | `is not set` |
| `CONFIG_SND_SOC_AK4375` | `is not set` |
| `CONFIG_SND_SOC_AK4458` | `is not set` |
| `CONFIG_SND_SOC_AK4554` | `is not set` |
| `CONFIG_SND_SOC_AK4613` | `is not set` |
| `CONFIG_SND_SOC_AK4619` | `is not set` |
| `CONFIG_SND_SOC_AK4642` | `is not set` |
| `CONFIG_SND_SOC_AK5386` | `is not set` |
| `CONFIG_SND_SOC_AK5558` | `is not set` |
| `CONFIG_SND_SOC_ALC5623` | `is not set` |
| `CONFIG_SND_SOC_AMD_ACP` | `is not set` |
| `CONFIG_SND_SOC_AUDIO_IIO_AUX` | `is not set` |
| `CONFIG_SND_SOC_AW8738` | `is not set` |
| `CONFIG_SND_SOC_AW87390` | `is not set` |
| `CONFIG_SND_SOC_AW88081` | `is not set` |
| `CONFIG_SND_SOC_AW88166` | `is not set` |
| `CONFIG_SND_SOC_AW88261` | `is not set` |
| `CONFIG_SND_SOC_AW88395` | `is not set` |
| `CONFIG_SND_SOC_AW88399` | `is not set` |
| `CONFIG_SND_SOC_BD28623` | `is not set` |
| `CONFIG_SND_SOC_BT_SCO` | `is not set` |
| `CONFIG_SND_SOC_CHV3_CODEC` | `is not set` |
| `CONFIG_SND_SOC_CHV3_I2S` | `is not set` |
| `CONFIG_SND_SOC_CS35L32` | `is not set` |
| `CONFIG_SND_SOC_CS35L33` | `is not set` |
| `CONFIG_SND_SOC_CS35L34` | `is not set` |
| `CONFIG_SND_SOC_CS35L35` | `is not set` |
| `CONFIG_SND_SOC_CS35L36` | `is not set` |
| `CONFIG_SND_SOC_CS35L41_I2C` | `is not set` |
| `CONFIG_SND_SOC_CS35L41_SPI` | `is not set` |
| `CONFIG_SND_SOC_CS35L45_I2C` | `is not set` |
| `CONFIG_SND_SOC_CS35L45_SPI` | `is not set` |
| `CONFIG_SND_SOC_CS35L56_I2C` | `is not set` |
| `CONFIG_SND_SOC_CS35L56_SPI` | `is not set` |
| `CONFIG_SND_SOC_CS4234` | `is not set` |
| `CONFIG_SND_SOC_CS4265` | `is not set` |
| `CONFIG_SND_SOC_CS4270` | `is not set` |
| `CONFIG_SND_SOC_CS4271_I2C` | `is not set` |
| `CONFIG_SND_SOC_CS4271_SPI` | `is not set` |
| `CONFIG_SND_SOC_CS42L42` | `is not set` |
| `CONFIG_SND_SOC_CS42L51_I2C` | `is not set` |
| `CONFIG_SND_SOC_CS42L52` | `is not set` |
| `CONFIG_SND_SOC_CS42L56` | `is not set` |
| `CONFIG_SND_SOC_CS42L73` | `is not set` |
| `CONFIG_SND_SOC_CS42L83` | `is not set` |
| `CONFIG_SND_SOC_CS42L84` | `is not set` |
| `CONFIG_SND_SOC_CS42XX8_I2C` | `is not set` |
| `CONFIG_SND_SOC_CS43130` | `is not set` |
| `CONFIG_SND_SOC_CS4341` | `is not set` |
| `CONFIG_SND_SOC_CS4349` | `is not set` |
| `CONFIG_SND_SOC_CS48L32` | `is not set` |
| `CONFIG_SND_SOC_CS530X_I2C` | `is not set` |
| `CONFIG_SND_SOC_CS530X_SPI` | `is not set` |
| `CONFIG_SND_SOC_CS53L30` | `is not set` |
| `CONFIG_SND_SOC_CX2072X` | `is not set` |
| `CONFIG_SND_SOC_DA7213` | `is not set` |
| `CONFIG_SND_SOC_DMIC` | `is not set` |
| `CONFIG_SND_SOC_ES7134` | `is not set` |
| `CONFIG_SND_SOC_ES7241` | `is not set` |
| `CONFIG_SND_SOC_ES8311` | `is not set` |
| `CONFIG_SND_SOC_ES8316` | `is not set` |
| `CONFIG_SND_SOC_ES8323` | `is not set` |
| `CONFIG_SND_SOC_ES8326` | `is not set` |
| `CONFIG_SND_SOC_ES8328_I2C` | `is not set` |
| `CONFIG_SND_SOC_ES8328_SPI` | `is not set` |
| `CONFIG_SND_SOC_ES8375` | `is not set` |
| `CONFIG_SND_SOC_ES8389` | `is not set` |
| `CONFIG_SND_SOC_FS210X` | `is not set` |
| `CONFIG_SND_SOC_FSL_ASRC` | `is not set` |
| `CONFIG_SND_SOC_FSL_AUDMIX` | `is not set` |
| `CONFIG_SND_SOC_FSL_ESAI` | `is not set` |
| `CONFIG_SND_SOC_FSL_MICFIL` | `is not set` |
| `CONFIG_SND_SOC_FSL_RPMSG` | `is not set` |
| `CONFIG_SND_SOC_FSL_SAI` | `is not set` |
| `CONFIG_SND_SOC_FSL_SPDIF` | `is not set` |
| `CONFIG_SND_SOC_FSL_SSI` | `is not set` |
| `CONFIG_SND_SOC_FSL_XCVR` | `is not set` |
| `CONFIG_SND_SOC_GENERIC_DMAENGINE_PCM` | `y` |
| `CONFIG_SND_SOC_GTM601` | `is not set` |
| `CONFIG_SND_SOC_HDA` | `is not set` |
| `CONFIG_SND_SOC_I2C_AND_SPI` | `y` |
| `CONFIG_SND_SOC_ICS43432` | `is not set` |
| `CONFIG_SND_SOC_IDT821034` | `is not set` |
| `CONFIG_SND_SOC_IMX_AUDMUX` | `is not set` |
| `CONFIG_SND_SOC_JH7110_PWMDAC` | `m` |
| `CONFIG_SND_SOC_JH7110_TDM` | `m` |
| `CONFIG_SND_SOC_LPASS_RX_MACRO` | `is not set` |
| `CONFIG_SND_SOC_LPASS_TX_MACRO` | `is not set` |
| `CONFIG_SND_SOC_LPASS_VA_MACRO` | `is not set` |
| `CONFIG_SND_SOC_LPASS_WSA_MACRO` | `is not set` |
| `CONFIG_SND_SOC_MAX9759` | `is not set` |
| `CONFIG_SND_SOC_MAX98088` | `is not set` |
| `CONFIG_SND_SOC_MAX98090` | `is not set` |
| `CONFIG_SND_SOC_MAX98357A` | `is not set` |
| `CONFIG_SND_SOC_MAX98373_I2C` | `is not set` |
| `CONFIG_SND_SOC_MAX98388` | `is not set` |
| `CONFIG_SND_SOC_MAX98390` | `is not set` |
| `CONFIG_SND_SOC_MAX98396` | `is not set` |
| `CONFIG_SND_SOC_MAX98504` | `is not set` |
| `CONFIG_SND_SOC_MAX98520` | `is not set` |
| `CONFIG_SND_SOC_MAX9860` | `is not set` |
| `CONFIG_SND_SOC_MAX9867` | `is not set` |
| `CONFIG_SND_SOC_MAX98927` | `is not set` |
| `CONFIG_SND_SOC_MIKROE_PROTO` | `is not set` |
| `CONFIG_SND_SOC_MSM8916_WCD_DIGITAL` | `is not set` |
| `CONFIG_SND_SOC_MT6351` | `is not set` |
| `CONFIG_SND_SOC_MT6357` | `is not set` |
| `CONFIG_SND_SOC_MT6358` | `is not set` |
| `CONFIG_SND_SOC_MT6660` | `is not set` |
| `CONFIG_SND_SOC_MTK_BTCVSD` | `is not set` |
| `CONFIG_SND_SOC_NAU8315` | `is not set` |
| `CONFIG_SND_SOC_NAU8325` | `is not set` |
| `CONFIG_SND_SOC_NAU8540` | `is not set` |
| `CONFIG_SND_SOC_NAU8810` | `is not set` |
| `CONFIG_SND_SOC_NAU8821` | `is not set` |
| `CONFIG_SND_SOC_NAU8822` | `is not set` |
| `CONFIG_SND_SOC_NAU8824` | `is not set` |
| `CONFIG_SND_SOC_NTP8835` | `is not set` |
| `CONFIG_SND_SOC_NTP8918` | `is not set` |
| `CONFIG_SND_SOC_PCM1681` | `is not set` |
| `CONFIG_SND_SOC_PCM1754` | `is not set` |
| `CONFIG_SND_SOC_PCM1789_I2C` | `is not set` |
| `CONFIG_SND_SOC_PCM179X_I2C` | `is not set` |
| `CONFIG_SND_SOC_PCM179X_SPI` | `is not set` |
| `CONFIG_SND_SOC_PCM186X_I2C` | `is not set` |
| `CONFIG_SND_SOC_PCM186X_SPI` | `is not set` |
| `CONFIG_SND_SOC_PCM3060_I2C` | `is not set` |
| `CONFIG_SND_SOC_PCM3060_SPI` | `is not set` |
| `CONFIG_SND_SOC_PCM3168A_I2C` | `is not set` |
| `CONFIG_SND_SOC_PCM3168A_SPI` | `is not set` |
| `CONFIG_SND_SOC_PCM5102A` | `is not set` |
| `CONFIG_SND_SOC_PCM512x_I2C` | `is not set` |
| `CONFIG_SND_SOC_PCM512x_SPI` | `is not set` |
| `CONFIG_SND_SOC_PCM6240` | `is not set` |
| `CONFIG_SND_SOC_PEB2466` | `is not set` |
| `CONFIG_SND_SOC_RT5616` | `is not set` |
| `CONFIG_SND_SOC_RT5631` | `is not set` |
| `CONFIG_SND_SOC_RT5640` | `is not set` |
| `CONFIG_SND_SOC_RT5659` | `is not set` |
| `CONFIG_SND_SOC_RT9120` | `is not set` |
| `CONFIG_SND_SOC_RT9123` | `is not set` |
| `CONFIG_SND_SOC_RT9123P` | `is not set` |
| `CONFIG_SND_SOC_RTQ9124` | `is not set` |
| `CONFIG_SND_SOC_RTQ9128` | `is not set` |
| `CONFIG_SND_SOC_SDCA_OPTIONAL` | `y` |
| `CONFIG_SND_SOC_SGTL5000` | `is not set` |
| `CONFIG_SND_SOC_SIMPLE_AMPLIFIER` | `is not set` |
| `CONFIG_SND_SOC_SIMPLE_MUX` | `is not set` |
| `CONFIG_SND_SOC_SMA1303` | `is not set` |
| `CONFIG_SND_SOC_SMA1307` | `is not set` |
| `CONFIG_SND_SOC_SOF_TOPLEVEL` | `is not set` |
| `CONFIG_SND_SOC_SPDIF` | `m` |
| `CONFIG_SND_SOC_SRC4XXX_I2C` | `is not set` |
| `CONFIG_SND_SOC_SSM2305` | `is not set` |
| `CONFIG_SND_SOC_SSM2518` | `is not set` |
| `CONFIG_SND_SOC_SSM2602_I2C` | `is not set` |
| `CONFIG_SND_SOC_SSM2602_SPI` | `is not set` |
| `CONFIG_SND_SOC_SSM3515` | `is not set` |
| `CONFIG_SND_SOC_SSM4567` | `is not set` |
| `CONFIG_SND_SOC_STA32X` | `is not set` |
| `CONFIG_SND_SOC_STA350` | `is not set` |
| `CONFIG_SND_SOC_STI_SAS` | `is not set` |
| `CONFIG_SND_SOC_TAS2552` | `is not set` |
| `CONFIG_SND_SOC_TAS2562` | `is not set` |
| `CONFIG_SND_SOC_TAS2764` | `is not set` |
| `CONFIG_SND_SOC_TAS2770` | `is not set` |
| `CONFIG_SND_SOC_TAS2780` | `is not set` |
| `CONFIG_SND_SOC_TAS2781_I2C` | `is not set` |
| `CONFIG_SND_SOC_TAS5086` | `is not set` |
| `CONFIG_SND_SOC_TAS571X` | `is not set` |
| `CONFIG_SND_SOC_TAS5720` | `is not set` |
| `CONFIG_SND_SOC_TAS5805M` | `is not set` |
| `CONFIG_SND_SOC_TAS6424` | `is not set` |
| `CONFIG_SND_SOC_TDA7419` | `is not set` |
| `CONFIG_SND_SOC_TFA9879` | `is not set` |
| `CONFIG_SND_SOC_TFA989X` | `is not set` |
| `CONFIG_SND_SOC_TLV320ADC3XXX` | `is not set` |
| `CONFIG_SND_SOC_TLV320ADCX140` | `is not set` |
| `CONFIG_SND_SOC_TLV320AIC23_I2C` | `is not set` |
| `CONFIG_SND_SOC_TLV320AIC23_SPI` | `is not set` |
| `CONFIG_SND_SOC_TLV320AIC31XX` | `is not set` |
| `CONFIG_SND_SOC_TLV320AIC32X4_I2C` | `is not set` |
| `CONFIG_SND_SOC_TLV320AIC32X4_SPI` | `is not set` |
| `CONFIG_SND_SOC_TLV320AIC3X_I2C` | `is not set` |
| `CONFIG_SND_SOC_TLV320AIC3X_SPI` | `is not set` |
| `CONFIG_SND_SOC_TPA6130A2` | `is not set` |
| `CONFIG_SND_SOC_TS3A227E` | `is not set` |
| `CONFIG_SND_SOC_TSCS42XX` | `is not set` |
| `CONFIG_SND_SOC_TSCS454` | `is not set` |
| `CONFIG_SND_SOC_UDA1334` | `is not set` |
| `CONFIG_SND_SOC_UDA1342` | `is not set` |
| `CONFIG_SND_SOC_WM8510` | `is not set` |
| `CONFIG_SND_SOC_WM8523` | `is not set` |
| `CONFIG_SND_SOC_WM8524` | `is not set` |
| `CONFIG_SND_SOC_WM8580` | `is not set` |
| `CONFIG_SND_SOC_WM8711` | `is not set` |
| `CONFIG_SND_SOC_WM8728` | `is not set` |
| `CONFIG_SND_SOC_WM8731_I2C` | `is not set` |
| `CONFIG_SND_SOC_WM8731_SPI` | `is not set` |
| `CONFIG_SND_SOC_WM8737` | `is not set` |
| `CONFIG_SND_SOC_WM8741` | `is not set` |
| `CONFIG_SND_SOC_WM8750` | `is not set` |
| `CONFIG_SND_SOC_WM8753` | `is not set` |
| `CONFIG_SND_SOC_WM8770` | `is not set` |
| `CONFIG_SND_SOC_WM8776` | `is not set` |
| `CONFIG_SND_SOC_WM8782` | `is not set` |
| `CONFIG_SND_SOC_WM8804_I2C` | `is not set` |
| `CONFIG_SND_SOC_WM8804_SPI` | `is not set` |
| `CONFIG_SND_SOC_WM8903` | `is not set` |
| `CONFIG_SND_SOC_WM8904` | `is not set` |
| `CONFIG_SND_SOC_WM8940` | `is not set` |
| `CONFIG_SND_SOC_WM8960` | `is not set` |
| `CONFIG_SND_SOC_WM8961` | `is not set` |
| `CONFIG_SND_SOC_WM8962` | `is not set` |
| `CONFIG_SND_SOC_WM8974` | `is not set` |
| `CONFIG_SND_SOC_WM8978` | `m` |
| `CONFIG_SND_SOC_WM8985` | `is not set` |
| `CONFIG_SND_SOC_XILINX_AUDIO_FORMATTER` | `is not set` |
| `CONFIG_SND_SOC_XILINX_I2S` | `is not set` |
| `CONFIG_SND_SOC_XILINX_SPDIF` | `is not set` |
| `CONFIG_SND_SOC_XTFPGA_I2S` | `is not set` |
| `CONFIG_SND_SOC_ZL38060` | `is not set` |
| `CONFIG_SND_SPI` | `y` |
| `CONFIG_SND_SUN4I_CODEC` | `is not set` |
| `CONFIG_SND_SUN4I_I2S` | `is not set` |
| `CONFIG_SND_SUN4I_SPDIF` | `is not set` |
| `CONFIG_SND_SUN50I_DMIC` | `is not set` |
| `CONFIG_SND_SUPPORT_OLD_API` | `is not set` |
| `CONFIG_SND_TEST_COMPONENT` | `is not set` |
| `CONFIG_SND_TIMER` | `y` |
| `CONFIG_SND_USB` | `y` |
| `CONFIG_SND_USB_6FIRE` | `is not set` |
| `CONFIG_SND_USB_AUDIO` | `is not set` |
| `CONFIG_SND_USB_CAIAQ` | `is not set` |
| `CONFIG_SND_USB_HIFACE` | `is not set` |
| `CONFIG_SND_USB_POD` | `is not set` |
| `CONFIG_SND_USB_PODHD` | `is not set` |
| `CONFIG_SND_USB_TONEPORT` | `is not set` |
| `CONFIG_SND_USB_UA101` | `is not set` |
| `CONFIG_SND_USB_VARIAX` | `is not set` |
| `CONFIG_SND_UTIMER` | `is not set` |
| `CONFIG_SND_VERBOSE_PROCFS` | `y` |
| `CONFIG_SND_VIA82XX` | `is not set` |
| `CONFIG_SND_VIA82XX_MODEM` | `is not set` |
| `CONFIG_SND_VIRTIO` | `is not set` |
| `CONFIG_SND_VIRTUOSO` | `is not set` |
| `CONFIG_SND_VX222` | `is not set` |
| `CONFIG_SND_YMFPCI` | `is not set` |
| `CONFIG_SOCK_RX_QUEUE_MAPPING` | `y` |
| `CONFIG_SOC_STARFIVE` | `y` |
| `CONFIG_SOFTIRQ_ON_OWN_STACK` | `y` |
| `CONFIG_SOFT_WATCHDOG` | `is not set` |
| `CONFIG_SOPHGO_CV1800B_ADC` | `is not set` |
| `CONFIG_SOPHGO_CV1800B_DMAMUX` | `is not set` |
| `CONFIG_SOPHGO_CV1800_RTCSYS` | `is not set` |
| `CONFIG_SOPHGO_SG2042_MSI` | `is not set` |
| `CONFIG_SOPHGO_SG2044_TOPSYS` | `is not set` |
| `CONFIG_SPACEMIT_CCU` | `y` |
| `CONFIG_SPACEMIT_K1_CCU` | `y` |
| `CONFIG_SPACEMIT_K1_EMAC` | `m` |
| `CONFIG_SPARSEMEM` | `y` |
| `CONFIG_SPARSEMEM_EXTREME` | `y` |
| `CONFIG_SPARSEMEM_VMEMMAP` | `y` |
| `CONFIG_SPI_ATMEL` | `is not set` |
| `CONFIG_SPI_AX88796C` | `is not set` |
| `CONFIG_SPI_CADENCE_QUADSPI` | `m` |
| `CONFIG_SPI_CADENCE_XSPI` | `is not set` |
| `CONFIG_SPI_CH341` | `is not set` |
| `CONFIG_SPI_DYNAMIC` | `y` |
| `CONFIG_SPI_FSL_QUADSPI` | `m` |
| `CONFIG_SPI_GPIO` | `is not set` |
| `CONFIG_SPI_MICROCHIP_CORE_QSPI` | `is not set` |
| `CONFIG_SPI_MICROCHIP_CORE_SPI` | `is not set` |
| `CONFIG_SPI_OC_TINY` | `is not set` |
| `CONFIG_SPI_PCI1XXXX` | `is not set` |
| `CONFIG_SPI_PL022` | `m` |
| `CONFIG_SPI_POLARFIRE_SOC` | `is not set` |
| `CONFIG_SPI_SG2044_NOR` | `is not set` |
| `CONFIG_SPI_SN_F_OSPI` | `is not set` |
| `CONFIG_SPI_SUN4I` | `is not set` |
| `CONFIG_SPI_SUN6I` | `y` |
| `CONFIG_SPI_VIRTIO` | `is not set` |
| `CONFIG_SPLIT_PMD_PTLOCKS` | `y` |
| `CONFIG_SPLIT_PTE_PTLOCKS` | `y` |
| `CONFIG_SPS30_I2C` | `is not set` |
| `CONFIG_SRF04` | `is not set` |
| `CONFIG_SRF08` | `is not set` |
| `CONFIG_SSFDC` | `is not set` |
| `CONFIG_STACKDEPOT` | `y` |
| `CONFIG_STACKDEPOT_MAX_FRAMES` | `64` |
| `CONFIG_STACKPROTECTOR_PER_TASK` | `y` |
| `CONFIG_STACKTRACE_BUILD_ID` | `is not set` |
| `CONFIG_STACK_TRACER` | `is not set` |
| `CONFIG_STARFIVE_JH8100_INTC` | `y` |
| `CONFIG_STARFIVE_STARLINK_CACHE` | `is not set` |
| `CONFIG_STARFIVE_STARLINK_PMU` | `is not set` |
| `CONFIG_STARFIVE_WATCHDOG` | `y` |
| `CONFIG_STK3310` | `is not set` |
| `CONFIG_STK8312` | `is not set` |
| `CONFIG_STK8BA50` | `is not set` |
| `CONFIG_STMMAC_PCI` | `is not set` |
| `CONFIG_STMMAC_PLATFORM` | `m` |
| `CONFIG_STMMAC_SELFTESTS` | `is not set` |
| `CONFIG_STP` | `m` |
| `CONFIG_STRICT_DEVMEM` | `is not set` |
| `CONFIG_STRICT_MODULE_RWX` | `y` |
| `CONFIG_ST_UVIS25` | `is not set` |
| `CONFIG_SUN20I_D1_CCU` | `y` |
| `CONFIG_SUN20I_D1_R_CCU` | `y` |
| `CONFIG_SUN20I_GPADC` | `is not set` |
| `CONFIG_SUN20I_PPU` | `y` |
| `CONFIG_SUN4I_EMAC` | `is not set` |
| `CONFIG_SUN4I_TIMER` | `y` |
| `CONFIG_SUN50I_DE2_BUS` | `is not set` |
| `CONFIG_SUN50I_H6_PRCM_PPU` | `is not set` |
| `CONFIG_SUN50I_IOMMU` | `y` |
| `CONFIG_SUN55I_PCK600` | `y` |
| `CONFIG_SUN6I_MSGBOX` | `y` |
| `CONFIG_SUN6I_RTC_CCU` | `y` |
| `CONFIG_SUN8I_DE2_CCU` | `m` |
| `CONFIG_SUN8I_THERMAL` | `is not set` |
| `CONFIG_SUNXI_CCU` | `y` |
| `CONFIG_SUNXI_RSB` | `is not set` |
| `CONFIG_SUNXI_SRAM` | `y` |
| `CONFIG_SUNXI_WATCHDOG` | `y` |
| `CONFIG_SUSPEND` | `y` |
| `CONFIG_SUSPEND_FREEZER` | `y` |
| `CONFIG_SWIOTLB_DYNAMIC` | `is not set` |
| `CONFIG_SX9310` | `is not set` |
| `CONFIG_SX9324` | `is not set` |
| `CONFIG_SX9360` | `is not set` |
| `CONFIG_SX9500` | `is not set` |
| `CONFIG_SYNTH_EVENTS` | `is not set` |
| `CONFIG_SYSCTL_ARCH_UNALIGN_ALLOW` | `y` |
| `CONFIG_SYSFB_SIMPLEFB` | `is not set` |
| `CONFIG_SYSVIPC_COMPAT` | `y` |
| `CONFIG_T5403` | `is not set` |
| `CONFIG_TCP_AO` | `is not set` |
| `CONFIG_TCS3414` | `is not set` |
| `CONFIG_TCS3472` | `is not set` |
| `CONFIG_TEHUTI_TN40` | `is not set` |
| `CONFIG_TH1520_AON_PROTOCOL` | `is not set` |
| `CONFIG_THEAD_TH1520_MBOX` | `is not set` |
| `CONFIG_THERMAL_CORE_TESTING` | `is not set` |
| `CONFIG_THERMAL_DEBUGFS` | `is not set` |
| `CONFIG_THERMAL_DEFAULT_GOV_FAIR_SHARE` | `is not set` |
| `CONFIG_THERMAL_DEFAULT_GOV_STEP_WISE` | `y` |
| `CONFIG_THERMAL_DEFAULT_GOV_USER_SPACE` | `is not set` |
| `CONFIG_THERMAL_EMERGENCY_POWEROFF_DELAY_MS` | `0` |
| `CONFIG_THERMAL_EMULATION` | `is not set` |
| `CONFIG_THERMAL_GOV_BANG_BANG` | `is not set` |
| `CONFIG_THERMAL_GOV_FAIR_SHARE` | `is not set` |
| `CONFIG_THERMAL_GOV_STEP_WISE` | `y` |
| `CONFIG_THERMAL_GOV_USER_SPACE` | `is not set` |
| `CONFIG_THERMAL_HWMON` | `y` |
| `CONFIG_THERMAL_MMIO` | `is not set` |
| `CONFIG_THERMAL_NETLINK` | `is not set` |
| `CONFIG_THERMAL_OF` | `y` |
| `CONFIG_THERMAL_STATISTICS` | `is not set` |
| `CONFIG_THREAD_SIZE_ORDER` | `2` |
| `CONFIG_TIMERLAT_TRACER` | `is not set` |
| `CONFIG_TIME_NS` | `y` |
| `CONFIG_TINYDRM_ILI9163` | `is not set` |
| `CONFIG_TINYDRM_SHARP_MEMORY` | `is not set` |
| `CONFIG_TI_ADC081C` | `is not set` |
| `CONFIG_TI_ADC0832` | `is not set` |
| `CONFIG_TI_ADC084S021` | `is not set` |
| `CONFIG_TI_ADC108S102` | `is not set` |
| `CONFIG_TI_ADC12138` | `is not set` |
| `CONFIG_TI_ADC128S052` | `is not set` |
| `CONFIG_TI_ADC161S626` | `is not set` |
| `CONFIG_TI_ADS1015` | `is not set` |
| `CONFIG_TI_ADS1100` | `is not set` |
| `CONFIG_TI_ADS1119` | `is not set` |
| `CONFIG_TI_ADS124S08` | `is not set` |
| `CONFIG_TI_ADS1298` | `is not set` |
| `CONFIG_TI_ADS131E08` | `is not set` |
| `CONFIG_TI_ADS7138` | `is not set` |
| `CONFIG_TI_ADS7924` | `is not set` |
| `CONFIG_TI_ADS7950` | `is not set` |
| `CONFIG_TI_ADS8344` | `is not set` |
| `CONFIG_TI_ADS8688` | `is not set` |
| `CONFIG_TI_DAC082S085` | `is not set` |
| `CONFIG_TI_DAC5571` | `is not set` |
| `CONFIG_TI_DAC7311` | `is not set` |
| `CONFIG_TI_DAC7612` | `is not set` |
| `CONFIG_TI_FPC202` | `is not set` |
| `CONFIG_TI_LMP92064` | `is not set` |
| `CONFIG_TI_TLC4541` | `is not set` |
| `CONFIG_TI_TMAG5273` | `is not set` |
| `CONFIG_TI_TSC2046` | `is not set` |
| `CONFIG_TMP006` | `is not set` |
| `CONFIG_TMP007` | `is not set` |
| `CONFIG_TMP117` | `is not set` |
| `CONFIG_TMPFS_QUOTA` | `is not set` |
| `CONFIG_TOOLCHAIN_HAS_V` | `y` |
| `CONFIG_TOOLCHAIN_HAS_VECTOR_CRYPTO` | `y` |
| `CONFIG_TOOLCHAIN_HAS_ZBA` | `y` |
| `CONFIG_TOOLCHAIN_HAS_ZBB` | `y` |
| `CONFIG_TOOLCHAIN_HAS_ZBC` | `y` |
| `CONFIG_TOOLCHAIN_HAS_ZBKB` | `y` |
| `CONFIG_TOOLCHAIN_NEEDS_EXPLICIT_ZICSR_ZIFENCEI` | `y` |
| `CONFIG_TPL0102` | `is not set` |
| `CONFIG_TPS65010` | `is not set` |
| `CONFIG_TRACEFS_AUTOMOUNT_DEPRECATED` | `y` |
| `CONFIG_TRACEPOINTS` | `y` |
| `CONFIG_TRACEPOINT_BENCHMARK` | `is not set` |
| `CONFIG_TRACER_SNAPSHOT` | `is not set` |
| `CONFIG_TRACE_CLOCK` | `y` |
| `CONFIG_TRACE_EVAL_MAP_FILE` | `is not set` |
| `CONFIG_TRACE_EVENT_INJECT` | `is not set` |
| `CONFIG_TRACE_GPU_MEM` | `is not set` |
| `CONFIG_TRACING` | `y` |
| `CONFIG_TRANSPARENT_HUGEPAGE` | `is not set` |
| `CONFIG_TRUSTED_KEYS` | `is not set` |
| `CONFIG_TSL2583` | `is not set` |
| `CONFIG_TSL2591` | `is not set` |
| `CONFIG_TSL2772` | `is not set` |
| `CONFIG_TSL4531` | `is not set` |
| `CONFIG_TSNEP` | `is not set` |
| `CONFIG_TSYS01` | `is not set` |
| `CONFIG_TSYS02D` | `is not set` |
| `CONFIG_TXGBE` | `is not set` |
| `CONFIG_TXGBEVF` | `is not set` |
| `CONFIG_UACCE` | `is not set` |
| `CONFIG_UNION_FIND` | `y` |
| `CONFIG_UPROBES` | `y` |
| `CONFIG_UPROBE_EVENTS` | `y` |
| `CONFIG_US5182D` | `is not set` |
| `CONFIG_USBPCWATCHDOG` | `is not set` |
| `CONFIG_USB_AMD5536UDC` | `is not set` |
| `CONFIG_USB_AUDIO` | `is not set` |
| `CONFIG_USB_BDC_UDC` | `is not set` |
| `CONFIG_USB_CDC_COMPOSITE` | `is not set` |
| `CONFIG_USB_CDNS2_UDC` | `is not set` |
| `CONFIG_USB_CDNS3_GADGET` | `y` |
| `CONFIG_USB_CDNS3_HOST` | `y` |
| `CONFIG_USB_CDNS3_PCI_WRAP` | `m` |
| `CONFIG_USB_CDNS3_STARFIVE` | `m` |
| `CONFIG_USB_CDNSP_PCI` | `is not set` |
| `CONFIG_USB_CDNS_HOST` | `y` |
| `CONFIG_USB_CDNS_SUPPORT` | `m` |
| `CONFIG_USB_CONFIGFS` | `m` |
| `CONFIG_USB_CONFIGFS_ACM` | `y` |
| `CONFIG_USB_CONFIGFS_ECM` | `y` |
| `CONFIG_USB_CONFIGFS_ECM_SUBSET` | `y` |
| `CONFIG_USB_CONFIGFS_EEM` | `y` |
| `CONFIG_USB_CONFIGFS_F_FS` | `y` |
| `CONFIG_USB_CONFIGFS_F_HID` | `is not set` |
| `CONFIG_USB_CONFIGFS_F_LB_SS` | `is not set` |
| `CONFIG_USB_CONFIGFS_F_MIDI` | `is not set` |
| `CONFIG_USB_CONFIGFS_F_MIDI2` | `is not set` |
| `CONFIG_USB_CONFIGFS_F_PRINTER` | `is not set` |
| `CONFIG_USB_CONFIGFS_F_UAC1` | `is not set` |
| `CONFIG_USB_CONFIGFS_F_UAC1_LEGACY` | `is not set` |
| `CONFIG_USB_CONFIGFS_F_UAC2` | `is not set` |
| `CONFIG_USB_CONFIGFS_F_UVC` | `is not set` |
| `CONFIG_USB_CONFIGFS_MASS_STORAGE` | `y` |
| `CONFIG_USB_CONFIGFS_NCM` | `y` |
| `CONFIG_USB_CONFIGFS_OBEX` | `y` |
| `CONFIG_USB_CONFIGFS_RNDIS` | `y` |
| `CONFIG_USB_CONFIGFS_SERIAL` | `y` |
| `CONFIG_USB_CONN_GPIO` | `is not set` |
| `CONFIG_USB_DEFAULT_AUTHORIZATION_MODE` | `1` |
| `CONFIG_USB_DUMMY_HCD` | `is not set` |
| `CONFIG_USB_EG20T` | `is not set` |
| `CONFIG_USB_ETH` | `is not set` |
| `CONFIG_USB_FUNCTIONFS` | `is not set` |
| `CONFIG_USB_F_ACM` | `m` |
| `CONFIG_USB_F_ECM` | `m` |
| `CONFIG_USB_F_EEM` | `m` |
| `CONFIG_USB_F_FS` | `m` |
| `CONFIG_USB_F_MASS_STORAGE` | `m` |
| `CONFIG_USB_F_NCM` | `m` |
| `CONFIG_USB_F_OBEX` | `m` |
| `CONFIG_USB_F_RNDIS` | `m` |
| `CONFIG_USB_F_SERIAL` | `m` |
| `CONFIG_USB_F_SUBSET` | `m` |
| `CONFIG_USB_GADGETFS` | `is not set` |
| `CONFIG_USB_GADGET_DEBUG` | `is not set` |
| `CONFIG_USB_GADGET_DEBUG_FILES` | `is not set` |
| `CONFIG_USB_GADGET_DEBUG_FS` | `is not set` |
| `CONFIG_USB_GADGET_STORAGE_NUM_BUFFERS` | `2` |
| `CONFIG_USB_GADGET_VBUS_DRAW` | `2` |
| `CONFIG_USB_GADGET_XILINX` | `is not set` |
| `CONFIG_USB_GOKU` | `is not set` |
| `CONFIG_USB_GPIO_VBUS` | `is not set` |
| `CONFIG_USB_GR_UDC` | `is not set` |
| `CONFIG_USB_G_ACM_MS` | `is not set` |
| `CONFIG_USB_G_DBGP` | `is not set` |
| `CONFIG_USB_G_HID` | `is not set` |
| `CONFIG_USB_G_MULTI` | `is not set` |
| `CONFIG_USB_G_NCM` | `is not set` |
| `CONFIG_USB_G_PRINTER` | `is not set` |
| `CONFIG_USB_G_SERIAL` | `is not set` |
| `CONFIG_USB_G_WEBCAM` | `is not set` |
| `CONFIG_USB_LIBCOMPOSITE` | `m` |
| `CONFIG_USB_LJCA` | `is not set` |
| `CONFIG_USB_M66592` | `is not set` |
| `CONFIG_USB_MASS_STORAGE` | `is not set` |
| `CONFIG_USB_MAX3420_UDC` | `is not set` |
| `CONFIG_USB_MIDI_GADGET` | `is not set` |
| `CONFIG_USB_MUSB_DUAL_ROLE` | `y` |
| `CONFIG_USB_MUSB_GADGET` | `is not set` |
| `CONFIG_USB_MUSB_HOST` | `is not set` |
| `CONFIG_USB_MUSB_POLARFIRE_SOC` | `is not set` |
| `CONFIG_USB_MUSB_SUNXI` | `m` |
| `CONFIG_USB_NET2280` | `is not set` |
| `CONFIG_USB_ONBOARD_DEV` | `is not set` |
| `CONFIG_USB_OTG` | `y` |
| `CONFIG_USB_OTG_FSM` | `is not set` |
| `CONFIG_USB_PCI_AMD` | `is not set` |
| `CONFIG_USB_PHY` | `y` |
| `CONFIG_USB_PXA27X` | `is not set` |
| `CONFIG_USB_R8A66597` | `is not set` |
| `CONFIG_USB_RAW_GADGET` | `is not set` |
| `CONFIG_USB_SNP_UDC_PLAT` | `is not set` |
| `CONFIG_USB_U_ETHER` | `m` |
| `CONFIG_USB_U_SERIAL` | `m` |
| `CONFIG_USB_XHCI_SIDEBAND` | `is not set` |
| `CONFIG_USB_ZERO` | `is not set` |
| `CONFIG_USER_EVENTS` | `is not set` |
| `CONFIG_USER_STACKTRACE_SUPPORT` | `y` |
| `CONFIG_U_SERIAL_CONSOLE` | `is not set` |
| `CONFIG_V4L2_ASYNC` | `m` |
| `CONFIG_V4L2_FWNODE` | `m` |
| `CONFIG_V4L_MEM2MEM_DRIVERS` | `is not set` |
| `CONFIG_V4L_PLATFORM_DRIVERS` | `is not set` |
| `CONFIG_VCAP` | `is not set` |
| `CONFIG_VCNL3020` | `is not set` |
| `CONFIG_VCNL4000` | `is not set` |
| `CONFIG_VCNL4035` | `is not set` |
| `CONFIG_VCPU_STALL_DETECTOR` | `is not set` |
| `CONFIG_VDSO_GETRANDOM` | `y` |
| `CONFIG_VEML3235` | `is not set` |
| `CONFIG_VEML6030` | `is not set` |
| `CONFIG_VEML6040` | `is not set` |
| `CONFIG_VEML6046X00` | `is not set` |
| `CONFIG_VEML6070` | `is not set` |
| `CONFIG_VEML6075` | `is not set` |
| `CONFIG_VF610_ADC` | `is not set` |
| `CONFIG_VF610_DAC` | `is not set` |
| `CONFIG_VFIO` | `is not set` |
| `CONFIG_VHOST_ENABLE_FORK_OWNER_CONTROL` | `y` |
| `CONFIG_VIDEO` | `y` |
| `CONFIG_VIDEOMODE_HELPERS` | `y` |
| `CONFIG_VIDEO_ADV_DEBUG` | `is not set` |
| `CONFIG_VIDEO_CADENCE_CSI2RX` | `m` |
| `CONFIG_VIDEO_CADENCE_CSI2TX` | `is not set` |
| `CONFIG_VIDEO_DEV` | `m` |
| `CONFIG_VIDEO_DS90UB913` | `is not set` |
| `CONFIG_VIDEO_DS90UB953` | `is not set` |
| `CONFIG_VIDEO_DS90UB960` | `is not set` |
| `CONFIG_VIDEO_FIXED_MINOR_RANGES` | `is not set` |
| `CONFIG_VIDEO_GS1662` | `is not set` |
| `CONFIG_VIDEO_MAX96714` | `is not set` |
| `CONFIG_VIDEO_MAX96717` | `is not set` |
| `CONFIG_VIDEO_RP1_CFE` | `is not set` |
| `CONFIG_VIDEO_V4L2_I2C` | `y` |
| `CONFIG_VIDEO_V4L2_SUBDEV_API` | `y` |
| `CONFIG_VIRTIO_ANCHOR` | `y` |
| `CONFIG_VIRTIO_DEBUG` | `is not set` |
| `CONFIG_VIRTIO_PCI_LIB` | `y` |
| `CONFIG_VIRTIO_PCI_LIB_LEGACY` | `y` |
| `CONFIG_VIRTIO_PMEM` | `is not set` |
| `CONFIG_VIRTIO_RTC` | `is not set` |
| `CONFIG_VIRTUALIZATION` | `y` |
| `CONFIG_VIRT_XFER_TO_GUEST_WORK` | `y` |
| `CONFIG_VL53L0X_I2C` | `is not set` |
| `CONFIG_VL6180` | `is not set` |
| `CONFIG_VLAN_8021Q_GVRP` | `is not set` |
| `CONFIG_VLAN_8021Q_MVRP` | `is not set` |
| `CONFIG_VMAP_STACK` | `y` |
| `CONFIG_VT_CONSOLE_SLEEP` | `y` |
| `CONFIG_VZ89X` | `is not set` |
| `CONFIG_WATCHDOG_CORE` | `y` |
| `CONFIG_WATCHDOG_HANDLE_BOOT_ENABLED` | `y` |
| `CONFIG_WATCHDOG_HRTIMER_PRETIMEOUT` | `is not set` |
| `CONFIG_WATCHDOG_NOWAYOUT` | `is not set` |
| `CONFIG_WATCHDOG_OPEN_TIMEOUT` | `0` |
| `CONFIG_WATCHDOG_PRETIMEOUT_GOV` | `is not set` |
| `CONFIG_WATCHDOG_SYSFS` | `is not set` |
| `CONFIG_WDAT_WDT` | `is not set` |
| `CONFIG_WDTPCI` | `is not set` |
| `CONFIG_WERROR` | `is not set` |
| `CONFIG_WLAN_VENDOR_PURELIFI` | `y` |
| `CONFIG_WLAN_VENDOR_SILABS` | `y` |
| `CONFIG_WPCM450_SOC` | `is not set` |
| `CONFIG_WQ_CPU_INTENSIVE_REPORT` | `is not set` |
| `CONFIG_WQ_POWER_EFFICIENT_DEFAULT` | `is not set` |
| `CONFIG_WWAN` | `is not set` |
| `CONFIG_X9250` | `is not set` |
| `CONFIG_XARRAY_MULTI` | `y` |
| `CONFIG_XFRM` | `y` |
| `CONFIG_XFRM_ALGO` | `m` |
| `CONFIG_XFRM_ESP` | `m` |
| `CONFIG_XFRM_INTERFACE` | `is not set` |
| `CONFIG_XFRM_IPTFS` | `is not set` |
| `CONFIG_XFRM_MIGRATE` | `is not set` |
| `CONFIG_XFRM_STATISTICS` | `is not set` |
| `CONFIG_XFRM_SUB_POLICY` | `is not set` |
| `CONFIG_XILINX_DMA` | `is not set` |
| `CONFIG_XILINX_EMACLITE` | `is not set` |
| `CONFIG_XILINX_INTC` | `is not set` |
| `CONFIG_XILINX_WATCHDOG` | `is not set` |
| `CONFIG_XILINX_XADC` | `is not set` |
| `CONFIG_XILINX_XDMA` | `is not set` |
| `CONFIG_XILINX_ZYNQMP_DPDMA` | `is not set` |
| `CONFIG_XILLYUSB` | `is not set` |
| `CONFIG_XOR_BLOCKS` | `m` |
| `CONFIG_XZ_DEC_ARM64` | `y` |
| `CONFIG_XZ_DEC_MICROLZMA` | `is not set` |
| `CONFIG_XZ_DEC_RISCV` | `y` |
| `CONFIG_YAMAHA_YAS530` | `is not set` |
| `CONFIG_ZERO_CALL_USED_REGS` | `is not set` |
| `CONFIG_ZIIRAVE_WATCHDOG` | `is not set` |
| `CONFIG_ZISOFS` | `y` |
| `CONFIG_ZL3073X_I2C` | `is not set` |
| `CONFIG_ZL3073X_SPI` | `is not set` |
| `CONFIG_ZLIB_DEFLATE` | `m` |
| `CONFIG_ZOPT2201` | `is not set` |
| `CONFIG_ZPA2326` | `is not set` |
| `CONFIG_ZRAM` | `is not set` |
| `CONFIG_ZSTD_COMMON` | `y` |
| `CONFIG_ZSTD_COMPRESS` | `y` |
| `CONFIG_ZSWAP` | `is not set` |

