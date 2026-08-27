#!/usr/bin/env bash
set -euo pipefail

MODULE_DIR="$(cd "$(dirname "$0")/module" && pwd)"
MODULE_NAME=ipblock
PROC=/proc/ipblock/blacklist
TARGET_IP=127.0.0.2

step() { echo; echo "==> $*"; }
ok()   { echo "    [OK] $*"; }
fail() { echo "    [FAIL] $*"; exit 1; }

cleanup() {
    sudo rmmod "$MODULE_NAME" 2>/dev/null || true
    make -C "$MODULE_DIR" clean -s
}
trap cleanup EXIT

step "Build"
make -C "$MODULE_DIR" -s
ok "ipblock.ko built"

step "Load"
sudo insmod "$MODULE_DIR/ipblock.ko"
ok "module loaded"

step "Kernel log after insmod"
sudo dmesg | grep ipblock | tail -3

step "proc entry exists"
test -f "$PROC" && ok "$PROC exists" || fail "$PROC not found"

step "Blacklist is empty"
CONTENT=$(sudo cat "$PROC")
test -z "$CONTENT" && ok "empty" || fail "expected empty, got: $CONTENT"

step "Add $TARGET_IP to blacklist"
echo "+$TARGET_IP" | sudo tee "$PROC" > /dev/null
ok "write done"

step "List shows $TARGET_IP"
CONTENT=$(sudo cat "$PROC")
echo "    $CONTENT"
echo "$CONTENT" | grep -q "$TARGET_IP" \
    && ok "$TARGET_IP in list" || fail "not in list"

step "Packet to $TARGET_IP is dropped"
if ping -c 1 -W 1 "$TARGET_IP" > /dev/null 2>&1; then
    fail "packet was NOT dropped (ping succeeded)"
else
    ok "ping failed — packet dropped as expected"
fi

step "Kernel log: drop recorded"
sudo dmesg | grep "ipblock: dropped" | tail -3

step "Remove $TARGET_IP from blacklist"
echo "-$TARGET_IP" | sudo tee "$PROC" > /dev/null
ok "write done"

step "Blacklist empty again"
CONTENT=$(sudo cat "$PROC")
test -z "$CONTENT" && ok "empty" || fail "expected empty, got: $CONTENT"

step "Packet to $TARGET_IP passes now"
if ping -c 1 -W 1 "$TARGET_IP" > /dev/null 2>&1; then
    ok "ping succeeded — packet passed"
else
    fail "ping still fails (unexpected)"
fi

step "Unload"
sudo rmmod "$MODULE_NAME"
ok "module unloaded"

step "proc entry gone"
test ! -e "$PROC" && ok "$PROC removed" || fail "$PROC still exists"

step "Kernel log after rmmod"
sudo dmesg | grep ipblock | tail -3

echo
echo "All tests passed."
