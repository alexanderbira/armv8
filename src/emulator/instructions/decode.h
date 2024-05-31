///
/// decode.h
/// Decode an instruction word to its intermediate representation (IR)
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>

#include "mask.h"
#include "../../common/ir/ir.h"

/// OP0 Mask
#define OP0_M BitMask(28, 25)

/// OP0 Immediate Mask
#define OP0_IMM_M B(1110)
/// OP0 Register Mask
#define OP0_REG_M B(0111)
/// OP0 Load/Store Mask
#define OP0_LS_M  B(0101)
/// OP0 Branch Mask
#define OP0_BR_M  B(1110)

/// OP0 Immediate Code
#define OP0_IMM_C B(1000)
/// OP0 Register Code
#define OP0_REG_C B(0101)
/// OP0 Load/Store Code
#define OP0_LS_C  B(0100)
/// OP0 Branch Code
#define OP0_BR_C  B(1010)

#include "immediate/immediateDecode.h"
#include "register/registerDecode.h"
#include "loadStore/loadStoreDecode.h"
#include "branch/branchDecode.h"

IR decode(Instruction word);

#endif //DECODE_H
