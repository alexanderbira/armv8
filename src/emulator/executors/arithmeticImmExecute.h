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
#include "../system/registers.h"
#include "../system/memory.h"
#include "../../common/ir/ir.h"

void arithmeticImmExecute(Imm_IR ir, Registers regs);

#endif //ARITHMETICIMMEXECUTE_H