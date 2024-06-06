///
/// branchDecoder.c
/// Decodes a binary word of a branch instruction to its [IR].
///
/// Created by Alexander Biraben-Renard on 30/05/2024.
///

#include "branchDecoder.h"

/// Decodes a binary word of a branch instruction to its [IR].
/// @param word The [Instruction] to decode.
/// @returns The [IR] of word.
IR decodeBranch(Instruction word) {
    Branch_IR branchIR;

    if ((word & BRANCH_UNCONDITIONAL_M) == BRANCH_UNCONDITIONAL_B) {
        // Get the 26-bit offset as a 32-bit unsigned integer
        int32_t simm26 = decompose(word, BRANCH_UNCONDITIONAL_SIMM26_M);
        simm26 = signExtend(simm26, BRANCH_UNCONDITIONAL_SIMM26_N);

        branchIR = (Branch_IR) { .type = BRANCH_UNCONDITIONAL, .data.simm26.data.immediate = simm26 };
    } else if ((word & BRANCH_REGISTER_M) == BRANCH_REGISTER_B) {
        branchIR = (Branch_IR) { .type = BRANCH_REGISTER, .data.xn = decompose(word, BRANCH_REGISTER_XN_M) };
    } else if ((word & BRANCH_CONDITIONAL_M) == BRANCH_CONDITIONAL_B) {
        struct Conditional conditional;

        // Get the 19-bit offset as a 32-bit unsigned integer
        int32_t simm19 = decompose(word, BRANCH_CONDITIONAL_SIMM19_M);
        conditional.simm19.data.immediate = signExtend(simm19, BRANCH_CONDITIONAL_SIMM19_N);

        // Get the condition code from the instruction and check if it's valid
        uint8_t condition = decompose(word, BRANCH_CONDITIONAL_COND_M);

        switch (condition) {
            case EQ:
            case NE:
            case GE:
            case LT:
            case GT:
            case LE:
            case AL:
                conditional.condition = condition;
                break;

            default:throwFatal("Invalid condition code!");
        }

        branchIR = (Branch_IR) { .type = BRANCH_CONDITIONAL, .data.conditional = conditional };
    } else {
        throwFatal("Invalid instruction format!");
    }

    return (IR) { .type = BRANCH, .ir.branchIR = branchIR };
}
