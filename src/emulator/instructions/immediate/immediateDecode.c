///
/// immediateDecode.c
/// Decodes an immediate instruction into its IR
///
/// Created by Billy Highley on 27/05/2024.
///

#include "immediateDecode.h"
#include <stdio.h>
#include <stdlib.h>

/// Decode a immediate group instruction to its IR
/// @param word The instruction to decode
/// @returns The IR of word
IR decodeImmediate(Instruction word) {

    // Initialise the IR
    IR ir;
    ir.type = DPImm;

    // Define an immediate type IR using the components of the instruction
    Imm_IR immIR;
    immIR.sf  = decompose(word, IMM_SF_M);
    immIR.opi = decompose(word, IMM_OPI_M);
    immIR.rd  = decompose(word, IMM_RD_M);

    // Determine the type of immediate instruction using opi
    switch (immIR.opi) {

        // Arithmetic instruction
        case ARITH: {

            // Define an arithmetic type IR using the components of the instruction
            // All arith types are valid since there are 4 of them encoded by 2 bits
            immIR.opc.arithType       = decompose(word, IMM_OPC_M);
            immIR.operand.arith.sh    = decompose(word, IMM_SH_M);
            immIR.operand.arith.imm12 = decompose(word, IMM_12_M);
            immIR.operand.arith.rn    = decompose(word, IMM_RN_M);
            break;

        }

        // Wide move instruction
        case WIDE_MOVE: {

            // Define a wide move type IR using the components of the instruction

            // All arith types except 0x1 are valid since there are 3 of them encoded by 2 bits
            uint8_t wideMoveType = decompose(word, IMM_OPC_M);
            if (wideMoveType == 0x1) {
                // 0x1 is not a valid wide move type
                fprintf(stderr, "Invalid wide move type");
                exit(-1);
            }

            immIR.opc.wideMoveType = wideMoveType;

            uint8_t hw = decompose(word, IMM_HW_M);
            if (immIR.sf == 0 && hw > 1) {
                fprintf(stderr, "hw can only be 0 or 1 when sf is 0");
                exit(-1);
            }
            immIR.operand.wideMove.hw = hw;

            immIR.operand.wideMove.imm16 = decompose(word, IMM_16_M);
            break;

        }

    }

    // The complete IR
    return ir;

}

