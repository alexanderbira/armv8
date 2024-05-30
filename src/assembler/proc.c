///
/// proc.c
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#include "proc.h"

/// Creates a fresh [AssemblerState].
/// @return A fresh [AssemblerState].
AssemblerState createState(void) {
    AssemblerState state;
    state.address = 0x0;
}

/// Function to process a directive, i.e. a line that begins with '.'.
/// @param line The line to "process".
/// @return The resulting binary word.
IR handleDirective(const char *line, const AssemblerState state) {
    // TODO: Implement
    return 0;
}

/// Function to process a label, i.e. a line of alphabet characters ending in ':'.
/// @param line The line to "process".
/// @return The resulting binary word.
IR handleLabel(const char *line, const AssemblerState state) {
    // TODO: Implement
    return 0;
}

/// Function to process an instruction.
/// @param line The line to "process".
/// @return The resulting binary word.
IR handleInstruction(const char *line, const AssemblerState state) {
    // TODO: Implement
    return 0;
}
