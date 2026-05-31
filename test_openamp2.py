#!/usr/bin/env python3
# Full BIDIRECTIONAL OpenAMP/rpmsg test for quard_star.
# Boot -> attach remoteproc -> load rpmsg_test.ko -> send a unique payload ->
# verify (a) FreeRTOS received our exact message and (b) FreeRTOS's echo came
# back into Linux.
import os, pty, select, subprocess, sys, time, threading, socket

ROOT = os.path.dirname(os.path.abspath(__file__))
LOG = open("/tmp/openamp2_console.log", "wb")
RTOSLOG = open("/tmp/openamp2_rtos.log", "wb")
TOKEN = "QUARDSTAR_PING_42"     # unique payload to make matching unambiguous

QEMU = f"{ROOT}/output/qemu/bin/qemu-system-riscv64"
ARGS = [
    QEMU, "-M", "quard-star", "-m", "1G", "-smp", "8", "-bios", "none",
    "-drive", f"if=pflash,bus=0,unit=0,format=raw,file={ROOT}/output/fw/fw.bin",
    "-drive", f"file={ROOT}/output/rootfs/rootfs.img,format=raw,id=hd0",
    "-device", "virtio-blk-device,drive=hd0",
    "-fw_cfg", 'name=opt/qemu_cmdline,string=qemu_vc=1280x720',
    "-serial", "mon:stdio", "-serial", "null",
    "-serial", "tcp::4446,server,nowait",
    "-nographic", "--parallel", "none",
]

buf = bytearray(); rtosbuf = bytearray(); lock = threading.Lock()

def rtos_reader():
    deadline = time.time() + 25; s = None
    while time.time() < deadline:
        try: s = socket.create_connection(("127.0.0.1", 4446), timeout=2); break
        except OSError: time.sleep(0.5)
    if not s:
        RTOSLOG.write(b"[no RTOS serial]\n"); RTOSLOG.flush(); return
    s.settimeout(1.0)
    while True:
        try:
            d = s.recv(4096)
            if not d: break
            with lock: rtosbuf.extend(d)
            RTOSLOG.write(d); RTOSLOG.flush()
        except socket.timeout: continue
        except OSError: break

def wait_for(token, timeout, src="con"):
    t = token if isinstance(token, bytes) else token.encode()
    end = time.time() + timeout
    while time.time() < end:
        with lock:
            b = bytes(buf) if src == "con" else bytes(rtosbuf)
        if t in b: return True
        time.sleep(0.2)
    return False

master, slave = pty.openpty()
proc = subprocess.Popen(ARGS, stdin=slave, stdout=slave, stderr=slave, close_fds=True)
os.close(slave)
threading.Thread(target=rtos_reader, daemon=True).start()

def reader():
    while True:
        try:
            r,_,_ = select.select([master], [], [], 0.5)
            if master in r:
                d = os.read(master, 4096)
                if not d: break
                with lock: buf.extend(d)
                LOG.write(d); LOG.flush()
        except OSError: break
threading.Thread(target=reader, daemon=True).start()

def send(line):
    os.write(master, (line + "\n").encode()); time.sleep(0.5)
def mark(tag):
    m = f"\n##### {tag} #####\n".encode()
    with lock: buf.extend(m)
    LOG.write(m); LOG.flush()

R = dict(boot=False, attach=False, channel=False, probe=False,
         rtos_got=False, linux_echo=False, received_file=False)
try:
    R["boot"] = wait_for("Welcome debugging on Qemu Quard Star board", 120)
    time.sleep(3); send("")

    mark("ATTACH"); send("insmod /driver/quard_star_rproc.ko"); time.sleep(2)
    send("echo start > /sys/class/remoteproc/remoteproc0/state")
    R["attach"]  = wait_for("rpmsg host is online", 15)
    R["channel"] = wait_for("creating channel freertos-test", 10)
    time.sleep(2)

    # debugfs may not be mounted by rcS; mount it explicitly.
    mark("LOAD rpmsg_test")
    send("mount -t debugfs none /sys/kernel/debug 2>/dev/null")
    send("insmod /driver/rpmsg_test.ko")
    R["probe"] = wait_for("rpmsg_test: endpoint created", 8) or wait_for("probing new channel", 4)
    send("ls /sys/kernel/debug/rpmsg_test/")
    time.sleep(1)

    # ---- The actual bidirectional exchange ----
    mark("SEND payload")
    send(f'echo "{TOKEN}" > /sys/kernel/debug/rpmsg_test/send')
    # (a) did FreeRTOS receive our exact payload?
    R["rtos_got"]  = wait_for(f"Received: {TOKEN}", 12, src="rtos")
    # (b) did FreeRTOS's echo come back into Linux?
    R["linux_echo"] = wait_for("Echo from FreeRTOS", 12, src="con")
    time.sleep(1)

    mark("READ back")
    send("cat /sys/kernel/debug/rpmsg_test/received; echo")
    send("dmesg | grep -i rpmsg_test | tail -20")
    R["received_file"] = wait_for("Echo from FreeRTOS", 6, src="con") or wait_for(TOKEN, 4, src="con")
    time.sleep(3)
    mark("DONE")
finally:
    try: os.write(master, b"\x01x"); time.sleep(1)
    except OSError: pass
    proc.terminate()
    try: proc.wait(timeout=5)
    except Exception: proc.kill()
    LOG.flush(); RTOSLOG.flush()

print("RESULT", R)
ok = R["boot"] and R["channel"] and R["probe"] and R["rtos_got"] and R["linux_echo"]
print("BIDIRECTIONAL:", "PASS" if ok else "FAIL")
sys.exit(0 if ok else 1)
