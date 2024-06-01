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
    Registers registers = &regs_s;
    Memory memory = allocMemFromFile(argv[1]);

    while (true) {
        BitData pcVal = getRegPC(registers);
        BitData instruction = readMem(memory, false, pcVal);

        // Catch halt instruction.
        if (instruction == HALT_INSTR_C) break;

        // Decode and execute.
        IR ir = getDecodeFunction(instruction)(instruction);
        getExecuteFunction(&ir)(&ir, registers, memory);

        //Increment PC only when no branch or jump instructions applied.
        if (pcVal == getRegPC(registers)) incRegPC(registers);
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
