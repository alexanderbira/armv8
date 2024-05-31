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

    while (true) {
        // Fetch the instruction pointed to by the PC
        BitData pcVal = getRegPC(regs);
        BitData instruction = readMem(mem, true, pcVal);

        // Halt the execution if the instruction is a halt
        if (instruction == HALT_INSTR_C) break;

        // Decode and execute the instruction
        IR ir = decode(instruction);
        execute(ir, regs, mem);

        //Increment PC only when no branch or jump instructions applied.
        if (ir.type != B) incRegPC(regs);
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
