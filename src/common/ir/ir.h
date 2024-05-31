///
/// ir.h
/// The intermediate representation of parsed instructions.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef COMMON_IR_H
#define COMMON_IR_H

#include "imm.h"
#include "reg.h"
#include "ls.h"
#include "branch.h"

/// The type of instruction represented.
typedef enum {
    DPImm,  ///< Data Processing (Immediate)
    DPReg,  ///< Data Processing (Register)
    LS,     ///< Load and Store
    Br,     ///< Branch
} IRType;

typedef struct {
    IRType type;
    union {
        Imm_IR imm;
        Reg_IR reg;
        LS_IR ls;
        Branch_IR branch;
    } repr;
} IR;

#endif //COMMON_IR_H
