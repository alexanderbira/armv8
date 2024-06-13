///
/// immediateExecutor.h
/// Delegates execution of an immediate instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_IMMEDIATE_EXECUTOR_H
#define EMULATOR_IMMEDIATE_EXECUTOR_H

#include "arithmeticImmediateExecutor.h"
#include "const.h"
#include "error.h"
#include "ir.h"
#include "memory.h"
#include "registers.h"
#include "wideMoveExecutor.h"

void executeImmediate(IR *irObject, Registers registers, unused Memory memory);

#endif // EMULATOR_IMMEDIATE_EXECUTOR_H
