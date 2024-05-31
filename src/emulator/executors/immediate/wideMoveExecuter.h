///
/// wideMoveExecuter.h
/// Executes a wide move instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_WIDE_MOVE_EXECUTOR_H
#define EMULATOR_WIDE_MOVE_EXECUTOR_H

#include "../../../common/ir/ir.h"
#include "../../system/memory.h"
#include "../../system/registers.h"

void wideMoveExecute(Immediate_IR *immediateIR, Registers regs);

#endif // EMULATOR_WIDE_MOVE_EXECUTOR_H