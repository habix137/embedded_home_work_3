#!/usr/bin/env bash
#
# Fill ~85 % of the system’s RAM with anonymous memory and keep it there
# until you press Ctrl-C.  Works on any Debian/Ubuntu/Raspbian/Armbian box
# without extra packages.

set -e

# 1. How much RAM do we want?  (kB → bytes)
total_kb=$(awk '/MemTotal/ {print $2}' /proc/meminfo)
target_bytes=$(( total_kb * 85 / 100 * 1024 ))     # 85 % of total

echo "Allocating $((target_bytes / 1024 / 1024)) MB (~85 %) …"

python3 - <<PY
import mmap, time, os, sys, signal

need = $target_bytes
print(f"PID {os.getpid()} is now holding", need//1024//1024, "MB …")

# Allocate the desired number of anonymous memory pages.
buf = mmap.mmap(-1, need)

# Touch every page so the kernel really commits RAM (no over-commit cheat).
page = mmap.PAGESIZE
for offset in range(0, need, page):
    buf[offset:offset+1] = b'\0'

print("Memory committed.  Press Ctrl-C to release.")
try:
    signal.pause()   # sleep forever
except KeyboardInterrupt:
    pass
PY