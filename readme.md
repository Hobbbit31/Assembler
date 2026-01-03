# BYTECODE_SEMANTICS.md
## VERSION A0 — Instruction Set & Bytecode Meaning (FOUNDATION)

### Purpose
This document defines the **authoritative bytecode specification** for the virtual machine.
It freezes the contract between the **assembler** (text → bytes) and the **VM** (bytes → execution).

Once finalized, **this file must not change** without versioning the VM.

---

## VM Execution Model (Assumptions)

- **Architecture**: Stack-based Virtual Machine
- **Operands**: Instructions operate on the operand stack unless stated otherwise
- **PC (Program Counter)**: Advances sequentially unless modified by control-flow instructions
- **Memory Model**:
  - Stack: Used for expression evaluation
  - Memory Array (`mem[]`): Indexed storage for variables
- **Endianness**: Little-endian for multi-byte operands
- **Integer Size**: 32-bit signed integers

---

## Opcode Table (Single Source of Truth)

| Mnemonic | Opcode | Operand | Meaning |
|--------|--------|---------|--------|
| PUSH  | `0x01` | int32 | Push literal integer onto stack |
| POP   | `0x02` | — | Pop top value from stack |
| DUP   | `0x03` | — | Duplicate top value on stack |
| ADD   | `0x10` | — | Pop `b`, pop `a`, push `a + b` |
| SUB   | `0x11` | — | Pop `b`, pop `a`, push `a - b` |
| MUL   | `0x12` | — | Pop `b`, pop `a`, push `a * b` |
| DIV   | `0x13` | — | Pop `b`, pop `a`, push `a / b` |
| CMP   | `0x14` | — | Pop `b`, pop `a`, push `1` if `a < b` else `0` |
| JMP   | `0x20` | addr | Unconditional jump to address |
| JZ    | `0x21` | addr | Jump if popped value == 0 |
| JNZ   | `0x22` | addr | Jump if popped value != 0 |
| STORE | `0x30` | idx | Pop value → `mem[idx]` |
| LOAD  | `0x31` | idx | Push `mem[idx]` onto stack |
| CALL  | `0x40` | addr | Call function at address |
| RET   | `0x41` | — | Return from function |
| HALT  | `0xFF` | — | Stop execution |

---

## Instruction Semantics (Formal)

### Stack Operations
- **PUSH int32**
  - Stack ← Stack ∪ {int32}

- **POP**
  - Stack ← Stack − {top}

- **DUP**
  - Stack ← Stack ∪ {top(Stack)}

---

### Arithmetic Operations
(All arithmetic instructions pop operands in **reverse order**)

- **ADD**
  - `b = pop()`, `a = pop()`
  - `push(a + b)`

- **SUB**
  - `b = pop()`, `a = pop()`
  - `push(a - b)`

- **MUL**
  - `b = pop()`, `a = pop()`
  - `push(a * b)`

- **DIV**
  - `b = pop()`, `a = pop()`
  - `push(a / b)`
  - ⚠ Division by zero → VM runtime error

---

### Comparison
- **CMP**
  - `b = pop()`, `a = pop()`
  - `push(1)` if `a < b`
  - `push(0)` otherwise

---

### Control Flow
- **JMP addr**
  - `PC ← addr`

- **JZ addr**
  - `v = pop()`
  - if `v == 0` → `PC ← addr`

- **JNZ addr**
  - `v = pop()`
  - if `v != 0` → `PC ← addr`

---

### Memory Access
- **STORE idx**
  - `mem[idx] ← pop()`

- **LOAD idx**
  - `push(mem[idx])`

---

### Function Control
- **CALL addr**
  - Push return address
  - `PC ← addr`

- **RET**
  - Pop return address
  - `PC ← return_address`

---

### Termination
- **HALT**
  - Stop VM execution immediately

---

## Bytecode Examples

### Example 1: Simple Arithmetic
Assembly:
- PUSH 10
- PUSH 20
- ADD
- HALT

Bytecode:
- 01 0A 00 00 00
- 01 14 00 00 00
- 10
- FF


---

### Example 2: Memory Store & Load
Assembly:

- PUSH 42
- STORE 0
- LOAD 0
- HALT


Bytecode:


- 01 2A 00 00 00
- 30 00
- 31 00
- FF


---

## Error Conditions (VM Responsibility)

- Stack underflow
- Invalid opcode
- Invalid memory index
- Division by zero
- Invalid jump address

---

## Versioning Rule

- This file defines **VERSION A0**
- Any change requires:
  - New version number (A1, A2, …)
  - VM + Assembler update together

---

## Viva Line (Memorize This)

> **“The VM does not understand text; it only understands bytes.”**
