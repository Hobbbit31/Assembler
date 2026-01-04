#include "loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BVM_MAGIC "BVM1"   /* 4-byte magic tag */
#define BVM_HEADER_SIZE 8  /* magic(4) + code_size(4) */

static int valid_opcode(unsigned char op) {
    /* whitelist of supported opcodes */
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

unsigned char *load_bytecode(const char *file, int *size) {
    FILE *f = fopen(file, "rb");
    if (!f) {
        fprintf(stderr, "error: cannot open %s\n", file);
        return NULL;
    }

    /* read and validate header: "BVM1" + int32 code size */
    unsigned char header[BVM_HEADER_SIZE];
    if (fread(header, 1, BVM_HEADER_SIZE, f) != BVM_HEADER_SIZE) {
        fprintf(stderr, "error: invalid bytecode header\n");
        fclose(f);
        return NULL;
    }

    if (memcmp(header, BVM_MAGIC, 4) != 0) {
        fprintf(stderr, "error: bad magic number\n");
        fclose(f);
        return NULL;
    }

    /* decode little-endian size */
    int code_size = 0;
    code_size |= (int)header[4];
    code_size |= (int)header[5] << 8;
    code_size |= (int)header[6] << 16;
    code_size |= (int)header[7] << 24;
    if (code_size < 0) {
        fprintf(stderr, "error: invalid code size\n");
        fclose(f);
        return NULL;
    }

    /* confirm file size matches header */
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    if (file_size != (long)(BVM_HEADER_SIZE + code_size)) {
        fprintf(stderr, "error: bytecode size mismatch\n");
        fclose(f);
        return NULL;
    }
    fseek(f, BVM_HEADER_SIZE, SEEK_SET);

    unsigned char *buf = malloc(code_size);
    if (!buf) {
        fprintf(stderr, "error: out of memory\n");
        fclose(f);
        return NULL;
    }
    if (fread(buf, 1, code_size, f) != (size_t)code_size) {
        fprintf(stderr, "error: truncated bytecode\n");
        free(buf);
        fclose(f);
        return NULL;
    }
    fclose(f);

    *size = code_size;
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
