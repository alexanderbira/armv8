///
/// masks.h
/// Masks, types, and mask functions
///
/// Created by Alexander Biraben-Renard and Billy Highley on 27/05/2024.
///

#ifndef MASKS_H
#define MASKS_H

#include <stdint.h>

#define b(x) B(#x)

#define OP0_MASK bitMask(28, 25)

#define DP_IMM_OP0_MASK     b(1110)
#define DP_REG_OP0_MASK     b(0111)
#define LOAD_STORE_OP0_MASK b(0101)
#define BRANCH_OP0_MASK     b(1110)

#define DP_IMM_OP0_CODE     b(1000)
#define DP_REG_OP0_CODE     b(0101)
#define LOAD_STORE_OP0_CODE b(0100)
#define BRANCH_OP0_CODE     b(1010)

typedef uint32_t Mask;

Mask bitMask(int, int);

#endif //MASKS_H
