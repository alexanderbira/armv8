///
/// arithmeticExecute.h
/// Executes an arithmetic instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef ARITHMETIC_H
#define ARITHMETIC_H

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

void arithmeticExecute(Imm_IR ir, Registers regs);

#endif //ARITHMETIC_H