///
/// loadStoreExecute.h
/// Execute a load and store instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#ifndef LOADSTOREEXECUTE_H
#define LOADSTOREEXECUTE_H

#include "../../common/ir/ir.h"
#include "../system/registers.h"
#include "../system/memory.h"

void executeLoadStore(LS_IR lsIr, Registers regs, Memory mem);

#endif //LOADSTOREEXECUTE_H
