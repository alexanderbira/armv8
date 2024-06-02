///
/// handler.h
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_HANDLER_H
#define ASSEMBLER_HANDLER_H

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../common/const.h"
#include "../common/ir/ir.h"
#include "helpers.h"
#include "state.h"
#include "handlers/instructionHandler.h"

IR handleDirective(const char *line, AssemblerState *state);

void handleLabel(const char *line, AssemblerState *state);

IR handleInstruction(const char *line, AssemblerState *state);

#endif // ASSEMBLER_HANDLER_H
