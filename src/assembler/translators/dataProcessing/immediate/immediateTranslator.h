///
/// immediateTranslator.h
/// Transform a [IR] of a Data Processing (Immediate) instruction to a binary instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_IMMEDIATE_TRANSLATOR_H
#define ASSEMBLER_IMMEDIATE_TRANSLATOR_H

#include "../../../../common/const.h"
#include "../../../../common/error.h"
#include "../../../../common/ir/ir.h"
#include "../../../helpers.h"
#include "../../../state.h"

Instruction translateImmediate(IR *irObject, unused AssemblerState *state);

#endif // ASSEMBLER_IMMEDIATE_TRANSLATOR_H
