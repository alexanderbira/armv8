///
/// loadStoreDecode.c
/// Decode a branch instruction to its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 30/05/2024.
///

#include "branchDecode.h"
#include <stdio.h>
#include <stdlib.h>

/// Decode a branch-group instruction to its IR
/// @param word The instruction to decode
/// @return The IR of word
IR decodeBranch(Instruction word) {
    // Initialise the IR
    IR ir;
    ir.type = Br;
    Branch_IR branchIr;

    if (decompose(word, B_UCOND_M) == B_UCOND_C) {
        // Unconditional branch
        branchIr.branchType = B;

        // Get the 26-bit offset as a 16-bit unsigned integer
        uint32_t simm26 = decompose(word, B_SIMM26_M);
        // Sign-extend the address offset (32 - 26 = 6)
        branchIr.branch.simm26 = (simm26 << 6) >> 6;

    } else if (
        decompose(word, B_REG_U_M) == B_REG_U_C &&
        decompose(word, B_REG_L_M) == B_REG_L_C
    ) {
        // Register branch
        branchIr.branchType = BR;

        branchIr.branch.xn = decompose(word, B_XN_M);

    } else if (
        decompose(word, B_COND_U_M) == B_COND_U_C &&
        decompose(word, B_COND_L_M) == B_COND_L_C
    ) {
        // Conditional branch
        branchIr.branchType = BCOND;

        struct Conditional conditional;

        // Get the 19-bit offset as a 16-bit unsigned integer
        uint32_t simm19 = decompose(word, B_SIMM19_M);
        // Sign-extend the address offset (32 - 19 = 13)
        conditional.simm19 = (simm19 << 13) >> 13;

        // Get the condition code from the instruction and check if it's valid
        uint8_t cond = decompose(word, B_COND_M);
        switch (cond) {
            case EQ:
            case NE:
            case GE:
            case LT:
            case GT:
            case LE:
            case AL:
                // cond is a valid condition code
                conditional.cond = cond;
                break;
            default:
                // cond is not a valid condition code
                fprintf(stderr, "Invalid condition code");
                exit(-1);
        }

        branchIr.branch.conditional = conditional;

    } else {
        // word is not a well-formed branch instruction
        fprintf(stderr, "Invalid instruction format");
        exit(-1);
    }

    // Save the branch IR to the IR
    ir.repr.branch = branchIr;
    return ir;
}
