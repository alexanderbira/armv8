///
/// registerDecoder.c
/// Decodes a binary word of a data processing (register) instruction to its [IR].
///
/// Created by Billy Highley on 30/05/2024.
///

#include "registerDecoder.h"

/// Decodes a binary word of a data processing (register) instruction to its [IR].
/// @param word The instruction to decode
/// @returns The IR of word
IR decodeRegister(Instruction word) {

    Register_IR registerIR = (Register_IR) {
            .sf  = decompose(word, REGISTER_SF_M),
            .M   = decompose(word, REGISTER_M_M),
            .opr = decompose(word, REGISTER_OPR_M),
            .rm  = decompose(word, REGISTER_RM_M),
            .rn  = decompose(word, REGISTER_RN_M),
            .rd  = decompose(word, REGISTER_RD_M),
    };

    Component opc = decompose(word, REGISTER_OPC_M);
    Component op = decompose(word, REGISTER_OPERAND_M);

    if (registerIR.M == 0 && ((registerIR.opr & REGISTER_ARITHMETIC_M) == REGISTER_ARITHMETIC_C)) {

        registerIR.group = ARITHMETIC;

    } else if (registerIR.M == 0 && ((registerIR.opr & REGISTER_BIT_LOGIC_M) == REGISTER_BIT_LOGIC_C)) {

        registerIR.group = BIT_LOGIC;

    } else if (registerIR.M == 1 && (registerIR.opr == REGISTER_MULTIPLY_C)) {

        registerIR.group = MULTIPLY;

    } else {

        throwFatal("[decodeRegister] Invalid data processing type!");

    }

    // Determine the type of register instruction using its group
    switch (registerIR.group) {

        case ARITHMETIC: {
            registerIR.opc.arithmetic = opc;
            registerIR.shift = (registerIR.opr & REGISTER_SHIFT_M) >> 1;
            break;
        }

        case BIT_LOGIC: {
            (registerIR.opr & 0) ?
            (registerIR.opc.logic.standard = opc) :
            (registerIR.opc.logic.negated = opc);
            registerIR.shift = (registerIR.opr & REGISTER_SHIFT_M) >> 1;
            registerIR.negated = registerIR.opr & 1;
            break;
        }

        case MULTIPLY: {
            registerIR.opc.multiply = opc;
            break;
        }

    }

    // Set operand constituents depending on whether the instruction is multiply type (M = 1) or not (M = 0).
    (registerIR.M) ?
    (registerIR.operand.multiply.x = op >> 5, registerIR.operand.multiply.ra = op & ((1 << 5) - 1)) :
    (registerIR.operand.imm6 = op);

    return (IR) {.type = REGISTER, .ir.registerIR = registerIR};

}
