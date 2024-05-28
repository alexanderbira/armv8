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
    char line[256];

    // TODO: Handle line too long... is it possible?
    while (fgets(line, sizeof(line), fileIn)) {
        // Note, line is trimmed by classification.
        AsmType type = classify(line);

        // TODO: Call corresponding function.
        // uint32_t inst = procTable[type](line);

        // TODO: Write result to file.
    }

    return EXIT_SUCCESS;
}

