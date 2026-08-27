#!/usr/bin/env bash
set -euo pipefail

SYSFS=/sys/kernel/blink/state
MODULE_DIR="$(cd "$(dirname "$0")/module" && pwd)"
MODULE_NAME=blink_module

BLINK_CYCLES=10
BLINK_DELAY=0.5

MASK_CAPS=4
MASK_ALL=7
MASK_OFF=0

step() { echo; echo "==> $*"; }
ok()   { echo "    [OK] $*"; }
fail() { echo "    [FAIL] $*"; exit 1; }

cleanup() {
    echo 0 | sudo tee "$SYSFS" > /dev/null 2>/dev/null || true
    sudo rmmod "$MODULE_NAME" 2>/dev/null || true
    make -C "$MODULE_DIR" clean -s
}
trap cleanup EXIT

step "Build"
make -C "$MODULE_DIR" -s
ok "blink_module.ko built"

step "Load"
sudo insmod "$MODULE_DIR/blink_module.ko"
ok "module loaded"

step "sysfs entry exists"
test -f "$SYSFS" && ok "$SYSFS exists" || fail "$SYSFS not found"

step "Kernel log after insmod"
sudo dmesg | grep blink_module | tail -3

step "Initial state is 0"
VAL=$(cat "$SYSFS")
test "$VAL" = "0" && ok "state=0 (off)" || fail "expected 0, got $VAL"

step "Blink with Caps Lock (mask=$MASK_CAPS) — $BLINK_CYCLES times, ${BLINK_DELAY}s interval"
echo "$MASK_CAPS" | sudo tee "$SYSFS" > /dev/null
echo "    blinking for $((BLINK_CYCLES / 2)) seconds..."
sleep $((BLINK_CYCLES / 2))
sudo dmesg | grep "KDSETLED" | tail -6

step "Verify state reads back as $MASK_CAPS"
VAL=$(cat "$SYSFS")
test "$VAL" = "$MASK_CAPS" && ok "state=$MASK_CAPS" || fail "expected $MASK_CAPS, got $VAL"

step "All LEDs (mask=$MASK_ALL)"
echo "$MASK_ALL" | sudo tee "$SYSFS" > /dev/null
sleep $((BLINK_CYCLES / 2))
sudo dmesg | grep "KDSETLED" | tail -6

step "Reject out-of-range value (mask=8)"
RESULT=$(echo 8 | sudo tee "$SYSFS" 2>&1 || true)
VAL=$(cat "$SYSFS")
test "$VAL" = "$MASK_ALL" \
    && ok "state unchanged after invalid input" \
    || fail "state changed to: $VAL"

step "Turn off (mask=$MASK_OFF)"
echo "$MASK_OFF" | sudo tee "$SYSFS" > /dev/null
sleep 0.2
VAL=$(cat "$SYSFS")
test "$VAL" = "$MASK_OFF" && ok "state=0 (all off)" || fail "expected 0, got $VAL"

step "Unload"
sudo rmmod "$MODULE_NAME"
ok "module unloaded"

step "sysfs entry gone"
test ! -e "$SYSFS" && ok "$SYSFS removed" || fail "$SYSFS still exists"

step "Kernel log after rmmod"
sudo dmesg | grep blink_module | tail -5

echo
echo "All tests passed."
