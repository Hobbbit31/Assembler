# ASM_SYNTAX.md
## VERSION A1 — Assembly Language Specification

### Purpose
This document defines the **exact syntax of the assembly language** understood by the assembler.

Assembly is a **human-readable interface** layered on top of the bytecode defined in
`BYTECODE_SEMANTICS.md (VERSION A0)`.

This file freezes:
- What assembly programs look like
- What is considered valid or invalid assembly
- How assembly maps to bytecode (structurally, not semantically)

---

## General Syntax Rules

### Instruction Format
MNEMONIC [OPERAND]


- Each instruction occupies **one line**
- `MNEMONIC` is mandatory
- `OPERAND` is optional and depends on the mnemonic
- Tokens are separated by **whitespace**
- Extra whitespace is ignored

---

### Comments
; this is a comment



- `;` starts a comment
- Everything after `;` on the same line is ignored
- Comments may appear:
  - On their own line
  - After an instruction

Example:
PUSH 10 ; push literal
ADD ; add top two values


---

### Case Sensitivity
- Mnemonics are **case-insensitive**
- Recommended canonical form: **UPPERCASE**

Valid:
push 10
PuSh 10
PUSH 10



Assembler normalizes internally.

---

### Line Structure Summary

| Element | Required |
|------|------|
| Empty line | Allowed |
| Comment-only line | Allowed |
| Instruction | Exactly one per line |
| Multiple instructions on one line | ❌ Not allowed |

---

## Mnemonic Set (VERSION A1)

The following mnemonics are **valid and reserved**:

PUSH POP DUP
ADD SUB MUL DIV CMP
JMP JZ JNZ
STORE LOAD
CALL RET
HALT


Any unknown mnemonic is a **syntax error**.

---

## Operand Rules (Strict)

### 1. PUSH
PUSH value


- Operand: **signed 32-bit integer**
- Decimal notation only
- Range:

Examples:
PUSH 0
PUSH 42
PUSH -10

Invalid:
PUSH
PUSH 3.14
PUSH 999999999999



---

### 2. JMP / JZ / JNZ / CALL
JMP <addr>
JZ <addr>
JNZ <addr>
CALL <addr>


- Operand: **absolute byte offset**
- Address is an integer ≥ 0
- Address refers to **byte index in final bytecode**
- Labels are **not supported in VERSION A1**

Examples:
JMP 20
JZ 64
CALL 128

Invalid:
JMP -1
JMP label


---

### 3. LOAD / STORE
LOAD <idx>
STORE <idx>


- Operand: **memory index**
- `idx` is a non-negative integer
- Index refers to `mem[idx]` in VM memory

Examples:
STORE 0
LOAD 1


Invalid:
LOAD
STORE -2


---

### 4. Zero-Operand Instructions

The following mnemonics **must not** have operands:

POP
DUP
ADD
SUB
MUL
DIV
CMP
RET
HALT


Valid:
DUP
ADD
HALT


Invalid:
ADD 10
HALT 0



---

## Program Structure

- A program is a **sequence of instructions**
- Execution starts at **byte offset 0**
- Program must end with `HALT`
- Behavior after `HALT` is undefined

Example Program:
PUSH 10
DUP
MUL
HALT



---

## Whitespace Rules

- Leading and trailing whitespace is ignored
- One or more spaces/tabs may separate tokens
- Blank lines are allowed

Valid:
PUSH 10

DUP
MUL
HALT



---

## Error Handling (Assembler Responsibility)

The assembler must reject programs with:
- Unknown mnemonic
- Missing required operand
- Extra operand
- Operand of wrong type
- Operand out of valid range
- Empty program
- Program without `HALT`

Errors must include:
- Line number
- Clear reason

---

## Relationship to Bytecode (A0)

- Assembly is **translated line-by-line** into bytecode
- Operand sizes and meaning are defined **only** in A0
- A1 defines syntax, **not execution semantics**

---

## Versioning Rule

- This file defines **VERSION A1**
- Labels, macros, or symbolic addresses are **explicitly excluded**
- Any extension requires **A2 or later**

---

## Viva Line (Memorize This)

> **“Assembly is for humans; bytecode is for machines.”**
