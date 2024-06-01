///
/// multiplyExecutor.h
/// Executes a multiply instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef EMULATOR_MULTIPLY_EXECUTOR_H
#define EMULATOR_MULTIPLY_EXECUTOR_H

#include "../../../common/ir/register.h"
#include "../../system/registers.h"

void multiplyExecute(Register_IR *registerIR, Registers regs);

#endif // EMULATOR_MULTIPLY_EXECUTOR_H
