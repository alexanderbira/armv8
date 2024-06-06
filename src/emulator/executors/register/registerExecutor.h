///
/// registerExecutor.h
/// Delegates execution of a register instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_REGISTER_EXECUTOR_H
#define EMULATOR_REGISTER_EXECUTOR_H

#include "../../../common/const.h"
#include "../../../common/error.h"
#include "../../../common/ir/ir.h"
#include "../../system/registers.h"
#include "../../system/memory.h"
#include "arithmeticRegisterExecutor.h"
#include "bitLogicExecutor.h"
#include "multiplyExecutor.h"

void executeRegister(IR *irObject, Registers registers, unused Memory memory);

#endif // EMULATOR_REGISTER_EXECUTOR_H
