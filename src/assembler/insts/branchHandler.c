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
    assertFatal(line.operandCount == 1, "[handleBranch] Incorrect number of operands!");

    Branch_IR branchIR;
    if (!strcmp(line.mnemonic, "b")) {
        Literal simm26 = parseLiteral(line.operands[0]);
        branchIR = (Branch_IR) {B, .branch.simm26 = simm26};
    } else if (!strcmp(line.mnemonic, "br")) {
        uint8_t xn = parseRegister(line.operands[0]);
        branchIR = (Branch_IR) {BR, .branch.xn = xn};
    } else {
        // Get just the condition string.
        char *name = line.mnemonic;
        name += 2;

        // Try to find the corresponding condition, else throw error.
        bool found = false;
        enum BranchCondition condition = AL;
        for (int i = 0; conditionMappings[i].name != NULL; i++) {
            if (!strcasecmp(conditionMappings[i].name, name)) {
                condition = conditionMappings[i].condition;
                found = true;
                break;
            }
        }

        assertFatal(found, "[branchHandler] Invalid condition code!");
        Literal simm19 = parseLiteral(line.operands[0]);
        branchIR = (Branch_IR) {BCOND, .branch.conditional = {simm19, condition}};
    }

    return (IR) {BRANCH, .repr.branch = branchIR};
}
