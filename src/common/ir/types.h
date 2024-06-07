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

    /// Whether this [Literal] is a label: 0 for false, 1 for true.
    bool isLabel;

    /// The contents.
    union LiteralData {

        /// The string label.
        char *label;

        /// The signed immediate value.
        int32_t immediate;

    } data;

} Literal;

/// The operation code for arithmetic operations.
enum ArithmeticType {

    /// Add.
    /// \code Rd := Rn + Op2 \endcode
    ADD,

    /// Add (set flags).
    /// \code Rd := Rn + Op2 \endcode
    ADDS,

    /// Subtract.
    /// \code Rd := Rn - Op2 \endcode
    SUB,

    /// Subtract (set flags).
    /// \code Rd := Rn - Op2 \endcode
    SUBS

};

#endif // IR_TYPES_H
