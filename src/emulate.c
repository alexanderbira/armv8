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
    Registers_s registersStruct = createRegs();
    Registers registers = &registersStruct;
    Memory memory = allocMemFromFile(argv[1]);

    // Fetch first instruction
    BitData pcVal = getRegPC(registers);
    Instruction instruction = readMem(memory, false, pcVal);

    // Fetch, decode, execute cycle while the program has not terminated
    while (instruction != HALT) {

        // Decode and execute.
        IR ir = getDecodeFunction(instruction)(instruction);
        getExecuteFunction(&ir)(&ir, registers, memory);

        // Increment PC only when no branch or jump instructions applied.
        if (pcVal == getRegPC(registers)) incRegPC(registers);

        // Fetch next instruction
        pcVal = getRegPC(registers);
        instruction = readMem(memory, false, pcVal);

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
