///
/// execute.h
/// Execute an instruction from its intermediate representation
///
/// Created by Billy Highley and Alexander Biraben-Renard on 29/05/2024.
///

#ifndef EXECUTE_H
#define EXECUTE_H

#include "../../common/ir/ir.h"
#include "../system/registers.h"
#include "../system/memory.h"

void execute(IR ir, Registers regs, Memory mem);

#endif //EXECUTE_H
