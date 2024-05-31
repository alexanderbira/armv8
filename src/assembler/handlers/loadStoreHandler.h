///
/// loadStoreHandler.h
/// Functions to parse from assembly and write as binary a Load Store instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_LOAD_STORE_H
#define ASSEMBLER_LOAD_STORE_H

#include <string.h>

#include "../../common/const.h"
#include "../../common/error.h"
#include "../../common/ir/ir.h"
#include "../helpers.h"
#include "../state.h"

IR parseLoadStore(TokenisedLine *line, unused AssemblerState *state);

BitInst translateLoadStore(IR *irObject, AssemblerState *state);

#endif // ASSEMBLER_LOAD_STORE_H
