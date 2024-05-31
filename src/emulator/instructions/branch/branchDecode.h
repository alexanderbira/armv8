///
/// loadStoreDecode.c
/// Decode a branch instruction to its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 30/05/2024.
///

#ifndef BRANCHDECODE_H
#define BRANCHDECODE_H

#include "../mask.h"
#include "../../../common/ir/ir.h"

/// Branch unconditional mask - used for decomposing on the bits relevant to
/// checking if an unconditional branch instruction is well-formed
#define B_UCOND_M BitMask(31, 26)

/// Branch unconditional code - the bit-pattern a branch instruction needs
/// in order to be an unconditional branch
#define B_UCOND_C B(000101)

/// Branch simm26 mask - used for extracting simm26 from an unconditional branch
/// instruction
#define B_SIMM26_M BitMask(25, 0)


/// Branch register upper mask - used for decomposing on the bits relevant to
/// checking if the upper portion of a register branch instruction is well-
/// formed
#define B_REG_U_M BitMask(31, 10)

/// Branch register lower mask - used for decomposing on the bits relevant to
/// checking if the lower portion of a register branch instruction is well-
/// formed
#define B_REG_L_M BitMask(4, 0)

/// Branch register upper code - the upper bit-pattern a branch instruction
/// needs in order to be a register branch
#define B_REG_U_C B(1101011000011111000000)

/// Branch register lower code - the lower bit-pattern a branch instruction
/// needs in order to be a register branch
#define B_REG_L_C B(00000)

/// Branch xn mask - used for extracting xn from a register branch instruction
#define B_XN_M BitMask(9, 5)


/// Branch conditional upper mask - used for decomposing on the bits relevant to
/// checking if the upper portion of a conditional branch instruction is well-
/// formed
#define B_COND_U_M BitMask(31, 24)

/// Branch conditional lower mask - used for decomposing on the bits relevant to
/// checking if the lower portion of a conditional branch instruction is well-
/// formed
#define B_COND_L_M BitMask(4, 4)

/// Branch conditional upper code - the upper bit-pattern a branch instruction
/// needs in order to be a conditional branch
#define B_COND_U_C B(01010100)

/// Branch conditional lower code - the lower bit-pattern a branch instruction
/// needs in order to be a conditional branch
#define B_COND_L_C B(0)

/// Branch simm19 mask - used for extracting simm19 from a conditional branch
/// instruction
#define B_SIMM19_M BitMask(23, 5)

/// Branch cond mask - used for extracting cond from a conditional branch
/// instruction
#define B_COND_M BitMask(3, 0)


IR decodeBranch(Instruction word);

#endif //BRANCHDECODE_H
