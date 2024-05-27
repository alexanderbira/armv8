#include <stdlib.h>
#include <sys/fcntl.h>

#include "emulator/system/registers.h"
#include "emulator/system/memory.h"
#include "emulator/io/output.h"

int main(int argc, char **argv) {
    // Check that [argv] is valid, i.e., has 2-3 args.
    if (argc < 2 || argc > 3) return EXIT_FAILURE;

    int fileIn = open(argv[1], O_RDONLY);

    // Initialise registers and memory.
    Registers reg = createReg();
    Memory mem = allocMemFromFile(fileIn);

    // TODO: Execution cycle, with halt

    // Dump contents of register and memory.
    FILE *fileOut = stdout;
    if (argc == 3) fileOut = fopen(argv[2], "w");

    dumpRegs(&reg, fileOut);
    dumpMem(mem, fileOut);

    return EXIT_SUCCESS;
}
