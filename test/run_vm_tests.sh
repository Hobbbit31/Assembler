#!/usr/bin/env bash
set -u
set -o pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ASM_BIN="$ROOT_DIR/assembler"
VM_BIN="$ROOT_DIR/bvm"

if [[ ! -x "$ASM_BIN" || ! -x "$VM_BIN" ]]; then
    echo "error: missing binaries; run 'make' first."
    exit 1
fi

tmp_dir="$(mktemp -d)"
trap 'rm -rf "$tmp_dir"' EXIT

fail=0

pass() {
    echo "PASS: $1"
}

fail_case() {
    echo "FAIL: $1"
    fail=1
}

# Test 1: simple program assembles and dumps expected bytes.
simple_bin="$tmp_dir/simple.bin"
if ! "$ASM_BIN" "$ROOT_DIR/test/test.asm" "$simple_bin" >"$tmp_dir/asm_simple.out" 2>&1; then
    fail_case "assemble simple program"
else
    if ! "$VM_BIN" "$simple_bin" >"$tmp_dir/vm_simple.out" 2>"$tmp_dir/vm_simple.err"; then
        fail_case "vm simple program exit"
    elif ! grep -q "01 0A 00 00 00 FF" "$tmp_dir/vm_simple.out"; then
        fail_case "vm simple program output"
    else
        pass "simple program"
    fi
fi

# Test 2: missing HALT should be rejected by VM validation.
nohalt_bin="$tmp_dir/nohalt.bin"
if ! "$ASM_BIN" "$ROOT_DIR/test/nohalt.asm" "$nohalt_bin" >/dev/null 2>&1; then
    fail_case "assemble nohalt program"
else
    if "$VM_BIN" "$nohalt_bin" >/dev/null 2>"$tmp_dir/nohalt.err"; then
        fail_case "nohalt should fail"
    elif ! grep -q "no HALT" "$tmp_dir/nohalt.err"; then
        fail_case "nohalt error message"
    else
        pass "nohalt program rejected"
    fi
fi

# Test 3: invalid opcode should be rejected.
printf '\x99' > "$tmp_dir/invalid.bin"
if "$VM_BIN" "$tmp_dir/invalid.bin" >/dev/null 2>"$tmp_dir/invalid.err"; then
    fail_case "invalid opcode should fail"
elif ! grep -q "invalid opcode" "$tmp_dir/invalid.err"; then
    fail_case "invalid opcode error message"
else
    pass "invalid opcode rejected"
fi

if [[ $fail -ne 0 ]]; then
    echo "VM tests failed."
    exit 1
fi

echo "All VM tests passed."
