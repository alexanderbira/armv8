///
/// arithmeticImmediateExecutor.h
/// Executes an arithmetic (immediate) instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_ARITHMETIC_IMMEDIATE_EXECUTOR_H
#define EMULATOR_ARITHMETIC_IMMEDIATE_EXECUTOR_H

#include "conditions.h"
#include "const.h"
#include "error.h"
#include "ir.h"
#include "memory.h"
#include "registers.h"

void arithmeticExecute(Immediate_IR *immediateIR, Registers registers);

#endif // EMULATOR_ARITHMETIC_IMMEDIATE_EXECUTOR_H
