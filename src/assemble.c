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
    AssemblerState state = createState();
    char line[256];

    while (fgets(line, sizeof(line), fileIn)) {
        char *trimmedLine = trim(line, " ");

        // Check if line is blank.
        if (strlen(line) == 0) continue;

        // Check if line is a label.
        char *colon = strchr(line, ':');
        if (colon != NULL) {
            // Ensure all preceding chars are alphabet.
            bool isLabel = true;
            for (char *p = line; p < colon; p++) {
                if (!isalpha((unsigned char) *p)) isLabel = false;
            }

            if (isLabel) handleLabel(trimmedLine, &state);
        }

        // By default, handle either directive or instructions.
        TokenisedLine tokenisedLine = tokenise(line);
        IR ir = (tokenisedLine.mnemonic == NULL)
                ? handleDirective(&tokenisedLine, &state)
                : handleInstruction(&tokenisedLine, &state);
        destroyTokenisedLine(tokenisedLine);
        addIR(&state, ir);
    }

    fclose(fileIn);
    FILE *fileOut = fopen(argv[2], "wb");

    for (size_t i = 0; i < state.irCount; i++) {
        IR ir = state.irList[i];
        Instruction instruction = getTranslateFunction(ir.type)(&ir, &state);
        fwrite(&instruction, sizeof(Instruction), 1, fileOut);
    }

    destroyState(state);
    fclose(fileOut);

    return EXIT_SUCCESS;
}
