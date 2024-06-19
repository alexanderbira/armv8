///
/// emulate.c
/// Emulates execution of an AArch64 binary file.
///
/// Created by Konstantinos Gkoutzis on 23/05/24.
///

#include "emulate.h"

int main(int argc, char **argv) {

    // Check that [argv] is valid, i.e., has 2-3 args.
    if (argc < 2 || argc > 3) return EXIT_FAILURE;

    // Initialise registers and memory.
    Registers_s registersStruct = createRegs();
    Registers registers = &registersStruct;
    Memory memory = allocMemFromFile(argv[1]);

    // Fetch first instruction
    Instruction instruction = readMem(memory, false, getRegPC(registers));

    // Fetch, decode, execute cycle while the program has not terminated
    while (instruction != HALT) {
        execute(&instruction, registers, memory);
    }

    // Dump contents of register and memory, then free memory.
    FILE *fileOut = stdout;
    if (argc == 3) fileOut = fopen(argv[2], "w");

    dumpRegs(registers, fileOut);
    dumpMem(memory, fileOut);
    freeMem(memory);

    fclose(fileOut);

    return EXIT_SUCCESS;
}
