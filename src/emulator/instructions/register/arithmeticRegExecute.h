///
/// arithmeticRegExecute.h
/// Executes an arithmetic (register) instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef ARITHMETICREGEXECUTE_H
#define ARITHMETICREGEXECUTE_H

#include "../mask.h"
#include "../instructions.h"
#include "../../system/registers.h"
#include "../../system/memory.h"
#include "../../../common/ir/ir.h"
#include "bitwiseShifts.h"

void arithmeticRegExecute(Reg_IR regIR, Registers regs);

#endif //ARITHMETICREGEXECUTE_H
