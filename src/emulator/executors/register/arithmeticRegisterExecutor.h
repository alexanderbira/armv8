///
/// arithmeticRegExecute.h
/// Executes an arithmetic (register) instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef EMULATOR_ARITHMETIC_REGISTER_EXECUTOR_H
#define EMULATOR_ARITHMETIC_REGISTER_EXECUTOR_H

#include "bitwiseShifts.h"
#include "conditions.h"
#include "register.h"
#include "registers.h"

void arithmeticRegisterExecute(Register_IR *registerIR, Registers registers);

#endif // EMULATOR_ARITHMETIC_REGISTER_EXECUTOR_H
