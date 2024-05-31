///
/// immediateDecode.h
/// Decodes an immediate instruction into its IR
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef IMMEDIATE_H
#define IMMEDIATE_H

#include "../instructions.h"
#include "../mask.h"
#include "../../system/registers.h"
#include "../../system/memory.h"
#include "../../../common/ir/ir.h"

/// sf (bit-width specifier) mask
#define IMM_SF_M  BitMask(31, 31)
/// opc (operation code) mask
#define IMM_OPC_M BitMask(30, 29)
/// opi (data process type) mask
#define IMM_OPI_M BitMask(25, 23)
/// rd (destination register) mask
#define IMM_RD_M  BitMask(4, 0)

// Arithmetic
/// sh mask
#define IMM_SH_M BitMask(22,22)
/// imm12 mask
#define IMM_12_M BitMask(21, 10)
/// rn mask
#define IMM_RN_M BitMask(9, 5)

// Wide move
/// hw mask
#define IMM_HW_M  BitMask(22, 21)
/// imm16 mask
#define IMM_16_M  BitMask(20, 5)




IR decodeImmediate(Instruction word);

#endif //IMMEDIATE_H