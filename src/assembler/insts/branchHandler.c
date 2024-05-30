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
IR parseBranch(TokenisedLine line, AssemblerState *state) {
    assertFatal(line.operandCount == 1, "[handleBranch] Incorrect number of operands!");

    Branch_IR branchIR;
    if (!strcmp(line.mnemonic, "b")) {
        const Literal simm26 = parseLiteral(line.operands[0]);
        branchIR = (Branch_IR) {UNCONDITIONAL, .branch.simm26 = simm26};
    } else if (!strcmp(line.mnemonic, "br")) {
        uint8_t xn = parseRegister(line.operands[0]);
        branchIR = (Branch_IR) {REGISTER, .branch.xn = xn};
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
        const Literal simm19 = parseLiteral(line.operands[0]);
        branchIR = (Branch_IR) {CONDITIONAL, .branch.conditional = {simm19, condition}};
    }

    return (IR) {BRANCH, .repr.branch = branchIR};
}

BitInst writeBranch(IR ir, AssemblerState *state) {
    assertFatal(ir.type == BRANCH, "[writeBranch] Received non-branch IR!");
    Branch_IR branch = ir.repr.branch;
    BitInst result;

    switch (branch.branchType) {
        case UNCONDITIONAL:
            result = BRANCH_UNCONDITIONAL;
            if (branch.branch.simm26.isLabel) {
                BitData *address = NULL;
                getMapping(state, branch.branch.simm26.data.label, address);
                assertFatal(address != NULL, "[writeBranch] No mapping for label!");
                return result | truncate(*address, BRANCH_UNCONDITIONAL_SIMM26_N);
            }

            return result | truncate(branch.branch.simm26.data.immediate, BRANCH_UNCONDITIONAL_SIMM26_N);
        case REGISTER:
            result = BRANCH_REGISTER;
            return result | (truncate(branch.branch.xn, BRANCH_REGISTER_XN_N) << BRANCH_REGISTER_XN_S);
        case CONDITIONAL:
            result = BRANCH_CONDITIONAL;
            if (branch.branch.conditional.simm19.isLabel) {
                BitData *address = NULL;
                getMapping(state, branch.branch.conditional.simm19.data.label, address);
                assertFatal(address != NULL, "[writeBranch] No mapping for label!");
                result |= truncate(*address, BRANCH_CONDITIONAL_SIMM19_N);
            } else {
                result |= truncate(branch.branch.conditional.simm19.data.immediate, BRANCH_CONDITIONAL_SIMM19_N);
            }

            return result | truncate(branch.branch.conditional.cond, BRANCH_CONDITIONAL_COND_N);
    }
}
