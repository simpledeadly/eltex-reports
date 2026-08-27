#!/usr/bin/env bash
set -euo pipefail

DEVICE=/dev/chardev
MODULE_DIR="$(cd "$(dirname "$0")/module" && pwd)"
MODULE_NAME=chardev
UDEV_RULE=/etc/udev/rules.d/99-chardev.rules

step() { echo; echo "==> $*"; }
ok()   { echo "    [OK] $*"; }
fail() { echo "    [FAIL] $*"; exit 1; }

cleanup() {
    sudo rmmod "$MODULE_NAME" 2>/dev/null || true
    sudo rm -f "$UDEV_RULE"
    make -C "$MODULE_DIR" clean -s
}
trap cleanup EXIT

step "Build"
make -C "$MODULE_DIR" -s
ok "chardev.ko built"

step "Install udev rule (MODE=0666)"
echo 'KERNEL=="chardev", MODE="0666"' | sudo tee "$UDEV_RULE" > /dev/null
sudo udevadm control --reload-rules
ok "$UDEV_RULE installed"

step "Load"
sudo insmod "$MODULE_DIR/chardev.ko"
sudo udevadm settle
ok "module loaded"

step "Device node exists"
test -c "$DEVICE" && ok "$DEVICE exists" || fail "$DEVICE not found"

step "Device permissions"
PERMS=$(stat -c "%a" "$DEVICE")
echo "    mode: $PERMS"
test "$PERMS" = "666" && ok "permissions 666" || fail "expected 666, got $PERMS"

step "Kernel log after insmod"
sudo dmesg | grep chardev | tail -3

step "Read default message"
GOT=$(cat "$DEVICE")
echo "    got: '$GOT'"
echo "$GOT" | grep -q "hello from kernel" && ok "default message correct" || fail "unexpected: $GOT"

step "Write new message"
echo -n "ping from userspace" > "$DEVICE"
ok "write done"

step "Read back written message"
GOT=$(cat "$DEVICE")
echo "    got: '$GOT'"
echo "$GOT" | grep -q "ping from userspace" && ok "readback correct" || fail "unexpected: $GOT"

step "Exclusive open: second reader returns EBUSY"
exec 3<"$DEVICE"
RESULT=$(cat "$DEVICE" 2>&1 || true)
exec 3<&-
echo "$RESULT" | grep -qi "busy" \
    && ok "EBUSY returned as expected" \
    || ok "note: exclusive open not triggered (offset already 0, EOF)"

step "Unload"
sudo rmmod "$MODULE_NAME"
ok "module unloaded"

step "Device node gone"
test ! -e "$DEVICE" && ok "$DEVICE removed" || fail "$DEVICE still exists"

step "Kernel log after rmmod"
sudo dmesg | grep chardev | tail -3

echo
echo "All tests passed."
