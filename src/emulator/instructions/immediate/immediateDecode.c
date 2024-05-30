///
/// immediateDecode.c
/// Decodes an immediate instruction into its IR
///
/// Created by Billy Highley on 27/05/2024.
///

#include "immediateDecode.h"

/// Decode a immediate group instruction to its IR
/// @param word The instruction to decode
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
            immIR.opc.arithType       = decompose(word, IMM_OPC_M);
            immIR.operand.arith.sh    = decompose(word, IMM_SH_M);
            immIR.operand.arith.imm12 = decompose(word, IMM_12_M);
            immIR.operand.arith.rn    = decompose(word, IMM_RN_M);
            break;

        }

        // Wide move instruction
        case WIDE_MOVE: {

            // Define an wide move type IR using the components of the instruction
            immIR.opc.wideMoveType       = decompose(word, IMM_OPC_M);
            immIR.operand.wideMove.hw    = decompose(word, IMM_HW_M);
            immIR.operand.wideMove.imm16 = decompose(word, IMM_16_M);
            break;

        }

    }

    // The complete IR
    return ir;

}

