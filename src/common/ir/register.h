///
/// register.h
/// The intermediate representation and functions to process a Data Processing (Register) instruction.
///
/// Created by Billy Highley on 29/05/2024.
///

#ifndef IR_REGISTER_H
#define IR_REGISTER_H

#include <stdbool.h>
#include <stdint.h>

#include "../const.h"
#include "types.h"

#define REGISTER_               b(0000_1010_0000_0000_0000_0000_0000_0000)

/// Mask for [sf] (the bit-width specifier) in a Register instruction.
#define REGISTER_SF_M           maskl(1)

/// Number of bits in [opc] in a Register instruction.
#define REGISTER_OPC_N          2

/// Number of bits to shift for [opc] in a Register instruction.
#define REGISTER_OPC_S          29

/// Mask for [opc] (operation code) in a Register instruction.
#define REGISTER_OPC_M          mask(30, 29)

/// Number of bits to shift for [opc] in a Register instruction.
#define REGISTER_M_S            28

/// Mask for [M] in a Register instruction.
#define REGISTER_M_M            mask(28, 28)

/// Number of bits in [opr] in a Register instruction.
#define REGISTER_OPR_N          4

/// Number of bits to shift for [opr] in a Register instruction.
#define REGISTER_OPR_S          21

/// Mask for [opr] in a Register instruction.
#define REGISTER_OPR_M          mask(24, 21)

/// Number of bits in [rm] in a Register instruction.
#define REGISTER_RM_N           5

/// Number of bits to shift for [rm] in a Register instruction.
#define REGISTER_RM_S           16

/// Mask for [rm] in a Register instruction.
#define REGISTER_RM_M           mask(20, 16)

/// Number of bits in [imm6] in [operand] in a Register instruction.
#define REGISTER_OPERAND_IMM6_N 6

/// Number of bits to shift [imm6] in [operand] in a Register instruction.
#define REGISTER_OPERAND_IMM6_S 10

/// Number of bits to shift [x] in [operand] in a Register instruction.
#define REGISTER_OPERAND_X_S    15

/// Number of bits in [ra] in [operand] in a Register instruction.
#define REGISTER_OPERAND_RA_N   5

/// Number of bits to shift [ra] in [operand] in a Register instruction.
#define REGISTER_OPERAND_RA_S   10

/// Mask for [operand] in a Register instruction.
#define REGISTER_OPERAND_M      mask(15, 10)

/// Number of bits in [rn] in [operand] in a Register instruction.
#define REGISTER_RN_N           5

/// Number of bits to shift [rn] in [operand] in a Register instruction.
#define REGISTER_RN_S           5

/// Mask for [rn] in a Register instruction.
#define REGISTER_RN_M           mask(9, 5)

/// Number of bits in [rd] in [operand] in a Register instruction.
#define REGISTER_RD_N           5

/// Mask for [rd] in a Register instruction.
#define REGISTER_RD_M           maskr(5)

/// Mask for [shift] in [opr] in a Register (Arithmetic / Bit-Logic) instruction.
#define REGISTER_SHIFT_M        b(0110)

/// Group mask for a Register (Arithmetic) instruction.
#define REGISTER_ARITHMETIC_GM  b(1001)

/// Group mask for a Register (Bit-Logic) instruction.
#define REGISTER_BITLOGIC_GM    b(1000)

/// Code for a Register (Arithmetic) instruction.
#define REGISTER_ARITHMETIC_C   b(1000)

/// Code for a Register (Bit-Logic) instruction.
#define REGISTER_BITLOGIC_C     b(0000)

/// Code for a Register (Multiply) instruction.
#define REGISTER_MULTIPLY_C     b(1000)

/// The intermediate representation of a data processing (register) instruction.
typedef struct {

    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// [2b] The operation code, determining the operation to be performed.
    union RegisterOpCode {

        /// The operation code for arithmetic operations.
        enum ArithmeticType arithmetic;

        /// The operation code for logical operations.
        union Logic {

            /// Standard operation codes.
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

        /// The operation code for multiply operations.
        enum MultiplyType {

            /// The operation code for multiply-add (non-negated product).
            /// \code Rd := Ra + (Rn * Rm) \endcode
            MADD,

            /// The operation code for multiply-sub (negated product).
            /// \code Rd := Ra - (Rn * Rm) \endcode
            MSUB

        } multiply;

    } opc;

    /// [1b] The type of instruction to perform: 0 for non-multiply, 1 for multiply.
    bool M;

    /// [4b] The type of instruction to perform (when combined with M).
    uint8_t opr;

    /// The type of register operation (derived from opr and M).
    enum RegisterType {

        /// The operation code for arithmetic instructions.
        ARITHMETIC,

        /// The operation code for bit-logic instructions.
        BIT_LOGIC,

        /// The operation code for multiply instructions.
        MULTIPLY

    } group;

    /// The type of shift to perform on Rm (derived from opr).
    enum ShiftType {

        /// The shift code for logical shift left.
        LSL,

        /// The shift code for logical shift right.
        LSR,

        /// The shift code for arithmetic shift right.
        ASR,

        /// The shift code for rotate right.
        ROR

    } shift;

    /// Whether the shifted register is to be bitwise negated (derived from opr).
    bool negated;

    /// [5b] The encoding of the Rm register.
    uint8_t rm;

    /// [6b] The final operand of the instruction.
    union RegisterOperand {

        /// [6b] Arithmetic and logical interpretation: 6-bit unsigned immediate value.
        uint8_t imm6;

        /// [6b] Multiply interpretation: operand consists of x and ra components.
        struct Multiply {

            /// [1b] Determines whether to negate the product: 0 for false, 1 for true.
            bool x;

            /// [5b] The encoding of the Ra register.
            uint8_t ra;

        } multiply;

    } operand;

    /// [5b] The encoding of the Rn register.
    uint8_t rn;

    /// [5b] The encoding of the Rd register.
    uint8_t rd;

} Register_IR;

#endif // IR_REGISTER_H
