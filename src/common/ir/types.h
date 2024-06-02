///
/// types.h
/// Common types to all types of instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef IR_TYPES_H
#define IR_TYPES_H

#include <stdint.h>

/// An intermediate representation of a <literal>, which is either a signed immediate
/// or a label reference. This is needed since not all label references are backwards.
typedef struct {
    /// Whether this literal is a label or not.
    bool isLabel;

    /// The contents of the literal.
    union {
        /// The string label of the literal.
        char *label;

        /// The signed immediate of the literal.
        int32_t immediate;
    } data;
} Literal;

/// The opcode for arithmetic operations.
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