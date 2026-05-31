#!/usr/bin/env python3
# Automated boot + OpenAMP/rpmsg test for quard_star.
# Boots QEMU (mirrors run.sh, minus the -d in_asm trace), drives the console
# shell to load the remoteproc driver and attach to the running FreeRTOS domain,
# then checks for the rpmsg success markers.
import os, pty, select, subprocess, sys, time, threading, socket

ROOT = os.path.dirname(os.path.abspath(__file__))
LOG = open("/tmp/openamp_console.log", "wb")
RTOSLOG = open("/tmp/openamp_rtos.log", "wb")

QEMU = f"{ROOT}/output/qemu/bin/qemu-system-riscv64"
ARGS = [
    QEMU, "-M", "quard-star", "-m", "1G", "-smp", "8", "-bios", "none",
    "-drive", f"if=pflash,bus=0,unit=0,format=raw,file={ROOT}/output/fw/fw.bin",
    "-drive", f"file={ROOT}/output/rootfs/rootfs.img,format=raw,id=hd0",
    "-device", "virtio-blk-device,drive=hd0",
    "-fw_cfg", 'name=opt/qemu_cmdline,string=qemu_vc=1280x720',
    "-serial", "mon:stdio",          # serial0 = Linux console (our pty)
    "-serial", "null",               # serial1
    "-serial", "tcp::4446,server,nowait",  # serial2 = FreeRTOS/RTOS domain
    "-nographic", "--parallel", "none",
]

buf = bytearray()
lock = threading.Lock()

def rtos_reader():
    # Connect to the RTOS serial (serial2) and log whatever FreeRTOS prints.
    deadline = time.time() + 20
    s = None
    while time.time() < deadline:
        try:
            s = socket.create_connection(("127.0.0.1", 4446), timeout=2)
            break
        except OSError:
            time.sleep(0.5)
    if not s:
        RTOSLOG.write(b"[could not connect to RTOS serial :4446]\n"); RTOSLOG.flush(); return
    s.settimeout(1.0)
    try:
        while True:
            try:
                d = s.recv(4096)
                if not d: break
                RTOSLOG.write(d); RTOSLOG.flush()
            except socket.timeout:
                continue
    except OSError:
        pass

def wait_for(token, timeout, after=b""):
    """Wait until token (bytes) appears in buf after the current tail; returns True/False."""
    t = token if isinstance(token, bytes) else token.encode()
    end = time.time() + timeout
    while time.time() < end:
        with lock:
            if t in bytes(buf):
                return True
        time.sleep(0.2)
    return False

master, slave = pty.openpty()
proc = subprocess.Popen(ARGS, stdin=slave, stdout=slave, stderr=slave, close_fds=True)
os.close(slave)

threading.Thread(target=rtos_reader, daemon=True).start()

def reader():
    while True:
        try:
            r, _, _ = select.select([master], [], [], 0.5)
            if master in r:
                d = os.read(master, 4096)
                if not d: break
                with lock:
                    buf.extend(d)
                LOG.write(d); LOG.flush()
        except OSError:
            break
threading.Thread(target=reader, daemon=True).start()

def send(line):
    os.write(master, (line + "\n").encode())
    time.sleep(0.4)

def mark(tag):
    with lock:
        buf.extend(f"\n##### {tag} #####\n".encode())
    LOG.write(f"\n##### {tag} #####\n".encode()); LOG.flush()

result = {"boot": False, "insmod": False, "attach": False, "online": False,
          "channel": False, "ctrl_node": False}
try:
    # 1) Wait for boot to reach the console shell.
    result["boot"] = wait_for("Welcome debugging on Qemu Quard Star board", 120)
    time.sleep(3)
    send("")          # nudge prompt
    send("uname -a")
    time.sleep(1)

    # 2) Load the remoteproc (attach-only) driver.
    mark("INSMOD rproc")
    send("insmod /driver/quard_star_rproc.ko")
    time.sleep(2)
    send("ls /sys/class/remoteproc/")
    result["insmod"] = wait_for("remoteproc0", 8)

    # 3) Attach to the running FreeRTOS domain.
    mark("ECHO start")
    send("echo start > /sys/class/remoteproc/remoteproc0/state")
    result["online"]  = wait_for("rpmsg host is online", 15)
    result["channel"] = wait_for("creating channel freertos-test", 10)
    time.sleep(2)

    # 4) Inspect results.
    mark("INSPECT")
    send("cat /sys/class/remoteproc/remoteproc0/state")
    send("ls -l /dev/rpmsg*")
    result["ctrl_node"] = wait_for("/dev/rpmsg_ctrl0", 5) or wait_for("rpmsg_ctrl0", 3)
    send("dmesg | grep -iE 'rproc|rpmsg|virtio' | tail -40")
    time.sleep(3)
    mark("DONE")
    result["attach"] = result["online"] or result["channel"]
finally:
    try:
        os.write(master, b"\x01x")  # Ctrl-A X -> quit qemu
        time.sleep(1)
    except OSError:
        pass
    proc.terminate()
    try: proc.wait(timeout=5)
    except Exception: proc.kill()
    LOG.flush(); RTOSLOG.flush()

print("RESULT", result)
ok = result["boot"] and result["online"] and result["channel"]
print("OVERALL:", "PASS" if ok else "FAIL")
sys.exit(0 if ok else 1)
