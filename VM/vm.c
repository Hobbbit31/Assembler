#include "vm.h"
#include <stdlib.h>
#include <stdio.h>


int valid_opcode(unsigned char op) {
    switch (op) {
        case 0x01: 
        case 0x02: 
        case 0x03:
        case 0x10:
        case 0x11: 
        case 0x12:
        case 0x13: 
        case 0x14:
        case 0x20: 
        case 0x21: 
        case 0x22:
        case 0x30: 
        case 0x31:
        case 0x40: 
        case 0x41:
        case 0xFF:
            return 1;
        default:
            return 0;
    }
}


int needs_operand(unsigned char op) {
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

void vm_init(Program *p, unsigned char *code, int size) {
    p->code = code;
    p->code_size = size;
    p->pc = 0;
    p->sp = 0;

    for (int i = 0; i < 256; i++)
        p->memory[i] = 0;
}

void vm_free(Program *p) {
    free(p->code);   /* VM owns bytecode memory */
}

unsigned char *load_bytecode(const char *file, int *size) {
    FILE *f = fopen(file, "rb");
    if (!f) return NULL;

    // /* skip header (8 bytes) intially planned to add a symbol which says that this is the file, if another without symbol file come reject it  */
    // fseek(f, 8, SEEK_SET);

    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buf = malloc(*size);
    fread(buf, 1, *size, f);
    fclose(f);

    return buf;
}

int vm_validate(Program *p) {
    int pc = 0;

    while (pc < p->code_size) {

        unsigned char op = p->code[pc++];

        /* opcode check */
        if (!valid_opcode(op)) {
            fprintf(stderr, "error: invalid opcode 0x%x at pc=%d\n", op, pc - 1);
            exit(1);
        }

        /* truncation check */
        if (needs_operand(op)) {
            if (pc + 4 > p->code_size) {
                fprintf(stderr, "error: truncated instruction at pc=%d\n", pc - 1);
                exit(1);
            }
            pc += 4;  /* skip operand */
        }

        /* HALT stops program */
        if (op == 0xFF) {
            return 1;  /* valid bytecode */
        }
    }

    /* if no HALT found */
    fprintf(stderr, "error: program has no HALT instruction\n");
    exit(1);
}

void vm_dump_bytecode(Program *p) {
    printf("Bytecode dump (%d bytes):\n", p->code_size);

    for (int i = 0; i < p->code_size; i++) {
        printf("%02X ", p->code[i]);

        /* new line every 8 bytes */
        if ((i + 1) % 8 == 0)
            printf("\n");
    }

    printf("\n");
}

