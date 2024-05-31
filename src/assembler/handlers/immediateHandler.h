///
/// immediateHandler.h
/// Functions to parse from assembly and write as binary a Data Processing (Immediate) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_IMMEDIATE_HANDLER_H
#define ASSEMBLER_IMMEDIATE_HANDLER_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/const.h"
#include "../../common/error.h"
#include "../../common/ir/ir.h"
#include "../helpers.h"
#include "../state.h"

IR parseImmediate(TokenisedLine *line, unused AssemblerState *state);

BitInst translateImmediate(IR *irObject, unused AssemblerState *state);

#endif // ASSEMBLER_IMMEDIATE_HANDLER_H
