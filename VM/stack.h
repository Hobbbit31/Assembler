#ifndef STACK_H
#define STACK_H

#include "vm.h"

/* operand stack helpers with safety checks */
void vm_push(Program *p, int value);
int vm_pop(Program *p);

#endif
