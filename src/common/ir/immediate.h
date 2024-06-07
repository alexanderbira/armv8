///
/// immediate.h
/// The intermediate representation of a data processing (immediate) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef IR_IMMEDIATE_H
#define IR_IMMEDIATE_H

#include <stdbool.h>
#include <stdint.h>

#include "../const.h"
#include "types.h"

/// Baseline code for a data processing (immediate) instruction.
#define IMMEDIATE_B                  b(0001_0000_0000_0000_0000_0000_0000_0000)

/// Number of bits to shift for [sf] in a data processing (immediate) instruction.
#define IMMEDIATE_SF_S               31

/// Mask for [sf] (the bit-width specifier) in a data processing (immediate) instruction.
#define IMMEDIATE_SF_M               maskl(1)

/// Number of bits to shift for [opc] in a data processing (immediate) instruction.
#define IMMEDIATE_OPC_S              29

/// Number of bits in [opc] in a data processing (immediate) instruction.
#define IMMEDIATE_OPC_N              2

/// Mask for [opc] (operation code) in a data processing (immediate) instruction.
#define IMMEDIATE_OPC_M              mask(30, 29)

/// Number of bits to shift for [opi] in a data processing (immediate) instruction.
#define IMMEDIATE_OPI_S              23

/// Number of bits in [opi] in a data processing (immediate) instruction.
#define IMMEDIATE_OPI_N              3

/// Mask for [opi] (data process type) in a data processing (immediate) instruction.
#define IMMEDIATE_OPI_M              mask(25, 23)

/// Number of bits in [rd] in a data processing (immediate) instruction.
#define IMMEDIATE_RD_N               5

/// Mask for [rd] (destination register) in a data processing (immediate) instruction.
#define IMMEDIATE_RD_M               mask(4, 0)

/// Number of bits to shift for [sh] in a data processing (immediate, arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_SH_S    22

/// Mask for [sh] in a data processing (immediate, arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_SH_M    mask(22,22)

/// Number of bits to shift for [imm12] in a data processing (immediate, arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_IMM12_S 10

/// Number of bits in [imm12] in a data processing (immediate, arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_IMM12_N 12

/// Mask for [imm12] in a data processing (immediate, arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_IMM12_M mask(21, 10)

/// Number of bits to shift for [rn] in a data processing (immediate, arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_RN_S    5

/// Number of bits in [rn] in a data processing (immediate, arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_RN_N    5

/// Mask for [rn] in a data processing (immediate, arithmetic) instruction.
#define IMMEDIATE_ARITHMETIC_RN_M    mask(9, 5)

/// Number of bits to shift for [hw] in a data processing (immediate, wide move) instruction.
#define IMMEDIATE_WIDE_MOVE_HW_S     21

/// Number of bits in [hw] in a data processing (immediate, wide move) instruction.
#define IMMEDIATE_WIDE_MOVE_HW_N     2

/// Mask for [hw] in a data processing (immediate, wide move) instruction.
#define IMMEDIATE_WIDE_MOVE_HW_M     mask(22, 21)

/// Number of bits to shift for [imm16] in a data processing (immediate, wide move) instruction.
#define IMMEDIATE_WIDE_MOVE_IMM16_S  5

/// Number of bits in [imm16] in a data processing (immediate, wide move) instruction.
#define IMMEDIATE_WIDE_MOVE_IMM16_N  16

/// Mask for [imm16] in a data processing (immediate, wide move) instruction.
#define IMMEDIATE_WIDE_MOVE_IMM16_M  mask(20, 5)

/// The intermediate representation of a data processing (immediate) instruction.
typedef struct {

    /// [1b] Encodes the bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// [2b] The operation code, determining the operation to be performed.
    union ImmediateOpCode {

        /// Arithmetic operation.
        /// @attention Ordinal values represent binary encodings.
        enum ArithmeticType arithmeticType;

        /// Wide move operation.
        /// @attention Ordinal values represent binary encodings.
        enum WideMoveType {

            /// Move wide with NOT.
            /// \code Rd := ~Op \endcode
            MOVN,

            /// Move wide with zero.
            /// \code Rd := Op \endcode
            MOVZ = 0x2,

            /// Move wide with keep.
            /// \code Rd[shift + 15:shift] := imm16 \endcode
            MOVK

        } wideMoveType;

    } opc;

    /// [3b] The type of data processing operation.
    /// @attention Ordinal values represent binary encodings.
    enum ImmediateType {

        /// Arithmetic operation.
        IMMEDIATE_ARITHMETIC = 0x2,

        /// Wide move operation.
        IMMEDIATE_WIDE_MOVE  = 0x5

    } opi;

    /// [18b] The value to be saved into the register.
    union ImmediateOperand {

        /// Arithmetic operation.
        struct Arithmetic {

            /// [1b] Determines whether to left shift [imm12] by 12-bits.
            bool sh;

            /// [12b] 12-bit unsigned immediate value.
            uint16_t imm12;

            /// [5b] The encoding of the Rn register.
            uint8_t rn;

        } arithmetic;

        /// Wide move operation.
        struct WideMove {

            /// [2b] Encodes a logical left shift by [hw] * 16 bits.
            uint8_t hw;

            /// [16b] 16-bit unsigned immediate value to move.
            uint16_t imm16;

        } wideMove;

    } operand;

    /// [5b] The encoding of the Rd register.
    uint8_t rd;

} Immediate_IR;

#endif // IR_IMMEDIATE_H
