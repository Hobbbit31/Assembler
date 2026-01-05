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

tmp_dir="$(mktemp -d "$ROOT_DIR/test/tmp.XXXXXX")"
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

# Test 4: stack program runs without errors.
stack_ok_bin="$tmp_dir/stack_ok.bin"
if ! "$ASM_BIN" "$ROOT_DIR/test/stack_ok.asm" "$stack_ok_bin" >/dev/null 2>&1; then
    fail_case "assemble stack_ok program"
else
    if ! "$VM_BIN" "$stack_ok_bin" >/dev/null 2>"$tmp_dir/stack_ok.err"; then
        fail_case "stack_ok should run"
    else
        pass "stack_ok program"
    fi
fi

# Test 5: stack underflow is trapped.
stack_underflow_bin="$tmp_dir/stack_underflow.bin"
if ! "$ASM_BIN" "$ROOT_DIR/test/stack_underflow.asm" "$stack_underflow_bin" >/dev/null 2>&1; then
    fail_case "assemble stack_underflow program"
else
    if "$VM_BIN" "$stack_underflow_bin" >/dev/null 2>"$tmp_dir/stack_underflow.err"; then
        fail_case "stack_underflow should fail"
    elif ! grep -q "stack underflow" "$tmp_dir/stack_underflow.err"; then
        fail_case "stack_underflow error message"
    else
        pass "stack_underflow trapped"
    fi
fi

# Test 6: arithmetic program runs without errors.
arith_bin="$tmp_dir/arith.bin"
if ! "$ASM_BIN" "$ROOT_DIR/test/arith.asm" "$arith_bin" >/dev/null 2>&1; then
    fail_case "assemble arith program"
else
    if ! "$VM_BIN" "$arith_bin" >/dev/null 2>"$tmp_dir/arith.err"; then
        fail_case "arith should run"
    else
        pass "arith program"
    fi
fi

# Test 7: division by zero is trapped.
div_zero_bin="$tmp_dir/div_zero.bin"
if ! "$ASM_BIN" "$ROOT_DIR/test/div_zero.asm" "$div_zero_bin" >/dev/null 2>&1; then
    fail_case "assemble div_zero program"
else
    if "$VM_BIN" "$div_zero_bin" >/dev/null 2>"$tmp_dir/div_zero.err"; then
        fail_case "div_zero should fail"
    elif ! grep -q "division by zero" "$tmp_dir/div_zero.err"; then
        fail_case "div_zero error message"
    else
        pass "div_zero trapped"
    fi
fi

if [[ $fail -ne 0 ]]; then
    echo "VM tests failed."
    exit 1
fi

echo "All VM tests passed."
