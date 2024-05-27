#include <stdlib.h>

#include "emulator/system/registers.h"
#include "emulator/io/output.h"

int main(int argc, char **argv) {
    // TODO: Check input arguments are valid.

    // TODO: File handler

    Registers reg = createReg();

    // TODO: Instantiate memory from input file

    // TODO: Execution cycle, with halt

    // TODO: Print stuff at the end.
    dumpRegs(&reg);

    return EXIT_SUCCESS;
}
