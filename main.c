#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "VM/vm.h"
#include "VM/loader.h"
#include "VM/exec.h"


int main(int argc, char **argv) {

    /* CLI rule */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <bytecode_file>\n", argv[0]);
        return VM_EXIT_ERR;
    }

    const char *file = argv[1];

    /* enforce .byc extension */
    const char *ext = strrchr(file, '.');
    if (!ext || strcmp(ext, ".byc") != 0) {
        fprintf(stderr, "error: expected .byc file\n");
        return VM_EXIT_ERR;
    }

    /* load bytecode */
    int size = 0;
    unsigned char *code = load_bytecode(file, &size);
    if (!code)
        return VM_EXIT_ERR;

    /* initialize VM program */
    Program prog;
    vm_init(&prog, code, size);

    vm_validate(&prog);
    vm_dump_bytecode(&prog);

    
    vm_run(&prog);

    vm_free(&prog);
    return VM_EXIT_OK;
}
