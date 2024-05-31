///
/// branchHandler.h
/// Functions to parse from assembly and write as binary a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_BRANCH_HANDLER_H
#define ASSEMBLER_BRANCH_HANDLER_H

#include <string.h>

#include "../../common/error.h"
#include "../../common/ir/ir.h"
#include "../helpers.h"

IR parseBranch(TokenisedLine line, AssemblerState *state);

BitInst translateBranch(IR irObject, AssemblerState *state);

#endif // ASSEMBLER_BRANCH_HANDLER_H
