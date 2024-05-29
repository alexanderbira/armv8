///
/// ir.h
/// The intermediate representation of parsed instructions.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_IR_H
#define ASSEMBLER_IR_H

#include "imm.h"

/// The type of instruction represented.
typedef enum {
    DPImm, ///< Data Processing (Immediate)
    DPReg, ///< Data Processing (Register)
    SDT,   ///< Single Data Transfer
    LL,    ///< Load Literal
    B,     ///< Branch
} IRType;

typedef struct {
    IRType type;
    union {
        Imm_IR imm;
    } repr;
} IR;

#endif //ASSEMBLER_IR_H
