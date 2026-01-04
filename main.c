#include <stdio.h>
#include <stdlib.h>
#include "VM/vm.h"
#include "VM/loader.h"
#include "VM/exec.h"


int main(int argc, char **argv) {

    /* CLI rule */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <bytecode_file>\n", argv[0]);
        return 1;
    }

    const char *file = argv[1];

    /* load bytecode */
    int size = 0;
    unsigned char *code = load_bytecode(file, &size);
    if (!code)
        return 1;

    /* initialize VM program */
    Program prog;
    vm_init(&prog, code, size);

    vm_validate(&prog);
    vm_dump_bytecode(&prog);

    
    vm_run(&prog);

    vm_free(&prog);
    return 0;
}
