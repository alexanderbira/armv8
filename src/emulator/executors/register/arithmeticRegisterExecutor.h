///
/// arithmeticRegExecute.h
/// Executes an arithmetic (register) instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef ARITHMETIC_REGISTER_EXECUTOR_H
#define ARITHMETIC_REGISTER_EXECUTOR_H

#include "../../../common/ir/register.h"
#include "../../system/registers.h"
#include "../conditions.h"
#include "bitwiseShifts.h"

void arithmeticRegisterExecute(Register_IR *registerIR, Registers regs);

#endif // ARITHMETIC_REGISTER_EXECUTOR_H
