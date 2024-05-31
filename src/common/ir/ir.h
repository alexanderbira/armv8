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
#include "sdt.h"
#include "branch.h"
#include "ls.h"
#include "branch.h"

/// The type of instruction represented.
typedef enum {
    IMMEDIATE,            ///< Data Processing (Immediate)
    REGISTER,             ///< Data Processing (Register)
    SINGLE_DATA_TRANSFER, ///< Single Data Transfer
    LOAD_LITERAL,         ///< Load Literal
    BRANCH,               ///< Branch
} IRType;

/// The general IR wrapper for all types of assembly instructions.
typedef struct {
    IRType type;
    union {
        Imm_IR imm;
        Reg_IR reg;
        SDT_IR sdt;
        LS_IR ls;
        Branch_IR branch;
    } repr;
} IR;

#endif //COMMON_IR_H
