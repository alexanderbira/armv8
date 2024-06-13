///
/// registerExecutor.h
/// Delegates execution of a register instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_REGISTER_EXECUTOR_H
#define EMULATOR_REGISTER_EXECUTOR_H

#include "arithmeticRegisterExecutor.h"
#include "bitLogicExecutor.h"
#include "const.h"
#include "error.h"
#include "ir.h"
#include "memory.h"
#include "multiplyExecutor.h"
#include "registers.h"

void executeRegister(IR *irObject, Registers registers, unused Memory memory);

#endif // EMULATOR_REGISTER_EXECUTOR_H
