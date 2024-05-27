#include <stdlib.h>
#include <stdio.h>

#include "emulator/system/registers.h"
#include "emulator/io/output.h"

int main(int argc, char **argv) {
    // TODO: Check input arguments are valid.
    // Checks that there is an input file, returns elsewise.
    if (argc < 2) return EXIT_FAILURE;
    int fileIn = argv[1];
    int fileOut;
    // Output file is by default stdout, and a given file if provided.
    if (argc > 2) {
        fileOut = stdout;
    } else fileOut = argv[2];

    // TODO: File handler

    Registers reg = createReg();

    // TODO: Instantiate memory from input file

    // TODO: Execution cycle, with halt

    // TODO: Print stuff at the end.
    dumpRegs(&reg);

    return EXIT_SUCCESS;
}
