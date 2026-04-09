# Quard Star Remoteproc / RPMsg / Mailbox 关系图（思维导图风格）

下面使用 Mermaid 画出结构体与调用关系。你可以在支持 Mermaid 的编辑器中直接渲染。

```mermaid
flowchart TD
    %% ===== 核心初始化（与平台 probe 解耦） =====
    subgraph I["remoteproc core 初始化"]
      I1["remoteproc_init(): 注册 class/sysfs/debugfs/cdev/崩溃处理"]
    end

    %% ===== 平台设备/SoC 驱动层 =====
    subgraph P["平台设备驱动 / SoC rproc driver"]
      P0["平台总线 + DT 匹配"] --> P1["platform_driver probe()"]
      P1 --> P2["rproc_alloc / rproc_add"]
      P2 --> P3["提供 rproc_ops 回调(驱动实现)"]
      P4["IRQ handler -> rproc_vq_interrupt() [driver->core]"]
    end

    %% ===== remoteproc 核心层 =====
    subgraph C["remoteproc 核心框架(控制路径)"]
      C1["sysfs: echo start"]
      C2["rproc_boot() [core]"]
      C3{"state == DETACHED ?"}
    end

    %% ===== Attach 细化 =====
    subgraph A["Attach 细化(核心+回调)"]
      A0["rproc_attach() [core]"]
      A1["rproc_prepare_device() -> ops->prepare() [callback]"]
      A2["rproc_set_rsc_table() -> ops->get_loaded_rsc_table() [callback]"]
      A3["rproc_handle_resources() [core]"]
      A4["创建 rproc-virtio [core]"]
      A5["rproc_alloc_registered_carveouts() [core]"]
      A6["ops->attach() [callback]"]
      A7["state=ATTACHED [core]"]
    end

    %% ===== Boot 细化 =====
    subgraph B["Boot 细化(核心+回调)"]
      B0["rproc_fw_boot() [core]"]
      B1["request_firmware() [core]"]
      B2["rproc_parse_fw() -> ops->parse_fw() [callback]"]
      B3["rproc_load_segments() -> ops->load() [callback]"]
      B4["rproc_handle_resources() [core]"]
      B5["rproc_alloc_registered_carveouts() [core]"]
      B6["rproc_start() -> ops->start() [callback]"]
      B7["state=RUNNING [core]"]
    end

    %% ===== virtio/rpmsg 传输层 =====
    subgraph V["virtio/rpmsg 传输层"]
      V1["virtio find_vqs -> vring 初始化 [core]"]
      V2["kick -> ops->kick() [callback]"]
      V3["远端 IRQ -> rproc_vq_interrupt() [core]"]
    end

    %% ===== 连接关系 =====
    I1 -.-> C1
    P2 --> C1
    C1 --> C2 --> C3
    C3 -- 是 --> A0
    C3 -- 否 --> B0

    A0 --> A1 --> A2 --> A3 --> A4 --> A5 --> A6 --> A7
    B0 --> B1 --> B2 --> B3 --> B4 --> B5 --> B6 --> B7

    A4 --> V1
    V1 --> V2
    P4 --> V3
    V3 --> V1

    %% ===== 样式 =====
    classDef platform fill:#f3e5f5,stroke:#8e24aa,color:#000;
    classDef core fill:#e3f2fd,stroke:#1e88e5,color:#000;
    classDef callback fill:#fff3e0,stroke:#fb8c00,color:#000;

    class P0,P1,P2,P3,P4 platform;
    class I1,C1,C2,C3,A0,A3,A4,A5,A7,B0,B1,B4,B5,B7,V1,V3 core;
    class A1,A2,A6,B2,B3,B6,V2 callback;

```

### 速读说明
- `pdev` 是平台设备对象，包含硬件资源；`dev` 是通用设备基类。
- `priv` 是驱动私有结构体，挂在 `pdev` 上。
- `rproc_ops` 是 remoteproc 核心回调表（prepare/kick/get_loaded_rsc_table 等）。
- `resource_table` 在共享内存，Linux attach 会解析 `fw_rsc_vdev` 和 `fw_rsc_vdev_vring`。
- `Mailbox -> PLIC -> IRQ -> rproc_vq_interrupt` 是 doorbell 中断链路。
- `reserved-memory` 为 rpmsg buffer 池提供 DMA 可见内存。

