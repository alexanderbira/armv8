///
/// imm.h
/// The intermediate representation and functions to process a Data Processing (Immediate) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef IMM_H
#define IMM_H

#include <stdbool.h>
#include <stdint.h>

#include "../const.h"
#include "../../emulator/system/registers.h"
#include "../../emulator/system/memory.h"

/// The intermediate representation of a Data Processing (Immediate) instruction.
typedef struct {
    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// [2b] The operation code, determining the operation to be performed. To access union, check [opi].
    union {
        /// The operation code for when the operation is arithmetic.
        /// This ordinal values of these enums correspond to their bit masks.
        enum ArithType {
            ADD,  ///< Rd := Rd + Op2
            ADDS, ///< Rd := Rd + Op2, set flags.
            SUB,  ///< Rd := Rn - Op2
            SUBS  ///< Rd := Rn - Op2, set flags.
        } arithType;

        /// The operation code for when the operation is a wide move.
        /// This ordinal values of these enums correspond to their bit masks.
        enum WideMoveType {
            MOVN,       ///< Rd := ¬Op
            MOVZ = 0x2, ///< Rd := Op
            MOVK        ///< Rd[shift + 15:shift] := imm16
        } wideMoveType;
    } opc;

    /// [3b] The type of data processing operation, determining the interpretation of [opc] and [operand].
    /// This ordinal values of these enums correspond to their bit masks.
    enum ImmType { ARITH = 0x2, WIDE_MOVE = 0x5 } opi;

    /// [18b] The value to be saved into the register.
    union {
        struct {
            /// [1b] Flag for whether the immediate is to be left-shifted by 12-bits.
            bool sh;

            /// [12b] 12-bit unsigned immediate value.
            uint16_t imm12;

            /// [5b] The first operand register.
            uint8_t rd;
        } arith;

        struct {
            /// [2b] Encodes a logical left shift by [hw] * 16 bits.
            uint8_t hw;

            /// [16b] 16-bit unsigned immediate value to move.
            uint16_t imm16;
        } wideMove;
    } operand;

    /// [5b] The encoding of the destination register, where specially 0b11111 encodes ZR, and additionally in an
    /// arithmetic instruction encodes the stack pointer.
    uint8_t rd;
} Imm_IR;

BitInst immToInst(Imm_IR ir);

void immProc(Imm_IR ir, Memory mem, Registers regs);

#endif //IMM_H
