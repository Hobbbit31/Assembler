#include "exec.h"
#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

static int needs_operand(unsigned char op) {
    /* instructions that carry a 4-byte operand */
    return (
        op == 0x01 ||  /* PUSH */
        op == 0x20 ||  /* JMP */
        op == 0x21 ||  /* JZ */
        op == 0x22 ||  /* JNZ */
        op == 0x30 ||  /* STORE */
        op == 0x31 ||  /* LOAD */
        op == 0x40     /* CALL */
    );
}

static int read_int32(const unsigned char *code, int offset) {
    /* little-endian 4-byte integer */
    uint32_t b0 = (uint32_t)code[offset];
    uint32_t b1 = (uint32_t)code[offset + 1] << 8;
    uint32_t b2 = (uint32_t)code[offset + 2] << 16;
    uint32_t b3 = (uint32_t)code[offset + 3] << 24;
    return (int)(int32_t)(b0 | b1 | b2 | b3);
}

void vm_run(Program *p) {
    /* V2: fetch + decode skeleton (execution comes later) */
    while (p->pc < p->code_size) {

        int pc = p->pc;
        unsigned char op = p->code[pc];

        /* fixed-width instruction step */
        if (needs_operand(op))
            p->pc = pc + 5;
        else
            p->pc = pc + 1;

        switch (op) {
            case 0x01: { /* PUSH */
                int value = read_int32(p->code, pc + 1);
                vm_push(p, value);
                break;
            }
            case 0x02: /* POP */
                (void)vm_pop(p);
                break;
            case 0x03: { /* DUP */
                int value = vm_pop(p);
                vm_push(p, value);
                vm_push(p, value);
                break;
            }
            case 0x10: /* ADD */ break;
            case 0x11: /* SUB */ break;
            case 0x12: /* MUL */ break;
            case 0x13: /* DIV */ break;
            case 0x14: /* CMP */ break;
            case 0x20: /* JMP */ break;
            case 0x21: /* JZ */ break;
            case 0x22: /* JNZ */ break;
            case 0x30: /* STORE */ break;
            case 0x31: /* LOAD */ break;
            case 0x40: /* CALL */ break;
            case 0x41: /* RET */ break;
            case 0xFF: /* HALT */ return;
            default:
                fprintf(stderr, "error: invalid opcode 0x%x at pc=%d\n", op, pc);
                exit(1);
        }
    }
}
