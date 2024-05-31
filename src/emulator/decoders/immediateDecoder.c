///
/// immediateDecode.c
/// Decodes an immediate instruction into its IR
///
/// Created by Billy Highley on 27/05/2024.
///

#include "immediateDecoder.h"

/// Decode a immediate group instruction to its IR
/// @param word The instruction to decode
/// @returns The IR of word
IR decodeImmediate(Instruction word) {
    Immediate_IR immediateIR = (Immediate_IR) {
            .sf = decompose(word, IMMEDIATE_SF_M),
            .opi = decompose(word, IMMEDIATE_OPI_M),
            .rd = decompose(word, IMMEDIATE_RD_M),
    };

    switch (immediateIR.opi) {
        case IMMEDIATE_ARITHMETIC: {
            // All arith types are valid since there are 4 of them encoded by 2 bits
            immediateIR.opc.arithmeticType = decompose(word, IMMEDIATE_OPC_M);
            immediateIR.operand.arithmetic = (struct Arithmetic) {
                    .sh    = decompose(word, IMMEDIATE_ARITHMETIC_SH_M),
                    .imm12 = decompose(word, IMMEDIATE_ARITHMETIC_IMM12_M),
                    .rn    = decompose(word, IMMEDIATE_ARITHMETIC_RN_M)
            };
            break;
        }
        case IMMEDIATE_WIDE_MOVE: {
            // All arith types except 0x1 are valid since there are 3 of them encoded by 2 bits
            immediateIR.opc.wideMoveType = decompose(word, IMMEDIATE_OPC_M);
            assertFatal(immediateIR.opc.wideMoveType != 0x1,
                        "[decodeImmediate] Invalid wide move type!");

            immediateIR.operand.wideMove.hw = decompose(word, IMMEDIATE_WIDE_MOVE_HW_M);
            assertFatal(!(immediateIR.sf == 0 && immediateIR.operand.wideMove.hw > 1),
                        "[decodeImmediate] sf == 0 and hw > 1 is an invalid combination!");

            immediateIR.operand.wideMove.imm16 = decompose(word, IMMEDIATE_WIDE_MOVE_IMM16_M);
            break;
        }
    }

    return (IR) {.type = IMMEDIATE, .ir.immediateIR = immediateIR};
}

