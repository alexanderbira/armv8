///
/// assemble.c
/// Assembles an AArch64 assembly source file.
///
/// Created by Konstantinos Gkoutzis on 23/05/24.
///

#include "assemble.h"

/// The entrypoint to the assembler program.
/// @param argc Number of arguments. Should be 3.
/// @param argv Arguments. In order: executable name, assembly in, and object code out.
/// @return Program exit code.
/// @example \code ./assemble code.s code.o \endcode
int main(int argc, char **argv) {
    // Check that [argv] is valid, i.e., has 3 args.
    if (argc != 3) {
        printf("Usage: ./assemble code.s out.bin\n");
        return EXIT_FAILURE;
    };

    // First pass, populate program [state] and generate [IR]s.
    FILE *fileIn = fopen(argv[1], "r");
    AssemblerState state = createState();
    char line[256];

    while (fgets(line, sizeof(line), fileIn)) {
        parse(line, &state);
    }

    fclose(fileIn);

    // Second pass, translate IRs to binary instruction based on [state].
    FILE *fileOut = fopen(argv[2], "wb");

    // Reset current address - jump offset calculations rely on this.
    state.address = 0x0;

    for (size_t i = 0; i < state.irCount; i++) {
        IR ir = state.irList[i];
        Instruction instruction = getTranslator(&ir.type)(&ir, &state);
        state.address += 0x4;
        fwrite(&instruction, sizeof(Instruction), 1, fileOut);
    }

    destroyState(state);
    fclose(fileOut);

    return EXIT_SUCCESS;
}
