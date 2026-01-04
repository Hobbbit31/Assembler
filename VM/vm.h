#ifndef VM_H
#define VM_H

#define STACK_MAX 1024  /* fixed stack capacity */

/* Program = runtime state of the VM */
typedef struct {
    unsigned char *code;   /* bytecode buffer */
    int code_size;          /* number of bytes */

    int pc;                 /* program counter */

    int stack[STACK_MAX];   /* operand stack */
    int sp;                 /* next free slot index */

    int memory[256];        /* LOAD / STORE memory */
} Program;

/* VM interface */
void vm_init(Program *p, unsigned char *code, int size);
/*free  */
void vm_free(Program *p);
/* loader inside vm.c */
unsigned char *load_bytecode(const char *file, int *size);

int vm_validate(Program *p);

void vm_dump_bytecode(Program *p);

/* stack helpers with safety checks */
void vm_push(Program *p, int value);
int vm_pop(Program *p);
#endif
