# VM Report (Runtime Side)

## Execution Notes
- The VM runs a fixed-width instruction stream (1 byte opcode + optional 4-byte operand).
- Execution stops on `HALT` or on a runtime error (stack, divide-by-zero, invalid jump, invalid opcode).

## Instruction Count
- The VM increments a counter for each executed instruction and prints it on `HALT`.

## Limitations
- No output instruction is implemented, so tests validate correctness by absence of errors.
- Jump addresses are absolute byte offsets in the loaded bytecode.
