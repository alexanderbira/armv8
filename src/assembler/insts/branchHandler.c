///
/// branchAssem.h
/// Functions to parse from assembly and write as binary a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "branchHandler.h"

struct {
    char *name;
    enum BranchCondition condition;
} static const conditionMappings[] = {
        {"eq", EQ},
        {"ne", NE},
        {"ge", GE},
        {"lt", LT},
        {"gt", GT},
        {"le", LE},
        {"al", AL},
        {NULL},
};

/// Translates an assembly branch instruction into its IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The IR form of the branch instruction.
/// @pre [line]'s mnemonic is at least one of "b", "br", or "b.COND".
IR handleBranch(TokenisedLine line, AssemblerState state) {
    assertFatal(line.parameterCount == 1, "[handleBranch] Incorrect number of operands!");

    Branch_IR branchIR;
    if (!strcmp(line.mnemonic, "b")) {
        branchIR.branchType = B;
        branchIR.branch.simm26 = parseLiteral(line.operands[0]);
    } else if (!strcmp(line.mnemonic, "br")) {
        branchIR.branchType = BR;
        branchIR.branch.xn = parseRegister(line.operands[0]);
    } else {
        branchIR.branchType = BCOND;

        // Get just the condition string.
        char *condition = line.mnemonic;
        condition += 2;

        // Try to find the corresponding condition, else throw error.
        bool found = false;
        for (int i = 0; conditionMappings[i].name != NULL; i++) {
            if (!strcasecmp(conditionMappings[i].name, condition)) {
                branchIR.branch.conditional.cond = conditionMappings[i].condition;
                found = true;
                break;
            }
        }

        assertFatal(found, "[branchHandler] Invalid condition code!");
        branchIR.branch.conditional.simm19 = parseLiteral(line.operands[0]);
    }

    IR ir;
    ir.type = BRANCH;
    ir.repr.branch = branchIR;
    return ir;
}
