///
/// handler.h
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_HANDLER_H
#define ASSEMBLER_HANDLER_H

#include <stdint.h>

#include "../common/const.h"
#include "../common/ir/ir.h"

/// Struct representing the current state of the assembler.
typedef struct {
    /// The address of the current instruction being handled.
    BitData address;

    // TODO: Add map (or similar) for labels and addresses.
} AssemblerState;

AssemblerState createState(void);

IR handleDirective(const char *line, const AssemblerState state);

IR handleLabel(const char *line, const AssemblerState state);

IR handleInstruction(const char *line, const AssemblerState state);

#endif //ASSEMBLER_HANDLER_H
