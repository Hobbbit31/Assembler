#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

void vm_push(Program *p, int value) {
    /* prevent writing past the fixed stack */
    if (p->sp >= STACK_MAX) {
        fprintf(stderr, "error: stack overflow\n");
        exit(1);
    }
    p->stack[p->sp++] = value;
}

int vm_pop(Program *p) {
    /* prevent popping from an empty stack */
    if (p->sp <= 0) {
        fprintf(stderr, "error: stack underflow\n");
        exit(1);
    }
    return p->stack[--p->sp];
}
