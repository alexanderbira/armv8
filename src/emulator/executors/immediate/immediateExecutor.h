///
/// immediateExecutor.h
/// Delegates execution of an immediate instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_IMMEDIATE_EXECUTOR_H
#define EMULATOR_IMMEDIATE_EXECUTOR_H

#include "../../../common/const.h"
#include "../../../common/error.h"
#include "../../../common/ir/ir.h"
#include "../../system/memory.h"
#include "../../system/registers.h"
#include "arithmeticImmediateExecutor.h"
#include "wideMoveExecutor.h"

void executeImmediate(IR *irObject, Registers regs, unused Memory mem);

#endif // EMULATOR_IMMEDIATE_EXECUTOR_H
