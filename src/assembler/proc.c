///
/// proc.c
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#include "proc.h"

/// Function to "process" an empty line.
/// @param line The line to "process".
/// @return Always 0, since no instruction is ever all 0s.
BitInst procEmpty(const char *line) { return 0; }

/// Function to process a directive, i.e. a line that begins with '.'.
/// @param line The line to "process".
/// @return The resulting binary word.
BitInst procDirective(const char *line) {
    // TODO: Implement
    return 0;
}

/// Function to process a label, i.e. a line of alphabet characters ending in ':'.
/// @param line The line to "process".
/// @return The resulting binary word.
BitInst procLabel(const char *line) {
    // TODO: Implement
    return 0;
}

/// Function to process an instruction.
/// @param line The line to "process".
/// @return The resulting binary word.
BitInst procInst(const char *line) {
    // TODO: Implement
    return 0;
}
