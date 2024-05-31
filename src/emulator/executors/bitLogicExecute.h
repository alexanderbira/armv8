///
/// bitLogicExecute.h
/// Executes a bit-logic instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef BITLOGICEXECUTE_H
#define BITLOGICEXECUTE_H

#include "../mask.h"
#include "../instructions.h"
#include "../system/registers.h"
#include "../system/memory.h"
#include "../../common/ir/ir.h"
#include "bitwiseShifts.h"

void bitLogicExecute(Reg_IR regIR, Registers regs);

#endif //BITLOGICEXECUTE_H
