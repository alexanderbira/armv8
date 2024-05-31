///
/// registerDecode.h
/// Decodes a register instruction into its IR
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef REGISTERDECODE_H
#define REGISTERDECODE_H

#include "../instructions.h"
#include "../mask.h"
#include "../../system/registers.h"
#include "../../system/memory.h"
#include "../../../common/ir/ir.h"

/// sf (bit-width specifier) mask
#define REG_SF_M  BitMask(31, 31)
/// opc (operation code) mask
#define REG_OPC_M BitMask(30, 29)
/// M mask
#define REG_M_M BitMask(28, 28)
/// opr mask
#define REG_OPR_M BitMask(24, 21)
/// rm mask
#define REG_RM_M  BitMask(20, 16)
/// operand mask
#define REG_OP_M  BitMask(15, 10)
/// rn mask
#define REG_RN_M  BitMask(9, 5)
/// rd mask
#define REG_RD_M  BitMask(4, 0)

// arithmetic group mask
#define REG_AR_G_M B(1001)
// bit-logic group mask
#define REG_BL_G_M B(1000)

// arithmetic code
#define REG_AR_C  B(1000)
// bit-logic code
#define REG_BL_C  B(0000)
// multiply code
#define REG_MU_C  B(1000)

IR decodeRegister(Instruction word);

#endif //REGISTERDECODE_H