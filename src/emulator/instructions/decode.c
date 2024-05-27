///
/// decode.c
/// Delegate the work to the correct instruction handler
///
/// Created by Billy Highley on 27/05/2024.
///

#include "decode.h"

/// Delegates decoding and execution to the corresponding process function.
/// @param word The instruction to process.
void decode(Instruction word, Registers regs, Memory mem) {
    Component op0 = decompose(word, OP0_M);

    if      ((op0 & OP0_IMM_M) == OP0_IMM_C)
            processImmediate(word, regs, mem);

    else if ((op0 & OP0_REG_M) == OP0_REG_C)
            processRegister(word, regs, mem);

    else if ((op0 & OP0_LS_M)  == OP0_LS_C)
            processLoadStore(word, regs, mem);

    else if ((op0 & OP0_BR_M)  == OP0_BR_C)
            processBranch(word, regs, mem);

    else    printf("Unknown op0: %d", op0);
}
