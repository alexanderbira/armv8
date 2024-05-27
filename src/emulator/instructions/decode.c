///
/// decode.c
/// Delegate the work to the correct instruction handler
///
/// Created by Billy Highley on 27/05/2024.
///

#include <stdio.h>

/// Delegates decoding and execution to the corresponding process function.
/// @param word The instruction to process.
void decode(Instruction word) {
    Component op0 = decompose(word, OP0_MASK);
    if      ((op0 & DP_IMM_OP0_MASK) == DP_IMM_OP0_CODE)
            processImmediate(word);
    else if ((op0 & DP_REG_OP0_MASK) == DP_REG_OP0_CODE)
            processRegister(word);
    else if ((op0 & LOAD_STORE_OP0_MASK) == LOAD_STORE_OP0_CODE)
            processLoadStore(word);
    else if ((op0 & BRANCH_OP0_MASK) == BRANCH_OP0_CODE)
            processBranch(word);
    else    printf("Unknown op0: %d", op0);
}
