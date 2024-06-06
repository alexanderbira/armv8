///
/// branchTranslator.c
/// Transform a [IR] of a Branch instruction to a binary instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "branchTranslator.h"

/// Converts the IR form of an Branch instruction to a binary word.
/// @param irObject The [IR] struct representing the instruction.
/// @param state The current state of the assembler.
/// @return 32-bit binary word of the instruction.
Instruction translateBranch(IR *irObject, AssemblerState *state) {
    assertFatal(irObject->type == BRANCH, "[translateBranch] Received non-branch IR!");
    Branch_IR *branch = &irObject->ir.branchIR;
    Instruction result;

    switch (branch->type) {

        case BRANCH_UNCONDITIONAL:
            result = BRANCH_UNCONDITIONAL_B;
            Literal *simm26 = &branch->data.simm26;
            if (simm26->isLabel) {
                parseOffset(&simm26->data, state);
            }

            return result | truncater(simm26->data.immediate, BRANCH_UNCONDITIONAL_SIMM26_N);

        case BRANCH_REGISTER:
            result = BRANCH_REGISTER_B;
            return result | (truncater(branch->data.xn, BRANCH_REGISTER_XN_N) << BRANCH_REGISTER_XN_S);

        case BRANCH_CONDITIONAL:
            result = BRANCH_CONDITIONAL_B;
            Literal *simm19 = &branch->data.conditional.simm19;
            if (simm19->isLabel) {
                parseOffset(&simm19->data, state);
            }

            result |= truncater(simm19->data.immediate, BRANCH_CONDITIONAL_SIMM19_N)
                    << BRANCH_CONDITIONAL_SIMM19_S;
            return result | truncater(branch->data.conditional.condition, BRANCH_CONDITIONAL_COND_N);
    }

    throwFatal("[translateBranch] Unknown type of branch instruction!");
}
