///
/// branchExecuter.h
/// Execute a branch instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 31/05/2024.
///

#ifndef EMULATOR_BRANCH_EXECUTOR_H
#define EMULATOR_BRANCH_EXECUTOR_H

#include "../../common/error.h"
#include "../../common/ir/ir.h"
#include "../system/registers.h"
#include "../system/memory.h"

void executeBranch(IR *irObject, Registers regs, Memory mem);

#endif // EMULATOR_BRANCH_EXECUTOR_H
