#define _GNU_SOURCE
#include <stdlib.h>

#include "emulator/system/registers.h"
#include "emulator/system/memory.h"
#include "emulator/io/output.h"

int main(int argc, char **argv) {
    // Check that [argv] is valid, i.e., has 2-3 args.
    // if (argc < 2 || argc > 3) return EXIT_FAILURE;

    // int fileIn = open(argv[1], O_RDONLY);

    Registers reg = createReg();
    Memory mem = allocMem();

    // TODO: Instantiate memory from input file

    // TODO: Execution cycle, with halt

    // Dump contents of register and memory.
    dumpRegs(&reg);
    dumpMem(mem);

    return EXIT_SUCCESS;
}
