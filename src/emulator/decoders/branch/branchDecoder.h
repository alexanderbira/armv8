///
/// branchDecoder.h
/// Decodes a binary word of a branch instruction to its [IR].
///
/// Created by Alexander Biraben-Renard on 30/05/2024.
///

#ifndef EMULATOR_BRANCH_DECODER_H
#define EMULATOR_BRANCH_DECODER_H

#include "../../../common/const.h"
#include "../../../common/error.h"
#include "../../../common/ir/ir.h"

IR decodeBranch(Instruction word);

#endif // EMULATOR_BRANCH_DECODER_H
