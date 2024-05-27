///
/// op0.h
/// Masks and codes for op0
///
/// Created by Alexander Biraben-Renard and Billy Highley on 27/05/2024.
///

#ifndef OP0_H
#define OP0_H

#include "mask.h"

/// OP0 Mask
#define OP0_M bitMask(28, 25)

/// OP0 Immediate Mask
#define OP0_IMM_M b(1110)
/// OP0 Register Mask
#define OP0_REG_M b(0111)
/// OP0 Load/Store Mask
#define OP0_LS_M  b(0101)
/// OP0 Branch Mask
#define OP0_BR_M  b(1110)

/// OP0 Immediate Code
#define OP0_IMM_C b(1000)
/// OP0 Register Code
#define OP0_REG_C b(0101)
/// OP0 Load/Store Code
#define OP0_LS_C  b(0100)
/// OP0 Branch Code
#define OP0_BR_C  b(1010)

#endif //OP0_H
