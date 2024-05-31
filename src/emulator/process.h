///
/// process.h
/// Midway point to get [IRExecutor]s and [BinaryParser]s.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 29/05/2024.
///

#ifndef EMULATOR_PROCESS_H
#define EMULATOR_PROCESS_H

#include "../common/const.h"
#include "../common/error.h"
#include "../common/ir/ir.h"
#include "decoders/immediateDecoder.h"
#include "decoders/registerDecoder.h"
#include "decoders/loadStoreDecoder.h"
#include "decoders/branchDecoder.h"
#include "system/memory.h"
#include "system/registers.h"

/// OP0 Mask
#define OP0_M            mask(28, 25)

/// The mask for op0 of a Data Processing (Immediate) binary instruction.
#define OP0_IMMEDIATE_M  b(1110)

/// The mask for op0 of a Data Processing (Register) binary instruction.
#define OP0_REGISTER_M   b(0111)

/// The mask for op0 of a Load-Store binary instruction.
#define OP0_LOAD_STORE_M b(0101)

/// The mask for op0 of a Branch binary instruction.
#define OP0_BRANCH_M     b(1110)

/// The code for op0 of a Data Processing (Immediate) binary instruction.
#define OP0_IMMEDIATE_C  b(1000)

/// The code for op0 of a Data Processing (Register) binary instruction.
#define OP0_REGISTER_C   b(0101)

/// The code for op0 of a Load-Store binary instruction.
#define OP0_LOAD_STORE_C b(0100)

/// The code for op0 of a Branch binary instruction.
#define OP0_BRANCH_C     b(1010)

typedef Instruction (*IRExecutor)(IR *irObject, Registers regs, Memory mem);

typedef IR (*BinaryParser)(Instruction instruction);

IRExecutor getExecuteFunction(IR *irObject);

BinaryParser getDecodeFunction(Instruction instruction);

#endif // EMULATOR_PROCESS_H
