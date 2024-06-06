///
/// loadStoreExecutor.h
/// Execute a load and store instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#ifndef EMULATOR_LOAD_STORE_EXECUTOR_H
#define EMULATOR_LOAD_STORE_EXECUTOR_H

#include "../../../common/const.h"
#include "../../../common/error.h"
#include "../../../common/ir/ir.h"
#include "../../system/registers.h"
#include "../../system/memory.h"

void executeLoadStore(IR *irObject, Registers registers, Memory memory);

#endif // EMULATOR_LOAD_STORE_EXECUTOR_H
