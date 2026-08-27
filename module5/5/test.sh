#!/usr/bin/env bash
set -euo pipefail

MODULE_DIR="$(cd "$(dirname "$0")/module" && pwd)"
USER_DIR="$(cd "$(dirname "$0")/user" && pwd)"
MODULE_NAME=netlink_mod
CLIENT="$USER_DIR/nl_client"

step() { echo; echo "==> $*"; }
ok()   { echo "    [OK] $*"; }

cleanup() {
    sudo rmmod "$MODULE_NAME" 2>/dev/null || true
    make -C "$MODULE_DIR" clean -s
    rm -f "$CLIENT"
}
trap cleanup EXIT

step "Build kernel module"
make -C "$MODULE_DIR" -s
ok "netlink_mod.ko built"

step "Build userspace client"
gcc -O2 -Wall -o "$CLIENT" "$USER_DIR/nl_client.c"
ok "nl_client built"

step "Load module"
sudo insmod "$MODULE_DIR/netlink_mod.ko"
ok "module loaded"

step "Kernel log after insmod"
sudo dmesg | grep netlink | tail -3

step "Send message and receive reply"
OUTPUT=$("$CLIENT")
echo "    $OUTPUT"
echo "$OUTPUT" | grep -q "received: hello from kernel" \
    && ok "reply matches expected" \
    || { echo "    [FAIL] unexpected output"; exit 1; }

step "Kernel log: received message logged"
sudo dmesg | grep "netlink: received" | tail -3

step "Unload module"
sudo rmmod "$MODULE_NAME"
ok "module unloaded"

step "Kernel log after rmmod"
sudo dmesg | grep netlink | tail -3

echo
echo "All tests passed."
