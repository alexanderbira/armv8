///
/// assemble.c
/// The entrypoint to the assembler program.
///
/// Created by ??? on ???.
///

#include "assemble.h"

int main(int argc, char **argv) {
    // Check that [argv] is valid, i.e., has 3 args.
    if (argc != 3) return EXIT_FAILURE;

    FILE *fileIn = fopen(argv[1], "r");
    char line[256];

    // TODO: Handle line too long... is it possible?
    while (fgets(line, sizeof(line), fileIn)) {
        // Note, line is trimmed by classification.
        AsmType type = classify(line);


    }

    return EXIT_SUCCESS;
}
