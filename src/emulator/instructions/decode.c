///
/// decode.c
/// Decode an instruction word to its intermediate representation (IR)
///
/// Created by Billy Highley on 27/05/2024.
///

#include "decode.h"

/// Decode an instruction word to its intermediate representation (IR)
/// @param word The instruction to decode.
/// @returns The IR of the instruction.
IR decode(Instruction word) {

    IR ir;

    Component op0 = decompose(word, OP0_M);

    if ((op0 & OP0_IMM_M) == OP0_IMM_C) {
        ir = decodeImmediate(word);
    }

    else if ((op0 & OP0_REG_M) == OP0_REG_C) {
        ir = decodeRegister(word);
    }

    else if ((op0 & OP0_LS_M)  == OP0_LS_C) {
        ir = decodeLoadStore(word);
    }

    else if ((op0 & OP0_BR_M)  == OP0_BR_C) {
        ir = decodeBranch(word);
    }

    else printf("Unknown op0: %d", op0); // TODO exit program here

    return ir;
}
