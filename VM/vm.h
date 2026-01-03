#ifndef VM_H
#define VM_H

/* Program = runtime state of the VM */
typedef struct {
    unsigned char *code;   /* bytecode buffer */
    int code_size;          /* number of bytes */

    int pc;                 /* program counter */

    int stack[1024];        /* operand stack */
    int sp;                 /* stack pointer */

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
#endif
