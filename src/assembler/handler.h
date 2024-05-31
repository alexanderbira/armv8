///
/// handler.h
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_HANDLER_H
#define ASSEMBLER_HANDLER_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../common/const.h"
#include "../common/ir/ir.h"
#include "insts/instructionHandler.h"
#include "insts/immediateHandler.h"
#include "insts/branchHandler.h"
#include "helpers.h"

void handleDirective(const char *line, AssemblerState *state);

void handleLabel(const char *line, AssemblerState *state);

IR handleInstruction(const char *line, AssemblerState *state);

#endif // ASSEMBLER_HANDLER_H
