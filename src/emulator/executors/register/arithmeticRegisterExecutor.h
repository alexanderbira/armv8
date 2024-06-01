///
/// arithmeticRegExecute.h
/// Executes an arithmetic (register) instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef ARITHMETICREGEXECUTE_H
#define ARITHMETICREGEXECUTE_H

#include "../../../common/ir/register.h"
#include "../../system/registers.h"
#include "bitwiseShifts.h"

void arithmeticRegisterExecute(Register_IR *registerIR, Registers regs);

#endif //ARITHMETICREGEXECUTE_H
