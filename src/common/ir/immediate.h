///
/// immediate.h
/// The intermediate representation of a Data Processing (Immediate) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef IR_IMMEDIATE_H
#define IR_IMMEDIATE_H

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

/// Baseline mask for a Immediate instruction.
#define IMMEDIATE_C                  b(0001_0000_0000_0000_0000_0000_0000_0000)

/// Number of bits to shift for [sh] in an Immediate (Arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_SH_S    22

/// Number of bits to shift for [imm12] in an Immediate (Arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_IMM12_S 22

/// Number of bits in [imm12] in an Immediate (Arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_IMM12_N 12

/// Number of bits to shift for [rn] in an Immediate (Arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_RN_S    5

/// Number of bits in [rn] in an Immediate (Arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_RN_N    5

/// Number of bits to shift for [hw] in an Immediate (Wide Move) instruction.
#define IMMEDIATE_WIDE_MOVE_HW_S     21

/// Number of bits in [hw] in an Immediate (Wide Move) instruction.
#define IMMEDIATE_WIDE_MOVE_HW_N     2

/// Number of bits to shift for [hw] in an Immediate (Wide Move) instruction.
#define IMMEDIATE_WIDE_MOVE_IMM16_S  5

/// Number of bits in [hw] in an Immediate (Wide Move) instruction.
#define IMMEDIATE_WIDE_MOVE_IMM16_N  16

/// Number of bits to shift for [sf] in an Immediate instruction.
#define IMMEDIATE_SF_S               31

/// Number of bits to shift for [opc] in an Immediate instruction.
#define IMMEDIATE_OPC_S              29

/// Number of bits in [opc] in an Immediate instruction.
#define IMMEDIATE_OPC_N              2

/// Number of bits to shift for [opi] in an Immediate instruction.
#define IMMEDIATE_OPI_S              23

/// Number of bits in [opi] in an Immediate instruction.
#define IMMEDIATE_OPI_N              3

/// Number of bits in [rd] in an Immediate instruction.
#define IMMEDIATE_RD_N               5

/// The intermediate representation of a data processing (immediate) instruction.
typedef struct {

    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// [2b] The operation code, determining the operation to be performed. To access union, check [opi].
    union ImmediateOpCode {

        enum ArithmeticType arithmeticType;

        /// The opcode for wide move operations.
        enum WideMoveType {

            /// The operation code for move wide with NOT.
            /// \code Rd := ~Op \endcode
            MOVN,

            /// The operation code for move wide with zero.
            /// \code Rd := Op \endcode
            MOVZ = 0x2,

            /// The operation code for move wide with keep.
            /// \code Rd[shift + 15:shift] := imm16 \endcode
            MOVK

        } wideMoveType;

    } opc;

    /// [3b] The type of data processing operation, determining the interpretation of opc and operand.
    enum ImmediateType { IMMEDIATE_ARITHMETIC = 0x2, IMMEDIATE_WIDE_MOVE = 0x5 } opi;

    /// [18b] The value to be saved into the register.
    union ImmediateOperand {

        struct Arithmetic {
            /// [1b] Determines whether to left shift imm12 by 12-bits.
            bool sh;

            /// [12b] 12-bit unsigned immediate value.
            uint16_t imm12;

            /// [5b] The first operand register.
            uint8_t rn;

        } arithmetic;

        struct WideMove {

            /// [2b] Encodes a logical left shift by hw * 16 bits.
            uint8_t hw;

            /// [16b] 16-bit unsigned immediate value to move.
            uint16_t imm16;

        } wideMove;

    } operand;

    /// [5b] The encoding of the destination register.
    uint8_t rd;

} Immediate_IR;

#endif // IR_IMMEDIATE_H
