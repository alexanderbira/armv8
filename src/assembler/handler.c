///
/// handler.c
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#include "handler.h"

/// Function to process a directive, i.e. a line that begins with '.'.
/// @param line The line to "process".
/// @return The resulting binary word.
/// @pre The incoming [line] is a directive, and is trimmed, i.e. ".<directive> <value>".
IR handleDirective(TokenisedLine *tokenisedLine, unused AssemblerState *state) {
    assertFatal(tokenisedLine->operandCount == 1,
                "[handleDirective] Invalid number of arguments!");

    if (!strcasecmp(tokenisedLine->subMnemonic, "int")) {
        // Reserve space for line, null terminator, and prepended '#'.
        char *immediateStr = (char *) malloc(strlen(tokenisedLine->operands[0]) + 2);
        *immediateStr = '#';
        strcpy(immediateStr + 1, tokenisedLine->operands[0]);

        // Very cheesy trick to reuse [parseImmediateStr].
        BitData immediate = parseImmediateStr(immediateStr, 8 * sizeof(BitData));
        free(immediateStr);
        return (IR) {.type = CONSTANT, .ir.memoryData = immediate};
    } else {
        throwFatal("[handleDirective] Invalid directive!");
    }
}

/// Function to process a label, i.e. a line of alphabet characters ending in ':'.
/// @param line The line to "process".
/// @return The resulting binary word.
/// @pre The incoming [line] is a label and is trimmed.
void handleLabel(const char *line, AssemblerState *state) {
    char *label = strdup(line);
    *strchr(label, ':') = '\0';

    addMapping(state, label, state->address);
}

/// Function to process an instruction.
/// @param line The line to "process".
/// @return The resulting binary word.
IR handleInstruction(TokenisedLine *tokenisedLine, AssemblerState *state) {
    IR result = getParseFunction(tokenisedLine->mnemonic)(tokenisedLine, state);

    // Always increment address by 4 since at compile time.
    state->address += 0x4;
    return result;
}
