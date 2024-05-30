///
/// branchAssem.h
/// Functions to parse from assembly and write as binary a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "branchHandler.h"

/// Translates an assembly branch instruction into its IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The IR form of the branch instruction.
/// @pre [line]'s mnemonic is at least one of "b", "br", or "b.COND".
IR handleBranch(TokenisedLine line, AssemblerState state) {
    // TODO: Assert 1 param

    Branch_IR branchIR;

    if (!strcmp(line.mnemonic, "b")) {
        branchIR.branchType = B;

    } else if (!strcmp(line.mnemonic, "br")) {
        branchIR.branchType = BR;

    } else {
        branchIR.branchType = BCOND;

        // Get just the condition string.
        char *condition = line.mnemonic;
        condition += 2;

        for (int i = 0; conditionMappings[i].name != NULL; i++) {
            if (conditionMappings[i].name == condition) {
                branchIR.branch.conditional.cond = conditionMappings[i].condition;
                break;
            }
        }
    }

    IR ir;
    ir.type = BRANCH;
    ir.repr = branchIR;
    return ir;
}
