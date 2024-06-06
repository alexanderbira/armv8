///
/// register.h
/// The intermediate representation and functions to process a data processing (register) instruction.
///
/// Created by Billy Highley on 29/05/2024.
///

#ifndef IR_REGISTER_H
#define IR_REGISTER_H

#include <stdbool.h>
#include <stdint.h>

#include "../const.h"
#include "types.h"

/// Baseline code for a data processing (register) instruction.
#define REGISTER_B              b(0000_1010_0000_0000_0000_0000_0000_0000)

/// Mask for [sf] (the bit-width specifier) in a data processing (register) instruction.
#define REGISTER_SF_M           maskl(1)

/// Number of bits to shift for [opc] in a data processing (register) instruction.
#define REGISTER_OPC_S          29

/// Number of bits in [opc] in a data processing (register) instruction.
#define REGISTER_OPC_N          2

/// Mask for [opc] (operation code) in a data processing (register) instruction.
#define REGISTER_OPC_M          mask(30, 29)

/// Number of bits to shift for [opc] in a data processing (register) instruction.
#define REGISTER_M_S            28

/// Mask for [M] in a data processing (register) instruction.
#define REGISTER_M_M            mask(28, 28)

/// Number of bits to shift for [opr] in a data processing (register) instruction.
#define REGISTER_OPR_S          21

/// Number of bits in [opr] in a data processing (register) instruction.
#define REGISTER_OPR_N          4

/// Mask for [opr] in a data processing (register) instruction.
#define REGISTER_OPR_M          mask(24, 21)

/// Number of bits to shift for [rm] in a data processing (register) instruction.
#define REGISTER_RM_S           16

/// Number of bits in [rm] in a data processing (register) instruction.
#define REGISTER_RM_N           5

/// Mask for [rm] in a data processing (register) instruction.
#define REGISTER_RM_M           mask(20, 16)

/// Mask for [operand] in a data processing (register) instruction.
#define REGISTER_OPERAND_M      mask(15, 10)

/// Number of bits to shift [imm6] in [operand] in a data processing (register) instruction.
#define REGISTER_OPERAND_IMM6_S 10

/// Number of bits in [imm6] in [operand] in a data processing (register) instruction.
#define REGISTER_OPERAND_IMM6_N 6

/// Number of bits to shift [x] in [operand] in a data processing (register) instruction.
#define REGISTER_OPERAND_X_S    15

/// Number of bits to shift [ra] in [operand] in a data processing (register) instruction.
#define REGISTER_OPERAND_RA_S   10

/// Number of bits in [ra] in [operand] in a data processing (register) instruction.
#define REGISTER_OPERAND_RA_N   5

/// Number of bits to shift [rn] in [operand] in a data processing (register) instruction.
#define REGISTER_RN_S           5

/// Number of bits in [rn] in [operand] in a data processing (register) instruction.
#define REGISTER_RN_N           5

/// Mask for [rn] in a data processing (register) instruction.
#define REGISTER_RN_M           mask(9, 5)

/// Number of bits in [rd] in [operand] in a data processing (register) instruction.
#define REGISTER_RD_N           5

/// Mask for [rd] in a data processing (register) instruction.
#define REGISTER_RD_M           maskr(5)

/// Code for a data processing (register, arithmetic) instruction.
#define REGISTER_ARITHMETIC_C   b(1000)

/// Mask for a data processing (register, arithmetic) instruction.
#define REGISTER_ARITHMETIC_M   b(1001)

/// Code for a data processing (register, bit-logic) instruction.
#define REGISTER_BIT_LOGIC_C    b(0000)

/// Mask for a data processing (register, bit-logic) instruction.
#define REGISTER_BIT_LOGIC_M    b(1000)

/// Mask for [shift] in [opr] in a data processing (register, arithmetic / bit-logic) instruction.
#define REGISTER_SHIFT_M        b(0110)

/// Code for a data processing (register, multiply) instruction.
#define REGISTER_MULTIPLY_C     b(1000)

/// The intermediate representation of a data processing (register) instruction.
typedef struct {

    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// [2b] The operation code, determining the operation to be performed. To access union, check [opi].
    union RegisterOpCode {

        enum ArithmeticType arithmetic;

        /// Arithmetic and logical instructions, separated into standard and negated types.
        union Logic {

            /// Standard operation codes
            enum StandardType {

                /// The operation code for AND.
                /// \code Rd := Rn & Op2 \endcode
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
                /// \code Rd := Rn & ~Op2 \endcode
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

        } logic;

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

    /// [1b] Type of instruction to perform (when combined with opr).
    bool M;

    /// [4b] Type of instruction to perform (when combined with M).
    uint8_t opr;

    /// Type of data processing operation. (Derived from [opr] and [M].)
    enum RegisterType { ARITHMETIC, BIT_LOGIC, MULTIPLY } group;

    /// The type of shift to perform on Rm. (Derived from [opr].)
    enum ShiftType {
        LSL, ///< Logical shift left.
        LSR, ///< Logical shift right.
        ASR, ///< Arithmetic shift right.
        ROR  ///< Rotate right.
    } shift;

    /// (Logical only) whether the shifted register is bitwise negated. (Derived from [opr].)
    bool negated;

    /// [5b] The encoding of the second operand register.
    uint8_t rm;

    /// [6b] The last operand of the instruction.
    union RegisterOperand {

        /// [6b] Arithmetic and logical interpretation: 6-bit unsigned immediate value.
        uint8_t imm6;

        /// [6b] Multiply interpretation: operand consists of [1b] x and [5b] ra components.
        struct Multiply {

            /// [1b] Determines whether to negate the product (1 = negate).
            bool x;

            /// [5b] The encoding of the second operand register.
            uint8_t ra;

        } multiply;

    } operand;

    /// [5b] The encoding of the first operand register.
    uint8_t rn;

    /// [5b] The encoding of the destination register.
    uint8_t rd;

} Register_IR;

#endif // IR_REGISTER_H
