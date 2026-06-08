/*
 * OpenAMP-based RPMsg for FreeRTOS trusted domain — public API
 * Copyright (c) 2025 Quard Star Project
 */
#ifndef OPENAMP_RPMSG_H
#define OPENAMP_RPMSG_H

/* 初始化 OpenAMP rpmsg（remoteproc + virtio + 命名端点 + NS 宣告）。返回 0 成功 */
int  openamp_rpmsg_init(void);

/* 轮询处理 Linux 发来的 rpmsg（在主循环中周期调用）*/
void openamp_rpmsg_poll(void);

#endif /* OPENAMP_RPMSG_H */
