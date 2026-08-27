#!/usr/bin/env bash
set -euo pipefail

PROC=/proc/quasi_file
MODULE_DIR="$(cd "$(dirname "$0")/module" && pwd)"
MODULE_NAME=proc_module
MAX_SIZE=10

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
ok "proc_module.ko built"

step "Load"
sudo insmod "$MODULE_DIR/proc_module.ko"
ok "module loaded"

step "Kernel log after insmod"
sudo dmesg | grep proc_module | tail -3

step "proc entry exists"
test -f "$PROC" && ok "$PROC exists" || fail "$PROC not found"

step "Write short message and read back"
echo -n "hello" | sudo tee "$PROC" > /dev/null
GOT=$(cat "$PROC")
echo "    got: '$GOT'"
test "$GOT" = "hello" && ok "readback correct" || fail "unexpected: $GOT"

step "Write exactly MAX_SIZE bytes (no truncation)"
printf '%0.s#' $(seq 1 $MAX_SIZE) | sudo tee "$PROC" > /dev/null
GOT=$(cat "$PROC")
echo "    got: '$GOT' (${#GOT} bytes)"
test "${#GOT}" -eq "$MAX_SIZE" && ok "${MAX_SIZE} bytes stored" || fail "expected $MAX_SIZE bytes, got ${#GOT}"

step "Write over MAX_SIZE — truncation to $MAX_SIZE bytes"
LONG="HEY, BRUDDAH"
echo -n "$LONG" | sudo tee "$PROC" > /dev/null
GOT=$(cat "$PROC")
TRUNCATED="${LONG:0:$MAX_SIZE}"
echo "    input:     '$LONG' (${#LONG} bytes)"
echo "    stored:    '$GOT' (${#GOT} bytes)"
echo "    expected:  '$TRUNCATED'"
test "$GOT" = "$TRUNCATED" \
    && ok "truncation correct" \
    || fail "expected '$TRUNCATED', got '$GOT'"

step "Overwrite with new short message"
echo -n "ok" | sudo tee "$PROC" > /dev/null
GOT=$(cat "$PROC")
echo "    got: '$GOT'"
test "$GOT" = "ok" && ok "overwrite correct" || fail "unexpected: $GOT"

step "Unload"
sudo rmmod "$MODULE_NAME"
ok "module unloaded"

step "proc entry gone"
test ! -f "$PROC" && ok "$PROC removed" || fail "$PROC still exists"

step "Kernel log after rmmod"
sudo dmesg | grep proc_module | tail -3

echo
echo "All tests passed."
