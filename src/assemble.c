///
/// assemble.c
/// The entrypoint to the assembler program.
///
/// Created by ??? on ???.
///

#include "assemble.h"

/// The entrypoint to the assembler program.
/// @param argc Number of arguments. Should be 3.
/// @param argv Arguments. Should contain program name, assembly input, and object code out.
/// @return Program exit code.
/// @example \code ./assemble code.s code.o \endcode
int main(int argc, char **argv) {
    // Check that [argv] is valid, i.e., has 3 args.
    if (argc != 3) return EXIT_FAILURE;

    FILE *fileIn = fopen(argv[1], "r");
    FILE *fileOut = fopen(argv[2], "wb");
    char line[256];
    BitAddr pc = 0x0;

    // TODO: Handle line too long... is it possible?
    while (fgets(line, sizeof(line), fileIn)) {
        // Note, line is trimmed by classification.
        AsmType type = classify(line);
        BitInst inst = procTable[type](line);

        // Write resulting instruction (if any) to file.
        if (!inst) continue;
        fwrite(&inst, sizeof(BitInst), 1, fileOut);
    }

    return EXIT_SUCCESS;
}

