///
/// reg.h
/// The intermediate representation and functions to process a Data Processing (Register) instruction.
///
/// Created by Billy Highley on 29/05/2024.
///

#ifndef REG_H
#define REG_H

#include <stdbool.h>
#include <stdint.h>

/// The intermediate representation of a data processing (register) instruction.
typedef struct {

    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// [2b] The operation code, determining the operation to be performed. To access union, check [opi].
    union {

        /// Arithmetic and logical instructions, separated into standard and negated types.
        union {

            /// Standard operation codes
            enum StandardType {

                /// The operation code for AND.
                /// \code Rd := Rd & Op2 \endcode
                AND,

                /// The operation code for (inclusive) OR.
                /// \code Rd := Rn | Op2 \endcode
                ORR,

                /// The operation code for (exclusive) OR.
                /// \code Rd := Rn ^ Op2 \endcode
                EOR,

                /// The operation code for AND, setting flags.
                /// \code Rd := Rn & Op2 \endcode
                ANDS

            } standard;

            /// Negated operation codes
            enum NegatedType {

                /// The operation code for bit clear.
                /// \code Rd := Rd & ~Op2 \endcode
                BIC,

                /// The operation code for (inclusive) OR NOT.
                /// \code Rd := Rn | ~Op2 \endcode
                ORN,

                /// The operation code for (exclusive) OR NOT.
                /// \code Rd := Rn ^ ~Op2 \endcode
                EON,

                /// The operation code for bit clear, setting flags.
                /// \code Rd := Rn & ~Op2 \endcode
                BICS

            } negated;

        } arithmeticAndLogic;

        /// Multiply operation codes.
        enum MultiplyType {

            /// Multiply-add (non-negated product).
            /// \code Rd := Ra + (Rn * Rm) \endcode
            MADD,

            /// Multiply-sub (negated product).
            /// \code Rd := Ra - (Rn * Rm) \endcode
            MSUB

        } multiply;

    } opc;

    /// [1b] Determines the type of instruction to perform (when combined with opr).
    bool M;

    /// [4b] Determines the type of instruction to perform (when combined with M).
    uint8_t opr;

    /// The type of data processing operation, determined by interpreting the combination of M and opr.
    enum RegType { ARITHMETIC, BIT_LOGIC, MULTIPLY } group;

    /// The type of shift to perform on Rm.
    enum ShiftType {
        LSL, ///< Logical shift left.
        LSR, ///< Logical shift right.
        ASR, ///< Arithmetic shift right.
        ROR  ///< Rotate right.
    } shift;

    /// [5b] The encoding of the second operand register.
    uint8_t rm;

    /// [6b] The last operand of the instruction.
    union {

        /// [6b] Arithmetic and logical interpretation: 6-bit unsigned immediate value.
        uint8_t imm6;

        /// [6b] Multiply interpretation: operand consists of [1b] x and [5b] ra components.
        struct {

            /// [1b] Determines whether to negate the product (1 = negate).
            bool x;

            /// [5b] The encoding of the second operand register.
            uint8_t ra;

        } mul;

    } operand;

    /// [5b] The encoding of the first operand register.
    uint8_t rn;

    /// [5b] The encoding of the destination register.
    uint8_t rd;

} Reg_IR;

#endif //REG_H
