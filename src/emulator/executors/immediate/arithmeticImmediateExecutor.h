///
/// arithmeticImmediateExecutor.h
/// Executes an arithmetic (immediate) instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_ARITHMETIC_IMMEDIATE_EXECUTOR_H
#define EMULATOR_ARITHMETIC_IMMEDIATE_EXECUTOR_H

#include "../../../common/const.h"
#include "../../../common/error.h"
#include "../../../common/ir/ir.h"
#include "../../system/registers.h"
#include "../../system/memory.h"

void arithmeticExecute(Immediate_IR *immediateIR, Registers registers);

#endif // EMULATOR_ARITHMETIC_IMMEDIATE_EXECUTOR_H
