///
/// types.h
/// Common types to all types of instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef IR_TYPES_H
#define IR_TYPES_H

#include <stdint.h>

/// Literal IR.
typedef struct {

    /// Whether this literal is a label: 0 for false, 1 for true.
    bool isLabel;

    /// The contents of the literal.
    union LiteralData {

        /// The string label of the literal.
        char *label;

        /// The signed immediate value of the literal.
        int32_t immediate;

    } data;

} Literal;

/// The operation code for arithmetic operations.
enum ArithmeticType {

    /// The operation code for add.
    /// \code Rd := Rn + Op2 \endcode
    ADD,

    /// The operation code for add, setting flags.
    /// \code Rd := Rn + Op2 \endcode
    ADDS,

    /// The operation code for subtract.
    /// \code Rd := Rn - Op2 \endcode
    SUB,

    /// The operation code for subtract, setting flags.
    /// \code Rd := Rn - Op2 \endcode
    SUBS

};

#endif // IR_TYPES_H
