///
/// loadStoreHandler.h
/// Transform a [IR] of a Load-Store instruction to a binary instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_LOAD_STORE_TRANSLATOR_H
#define ASSEMBLER_LOAD_STORE_TRANSLATOR_H

#include <string.h>

#include "../../../common/const.h"
#include "../../../common/error.h"
#include "../../../common/ir/ir.h"
#include "../../helpers.h"
#include "../../state.h"

Instruction translateLoadStore(IR *irObject, AssemblerState *state);

#endif // ASSEMBLER_LOAD_STORE_TRANSLATOR_H
