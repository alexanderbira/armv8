///
/// ir.h
/// The intermediate representation of parsed instructions.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef IR_H
#define IR_H

#include "const.h"
#include "immediate.h"
#include "register.h"
#include "loadStore.h"
#include "branch.h"

/// The type of [IR] represented.
typedef enum {

    /// Data processing (immediate).
    IMMEDIATE,

    /// Data processing (register).
    REGISTER,

    /// Load/store.
    LOAD_STORE,

    /// Branch.
    BRANCH,

    /// Direct to memory constant.
    DIRECTIVE

} IRType;

/// The general IR wrapper for all types of assembly instructions.
typedef struct {

    /// The type of the IR.
    IRType type;

    /// The contents of the IR.
    union {

        /// Data processing (immediate) IR.
        Immediate_IR immediateIR;

        /// Data processing (register) IR.
        Register_IR registerIR;

        /// Load/store IR.
        LoadStore_IR loadStoreIR;

        /// Branch IR.
        Branch_IR branchIR;

        /// Data (used by directives) IR.
        BitData memoryData;

    } ir;

} IR;

#endif // IR_H
