///
/// branchHandler.h
/// Functions to parse from assembly and write as binary a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "branchHandler.h"

struct {
    char *name;
    enum BranchCondition condition;
} static const conditionMappings[] = {
        { "eq", EQ },
        { "ne", NE },
        { "ge", GE },
        { "lt", LT },
        { "gt", GT },
        { "le", LE },
        { "al", AL },
        { NULL },
};

/// Translates an assembly branch instruction into its IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @return The IR form of the branch instruction.
/// @pre [line]'s mnemonic is at least one of "b", "br", or "b.COND".
IR parseBranch(TokenisedLine line, AssemblerState *state) {
    assertFatal(line.operandCount == 1, "[parseBranch] Incorrect number of operands!");

    Branch_IR branchIR;
    if (!strcmp(line.mnemonic, "b")) {
        const Literal simm26 = parseLiteral(line.operands[0]);
        branchIR = (Branch_IR) { BRANCH_UNCONDITIONAL, .data.simm26 = simm26 };
    } else if (!strcmp(line.mnemonic, "br")) {
        uint8_t xn = parseRegister(line.operands[0], NULL);
        branchIR = (Branch_IR) { BRANCH_REGISTER, .data.xn = xn };
    } else {
        // Get just the condition string.
        char *name = line.mnemonic;
        name += 2;

        // Try to find the corresponding condition, else throw error.
        bool found = false;
        enum BranchCondition condition = AL; // Default value - will be caught by assert.
        for (int i = 0; conditionMappings[i].name != NULL; i++) {
            if (!strcasecmp(conditionMappings[i].name, name)) {
                condition = conditionMappings[i].condition;
                found = true;
                break;
            }
        }

        assertFatal(found, "[parseBranch] Invalid condition code!");
        const Literal simm19 = parseLiteral(line.operands[0]);
        branchIR = (Branch_IR) { BRANCH_CONDITIONAL, .data.conditional = { simm19, condition } };
    }

    return (IR) { BRANCH, .ir.branchIR = branchIR };
}

/// Converts the IR form of an Branch instruction to a binary word.
/// @param irObject The [IR] struct representing the instruction.
/// @param state The current state of the assembler.
/// @return 32-bit binary word of the instruction.
BitInst translateBranch(IR irObject, AssemblerState *state) {
    assertFatal(irObject.type == BRANCH, "[writeBranch] Received non-branch IR!");
    Branch_IR branch = irObject.ir.branchIR;
    BitInst result;

    switch (branch.type) {
        case BRANCH_UNCONDITIONAL:
            result = BRANCH_UNCONDITIONAL_C;

            Literal simm26 = branch.data.simm26;
            if (simm26.isLabel) {
                BitData *address = NULL;
                address = getMapping(state, simm26.data.label);
                assertFatal(address != NULL, "[writeBranch] No mapping for label!");
                return result | truncate(*address, BRANCH_UNCONDITIONAL_SIMM26_N);
            }

            return result | truncate(simm26.data.immediate, BRANCH_UNCONDITIONAL_SIMM26_N);
        case BRANCH_REGISTER:
            result = BRANCH_REGISTER_C;
            return result | (truncate(branch.data.xn, BRANCH_REGISTER_XN_N) << BRANCH_REGISTER_XN_S);
        case BRANCH_CONDITIONAL:
            result = BRANCH_CONDITIONAL_C;

            struct Conditional conditional = branch.data.conditional;
            if (conditional.simm19.isLabel) {
                BitData *address = NULL;
                address = getMapping(state, conditional.simm19.data.label);
                assertFatal(address != NULL, "[writeBranch] No mapping for label!");
                result |= truncate(*address, BRANCH_CONDITIONAL_SIMM19_N);
            } else {
                result |= truncate(conditional.simm19.data.immediate, BRANCH_CONDITIONAL_SIMM19_N);
            }

            return result | truncate(conditional.condition, BRANCH_CONDITIONAL_COND_N);
    }

    throwFatal("[writeBranch] Unknown type of branch instruction!");
    return 0x0; // For the compiler.
}
