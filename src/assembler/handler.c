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
IR handleDirective(unused const char *line, unused AssemblerState *state) {
    TokenisedLine tokenisedLine = tokenise(line);

    assertFatal(tokenisedLine.operandCount == 1, "[handleDirective] Invalid number of arguments!");
    char *directive = tokenisedLine.mnemonic + 1;

    if (!strcasecmp(directive, "int")) {
        BitData immediate;
        assertFatal(sscanf("%" SCNx32, tokenisedLine.operands[1], &immediate),
                    "[handleDirective] Failed to parse immediate for `.int`!");
        destroyTokenisedLine(tokenisedLine);
        return (IR) {.type = CONSTANT, .ir.memoryData = immediate};
    } else {
        throwFatal("[handleDirective] Invalid directive!");
    }
}

/// Function to process a label, i.e. a line of alphabet characters ending in ':'.
/// @param line The line to "process".
/// @return The resulting binary word.
/// @pre The incoming [line] is a label,  i.e. a line of alphabet characters ending in ':', and is trimmed.
void handleLabel(const char *line, AssemblerState *state) {
    // Remove trailing colon. See precondition.
    char *label = strdup(line);
    *(label + strlen(label) - 1) = '\0';

    addMapping(state, label, state->address);
}

/// Function to process an instruction.
/// @param line The line to "process".
/// @return The resulting binary word.
IR handleInstruction(const char *line, AssemblerState *state) {
    TokenisedLine tokenisedLine = tokenise(line);
    IR result = getParseFunction(tokenisedLine.mnemonic)(&tokenisedLine, state);
    destroyTokenisedLine(tokenisedLine);

    // Always increment address by 4 since at compile time,
    // jumps are not possible.
    state->address += 0x4;
    return result;
}
