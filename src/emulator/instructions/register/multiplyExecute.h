///
/// multiplyExecute.h
/// Executes a multiply instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef MULTIPLYEXECUTE_H
#define MULTIPLYEXECUTE_H

#include "../mask.h"
#include "../instructions.h"
#include "../../system/registers.h"
#include "../../system/memory.h"
#include "../../../common/ir/ir.h"

void multiplyExecute(Reg_IR regIR, Registers regs);

#endif //MULTIPLYEXECUTE_H
