///
/// branchExecute.h
/// Execute a branch instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 31/05/2024.
///

#ifndef BRANCHEXECUTE_H
#define BRANCHEXECUTE_H

#include "../../../common/ir/ir.h"
#include "../../system/registers.h"
#include "../../system/memory.h"

void executeBranch(Branch_IR branchIr, Registers regs, Memory mem);

#endif //BRANCHEXECUTE_H
