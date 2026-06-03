#!/usr/bin/env python3
# RV-Mailbox 在标准 QEMU virt 机器上的 Linux 自测演示。
# 启动 virt + 我们的 v6.10 Image + rootfs，登录后 insmod 自测驱动并抓 dmesg。
import os, pty, select, subprocess, time, sys

QB = "source/qemu-8.0.2/build/qemu-system-riscv64"
KERNEL = "output/linux_kernel/Image"
ROOTFS = "output/rootfs/rootfs.img"
LOG = open("/tmp/virt_mailbox_console.log", "wb")

cmd = [
    QB, "-M", "virt", "-m", "1G", "-smp", "4", "-nographic",
    "-kernel", KERNEL,
    "-drive", f"file={ROOTFS},format=raw,id=hd0",
    "-device", "virtio-blk-device,drive=hd0",
    "-append", "root=/dev/vda2 rw console=ttyS0 earlycon",
]

# 单 pty 双向：qemu 的 stdin/stdout/stderr 都接 slave，我们读写 master
master, slave = pty.openpty()
p = subprocess.Popen(cmd, stdin=slave, stdout=slave, stderr=slave, close_fds=True)
os.close(slave)

buf = b""
def feed(t=0.3):
    global buf
    r,_,_ = select.select([master], [], [], t)
    if master in r:
        try: d = os.read(master, 65536)
        except OSError: return b""
        if d:
            LOG.write(d); LOG.flush()
            sys.stdout.write(d.decode(errors="replace")); sys.stdout.flush()
            buf += d
            return d
    return b""

def wait_for(pats, timeout):
    t0 = time.time()
    while time.time() - t0 < timeout:
        feed(0.3)
        for pat in pats:
            if pat.encode() in buf:
                return pat
    return None

def send(s):
    os.write(master, (s + "\n").encode()); time.sleep(0.4)

res = {"boot": False, "logged_in": False, "dt_node": False,
       "rev_ok": False, "irq_present": False, "selftest_pass": False}
try:
    # 1) 等 shell 提示符（buildroot 自动登录到 ~ #）
    hit = wait_for(["~ #", "buildroot login:", "login:", "/ #"], 90)
    res["boot"] = bool(hit)
    if hit and "login:" in hit:
        send("root"); wait_for(["~ #", "# "], 10)
    send(""); wait_for(["~ #", "# "], 8)
    res["logged_in"] = True

    # 哨兵用 $? 解析后的数字，命令回显里是字面 "$?"，不会自匹配
    def run(c, key, timeout):
        global buf
        buf = b""
        send(f"{c}; echo {key}_$?")
        wait_for([f"{key}_0", f"{key}_1", f"{key}_2", f"{key}_127"], timeout)
        return buf.decode(errors="replace")

    dt = run("cat /proc/device-tree/soc/mailbox@102000/compatible 2>/dev/null", "DT", 8)
    res["dt_node"] = "qemu,riscv-mailbox" in dt

    run("insmod /root/rv_mailbox_selftest.ko 2>&1", "INS", 15)

    txt = run("dmesg | grep -iE 'RV-Mailbox|self-test'", "DM", 12)
    res["rev_ok"] = "REVISION=0x0100" in txt
    res["selftest_pass"] = "self-test PASS" in txt

    irqs = run("cat /proc/interrupts", "IRQ", 10)
    res["irq_present"] = "rv_mailbox_selftest" in irqs

finally:
    try: send("poweroff -f")
    except Exception: pass
    time.sleep(2)
    p.terminate()
    try: p.wait(timeout=5)
    except Exception: p.kill()
    LOG.close()

print("\n\nRESULT", res)
ok = res["boot"] and res["selftest_pass"]
print("VIRT-MAILBOX-LINUX:", "PASS" if ok else "FAIL")
sys.exit(0 if ok else 1)
