# Quard Star AMP IPC Contract (Linux 6.10 + FreeRTOS)

## 1) Shared Memory Layout (Frozen)

| Region | PA Start | Size | PA End | Notes |
|---|---:|---:|---:|---|
| VRING0 | `0xBF700000` | `0x2000` (8KB) | `0xBF701FFF` | FreeRTOS TX ring / Linux RX ring |
| VRING1 | `0xBF702000` | `0x2000` (8KB) | `0xBF703FFF` | FreeRTOS RX ring / Linux TX ring |
| RPMsg Buffers | `0xBF704000` | `0x8000` (32KB) | `0xBF70BFFF` | Payload buffers |
| Resource Table | `0xBF70C000` | `0x1000` (4KB) | `0xBF70CFFF` | remoteproc attach contract |

Reserved memory in DTS stays:

- `amp_shm: amp_shared_region@bf700000 { reg = <0x0 0xbf700000 0x0 0x00020000>; }`

Resource Table placement:

- The FreeRTOS ELF defines `.resource_table` at `0xBF70C000`
- Build extracts `resource_table.bin` and packs it into `fw.bin` at `0x20300000`
- LowLevelBoot copies 4KB from `0x20300000` to `0xBF70C000`
- No runtime `memcpy` is used; the table exists immediately at boot

## 2) Resource Table ABI (Linux-Compatible)

Source of truth:

- Linux: `linux/include/linux/remoteproc.h`
- FreeRTOS: `trusted_domain/bsp_include/resource_table.h`

Frozen structure order:

1. `resource_table` header (`ver/num/reserved/offset[0]`)
2. `fw_rsc_hdr` (`type = RSC_VDEV`)
3. `fw_rsc_vdev`
4. `fw_rsc_vdev_vring` x2 (**with `pa` field**)

Current packed layout (`struct shared_resource_table`):

| Field | Offset | Size |
|---|---:|---:|
| `ver` | 0 | 4 |
| `num` | 4 | 4 |
| `reserved[2]` | 8 | 8 |
| `offset[0]` | 16 | 4 |
| `hdr` | 20 | 4 |
| `vdev` | 24 | 24 |
| `vring0` | 48 | 20 |
| `vring1` | 68 | 20 |
| **Total** |  | **88** |

Runtime rules:

- `notifyid` is owned by Linux remoteproc and can be rewritten during attach.
- FreeRTOS only reads `vdev.status`; it does not force host-owned status bits.
- `VIRTIO_CONFIG_S_DRIVER_OK` gates FreeRTOS RPMsg init.
- `pa` is set equal to `da` in this no-IOMMU system for clarity.

### Resource Table Explained (Simple Version)

Resource Table is the “device manual” FreeRTOS writes for Linux. In attach-only
mode, Linux reads it to know:

- Which virtio device exists (RPMsg)
- How many vrings exist
- Where each vring lives in physical memory
- Which notifyid values Linux will later write back

Key terms:

- `fw_rsc_hdr`: the resource entry header, tells Linux the type (VDEV here)
- `fw_rsc_vdev`: the virtio device header (RPMsg)
- `fw_rsc_vdev_vring`: per‑vring descriptor (address, align, size, notifyid)

Why `VRING_SIZE=128`:

Linux computes vring memory by `size = PAGE_ALIGN(vring_size(num, align))`.
With `align=4096`, `num=256` requires ~12KB, but our fixed window is 8KB.
So we must use `num=128` to fit.

Common errors:

- `Registered carveout doesn't fit len request`: vring size > fixed window
- `virtio_rpmsg_bus ... -12`: buffer pool allocation failed
- `DRIVER_OK` not set: virtio/rpmsg init failed on Linux

## 3) Mailbox Doorbell Direction Contract (Frozen)

Mailbox base: `0x10004000`

| Register | Offset | Meaning |
|---|---:|---|
| `REG_LINUX_TRIG` | `0x00` | Trigger IRQ to Linux |
| `REG_LINUX_ACK` | `0x04` | ACK/Clear Linux IRQ |
| `REG_LINUX_IE` | `0x0C` | Linux IRQ enable |
| `REG_RTOS_TRIG` | `0x20` | Trigger IRQ to FreeRTOS |
| `REG_RTOS_ACK` | `0x24` | ACK/Clear FreeRTOS IRQ |
| `REG_RTOS_IE` | `0x2C` | FreeRTOS IRQ enable |

Direction matrix:

- FreeRTOS -> Linux kick: write `REG_LINUX_TRIG`
- Linux -> FreeRTOS kick: write `REG_RTOS_TRIG`
- Linux ISR clear: write `REG_LINUX_ACK`
- FreeRTOS ISR clear (if enabled later): write `REG_RTOS_ACK`

## 4) Attach/Runtime State Machine

1. FreeRTOS boots and writes Resource Table at `0xBF70C000`.
2. Linux loads `quard_star_rproc.ko`, device state is `RPROC_DETACHED`.
3. User performs manual attach (`/sys/class/remoteproc/remoteprocX/state` -> `start`).
4. remoteproc parses table and creates rproc-virtio.
5. Linux virtio/rpmsg sets `vdev.status` (eventually contains `DRIVER_OK`).
6. FreeRTOS detects `DRIVER_OK`, then initializes `simple_rpmsg`.
7. RPMsg traffic uses avail/used rings + mailbox doorbells.

### Linux Attach (What Actually Happens)

1. Linux maps Resource Table from shared memory.
2. `rproc_alloc_vring()` checks vring carveout size and alignment.
3. `rproc_add_virtio_dev()` registers the virtio device.
4. `virtio_rpmsg_bus` probes and allocates buffers.
5. Linux writes `DRIVER_OK` into `vdev.status`.

## 5) Build/Smoke Validation Commands

### Build

```bash
./build.sh freertos rebuild
./build.sh driver rebuild
```

### Linux runtime checks

```bash
insmod /driver/quard_star_rproc.ko
insmod /driver/mailbox_test.ko
ls /sys/class/remoteproc/
cat /sys/class/remoteproc/remoteproc0/state
echo start > /sys/class/remoteproc/remoteproc0/state   # attach path
```

### Expected signs

- No resource-table parse errors in `dmesg`.
- Mailbox IRQ handler executes and ACK is visible.
- FreeRTOS log reaches `Linux attached! vdev.status = DRIVER_OK`.
- Endpoint NS announce is sent after `DRIVER_OK`.
