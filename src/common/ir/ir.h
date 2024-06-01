///
/// ir.h
/// The intermediate representation of parsed instructions.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef IR_H
#define IR_H

#include "immediate.h"
#include "register.h"
#include "loadStore.h"
#include "branch.h"

/// The type of instruction represented.
typedef enum {
    IMMEDIATE,            ///< Data Processing (Immediate)
    REGISTER,             ///< Data Processing (Register)
    LOAD_STORE,           ///< Load-Store
    BRANCH,               ///< Branch
} IRType;

/// The general IR wrapper for all types of assembly instructions.
typedef struct {
    /// The type of IR stored in [ir].
    IRType type;

    /// The actual specific IR representation.
    union {
        Immediate_IR immediateIR;
        Register_IR registerIR;
        LoadStore_IR loadStoreIR;
        Branch_IR branchIR;
    } ir;
} IR;

#endif // IR_H
