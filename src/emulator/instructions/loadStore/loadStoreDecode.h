///
/// loadStoreDecode.h
/// Decode a load and store instruction to its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 27/05/2024.
///

#ifndef LOADSTOREDECODE_H
#define LOADSTOREDECODE_H

#include "../mask.h"
#include "../../../common/ir/ir.h"

/// Single data transfer mask - used for decomposing on the bits relevant to
/// checking if a single data transfer instruction is well-formed
#define SDT_M BitMask(31, 23)

/// Single data transfer well-formed mask - used for extracting bits relevant
/// to checking if a single data transfer instruction is well-formed
#define SDT_WF_M B(101111111)

/// Single data transfer code - the bit-pattern a single data transfer
/// instruction needs to be well-formed
#define SDT_C B(101110010)


/// Load literal mask - used for decomposing on the bits relevant to checking if
/// a load literal instruction is well-formed
#define LL_M BitMask(31, 24)

/// Load literal well-formed mask - used for extracting bits relevant
/// to checking if a load literal instruction is well-formed
#define LL_WF_M B(10111111)

/// Load literal code - the bit-pattern a load literal instruction needs to be
/// well-formed
#define LL_C B(00011000)


/// Load store SF mask - used to decompose on SF
#define LS_SF_M BitMask(30, 30)

/// Load store target register mask - used to decompose on the target register
#define LS_RT_M BitMask(4, 0)


/// Load literal simm19 mask - used to decompose on simm19
#define LL_SIMM19_M BitMask(23, 5)

/// Single data transfer U mask - used to decompose on U
#define SDT_U_M BitMask(24, 24)

/// Single data transfer L mask - used to decompose on U
#define SDT_L_M BitMask(22, 22)

/// Single data transfer offset mask - used to decompose on offset
#define SDT_OFFSET_M BitMask(21, 10)

/// Single data transfer xn mask - used to decompose on xn
#define SDT_XN_M BitMask(9, 5)


/// Single data transfer register offset mask - used for extracting offset bits
/// relevant to checking if the instruction uses the register offset addressing
/// mode
#define SDT_RO_M B(100000111111)

/// Single data transfer register offset code - the bit-pattern an offset needs
/// in order to use the register offset addressing mode
#define SDT_RO_C B(100000011010)

/// Single data transfer pre/post-index mask - used for extracting offset bits
/// relevant to checking if the instruction uses the pre/post-index addressing
/// mode
#define SDT_PPI_M B(100000000001)

/// Single data transfer pre/post-index code - the bit-pattern an offset needs
/// in order to use the pre/post-index addressing mode
#define SDT_PPI_C B(000000000001)


/// Single data transfer register offset xm mask - used for extracting xm from
/// the offset in the register offset addressing mode
#define SDT_RO_XM_M BitMask(10, 6)

/// Single data transfer pre/post-index simm9 mask - used for extracting simm9
/// from the offset in the pre/post-index addressing mode
#define SDT_PPI_SIMM9_M BitMask(10, 2)

/// Single data transfer pre/post-index I mask - used for extracting I from the
/// offset in the pre/post-index addressing mode
#define SDT_PPI_I_M BitMask(1, 1)


IR decodeLoadStore(Instruction word);

#endif //LOADSTOREDECODE_H
