///
/// emulate.c
/// Emulate ARM64 code
///
/// Created by Jack Wong on 27/05/2024.
///

#include "emulate.h"

int main(int argc, char **argv) {
    // Check that [argv] is valid, i.e., has 2-3 args.
    if (argc < 2 || argc > 3) return EXIT_FAILURE;

    // Initialise registers and memory.
    Regs_s regs_s = createRegs();
    Registers regs = &regs_s;
    Memory mem = allocMemFromFile(argv[1]);

    // TODO: Execution cycle, with halt
    while (true) {
        BitData pcVal = getRegPC(regs);
        BitData instruction = readMem(mem, 1, pcVal);

        if (instruction == HALT_INSTR_C) {
            // Halt the program
            break;
        }

        // decode(instruction, regs, mem);

        //Increment PC as normal when no branch or jump instructions applied.
        if (pcVal == getRegPC(regs)) incRegPC(regs);
    }

    // Dump contents of register and memory, then free memory.
    FILE *fileOut = stdout;
    if (argc == 3) fileOut = fopen(argv[2], "w");

    dumpRegs(regs, fileOut);
    dumpMem(mem, fileOut);
    freeMem(mem);

    fclose(fileOut);

    return EXIT_SUCCESS;
}
