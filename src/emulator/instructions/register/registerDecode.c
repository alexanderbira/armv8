///
/// registerDecode.c
/// Decodes a register instruction into its IR
///
/// Created by Billy Highley on 30/05/2024.
///

#include "registerDecode.h"
#include <stdio.h>
#include <stdlib.h>

/// Decode a register group instruction to its IR
/// @param word The instruction to decode
/// @returns The IR of word
IR decodeRegister(Instruction word) {

    // Initialise the IR
    IR ir;
    ir.type = DPReg;

    // Define a register type IR using the components of the instruction
    Reg_IR regIR;

    regIR.sf  = decompose(word, REG_SF_M);
    regIR.M   = decompose(word, REG_M_M);
    regIR.opr = decompose(word, REG_OPR_M);
    regIR.rm  = decompose(word, REG_RM_M);
    regIR.rn  = decompose(word, REG_RN_M);
    regIR.rd  = decompose(word, REG_RD_M);

    Component opc = decompose(word, REG_OPC_M);
    Component op = decompose(word, REG_OP_M);

    if (regIR.M == 0 && ((regIR.opr & REG_AR_G_M) == REG_AR_C)) {

        regIR.group = ARITHMETIC;

    } else if (regIR.M == 0 && ((regIR.opr & REG_BL_G_M) == REG_BL_C)) {

        regIR.group = BIT_LOGIC;

    } else if (regIR.M == 1 && (regIR.opr == REG_MU_C)) {

        regIR.group = MULTIPLY;

    } else {

        fprintf(stderr, "Invalid data processing type");
        exit(-1);

    }

    // Determine the type of register instruction using its group
    switch (regIR.group) {

        // Arithmetic instruction
        case ARITHMETIC: {

            // Define an arithmetic type IR using the components of the instruction
            regIR.opc.arithType = opc;
            regIR.shift = (regIR.opr & B(0110)) >> 1;
            break;

        }

        // Bit-logic instruction
        case BIT_LOGIC: {

            (regIR.opr & 0)?
            (regIR.opc.logic.standard = opc):
            (regIR.opc.logic.negated  = opc);
            regIR.shift = (regIR.opr & B(0110)) >> 1;
            regIR.negated = regIR.opr & 1;
            break;

        }

        // Multiply instruction
        case MULTIPLY: {

            regIR.opc.multiply = opc;
            break;

        }

    }

    // Set operand constituents depending on whether the instruction is multiply type (M = 1) or not (M = 0)

    (regIR.M)?
    (regIR.operand.mul.x = op >> 5,
     regIR.operand.mul.ra = op & ((1 << 6) - 1)):
    (regIR.operand.imm6 = op);

    // The complete IR
    return ir;

}
