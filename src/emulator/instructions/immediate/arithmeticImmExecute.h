///
/// arithmeticImmExecute.h
/// Executes an arithmetic (immediate) instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef ARITHMETICIMMEXECUTE_H
#define ARITHMETICIMMEXECUTE_H

#include "../mask.h"
#include "../instructions.h"
#include "../../system/registers.h"
#include "../../system/memory.h"
#include "../../../common/ir/ir.h"

/// sh mask
#define IMM_SH_M BitMask(22,22)
/// imm12 mask
#define IMM_12_M BitMask(21, 10)
/// rn mask
#define IMM_RN_M BitMask(9, 5)

void arithmeticImmExecute(Imm_IR ir, Registers regs);

#endif //ARITHMETICIMMEXECUTE_H