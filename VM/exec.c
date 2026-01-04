#include "exec.h"

#include <stdio.h>
#include <stdlib.h>

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
            case 0x01: /* PUSH */ break;
            case 0x02: /* POP */ break;
            case 0x03: /* DUP */ break;
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
