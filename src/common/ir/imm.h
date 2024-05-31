///
/// imm.h
/// The intermediate representation of a Data Processing (Immediate) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef COMMON_IMM_H
#define COMMON_IMM_H

#include <stdbool.h>
#include <stdint.h>
#include "enums.h"

/// The intermediate representation of a data processing (immediate) instruction.
typedef struct {

    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// [2b] The operation code, determining the operation to be performed. To access union, check [opi].
    union OpcImm {

        enum ArithType arithType;

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
    enum ImmType { ARITH = 0x2, WIDE_MOVE = 0x5 } opi;

    /// [18b] The value to be saved into the register.
    union OperandImm {

        struct Arith {
            /// [1b] Determines whether to left shift imm12 by 12-bits.
            bool sh;

            /// [12b] 12-bit unsigned immediate value.
            uint16_t imm12;

            /// [5b] The first operand register.
            uint8_t rn;

        } arith;

        struct WideMove {

            /// [2b] Encodes a logical left shift by hw * 16 bits.
            uint8_t hw;

            /// [16b] 16-bit unsigned immediate value to move.
            uint16_t imm16;

        } wideMove;

    } operand;

    /// [5b] The encoding of the destination register.
    uint8_t rd;

} Imm_IR;

#endif //COMMON_IMM_H
