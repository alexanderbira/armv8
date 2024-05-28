#include <stdlib.h>

#include "emulator/system/registers.h"
#include "emulator/system/memory.h"
#include "emulator/io/output.h"

int main(int argc, char **argv) {
    // Check that [argv] is valid, i.e., has 2-3 args.
    if (argc < 2 || argc > 3) return EXIT_FAILURE;

    // Initialise registers and memory.
    Regs reg = createReg();
    Memory mem = allocMemFromFile(argv[1]);

    // TODO: Execution cycle, with halt

    // Dump contents of register and memory, then free memory.
    FILE *fileOut = stdout;
    if (argc == 3) fileOut = fopen(argv[2], "w");

    dumpRegs(&reg, fileOut);
    dumpMem(mem, fileOut);
    freeMem(mem);

    return EXIT_SUCCESS;
}
