///
/// wideMoveExecute.h
/// Executes a wide move instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef WIDEMOVE_H
#define WIDEMOVE_H

#include "../mask.h"
#include "../instructions.h"
#include "../system/registers.h"
#include "../system/memory.h"
#include "../../common/ir/ir.h"

void wideMoveExecute(Imm_IR ir, Registers regs);

#endif //WIDEMOVE_H