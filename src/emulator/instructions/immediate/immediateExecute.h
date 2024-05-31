///
/// immediateExecute.h
/// Delegates execution of an immediate instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef IMMEDIATEEXECUTE_H
#define IMMEDIATEEXECUTE_H

#include "../../../common/ir/ir.h"
#include "../../system/registers.h"
#include "../instructions.h"
#include "arithmeticImmExecute.h"
#include "wideMoveExecute.h"

void executeImmediate(Imm_IR immIR, Registers regs);

#endif //IMMEDIATEEXECUTE_H
