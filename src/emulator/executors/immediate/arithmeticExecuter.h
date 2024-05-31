///
/// arithmeticExecuter.h
/// Executes an arithmetic (immediate) instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_ARITHMETIC_EXECUTER_H
#define EMULATOR_ARITHMETIC_EXECUTER_H

#include "../../../common/ir/ir.h"
#include "../../system/memory.h"
#include "../../system/registers.h"

void arithmeticExecute(Immediate_IR *immediateIR, Registers regs);

#endif // EMULATOR_ARITHMETIC_EXECUTER_H