#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "emulator/system/registers.h"
#include "emulator/io/output.h"

int main(int argc, char **argv) {
    // Check that [argv] is valid, i.e., has 2-3 args.
    if (argc < 2 || argc > 3) return EXIT_FAILURE;

    // int fileIn = open(argv[1], O_RDONLY);

    Registers reg = createReg();

    // TODO: Instantiate memory from input file

    // TODO: Execution cycle, with halt

    // TODO: Print stuff at the end.
    dumpRegs(&reg);

    return EXIT_SUCCESS;
}
