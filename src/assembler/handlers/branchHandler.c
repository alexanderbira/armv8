///
/// branchHandler.h
/// Functions to parse from assembly and write as binary a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "branchHandler.h"

static struct BranchEntry {
    char *name;
    enum BranchCondition condition;
} const conditionMappings[] = {
        { "eq", EQ },
        { "ne", NE },
        { "ge", GE },
        { "lt", LT },
        { "gt", GT },
        { "le", LE },
        { "al", AL },
};

/// Translates an assembly branch instruction into its IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @return The IR form of the branch instruction.
/// @pre [line]'s mnemonic is at least one of "b", "br", or "b.COND".
IR parseBranch(TokenisedLine *line, unused AssemblerState *state) {

    assertFatal(line->operandCount == 1, "[parseBranch] Incorrect number of operands!");

    Branch_IR branchIR;

    if (!strcmp(line->mnemonic, "b")) {
        // Either branch unconditional or conditional.
        if (line->subMnemonic == NULL) {
            const Literal simm26 = parseLiteral(line->operands[0]);
            branchIR = (Branch_IR) {.type = BRANCH_UNCONDITIONAL, .data.simm26 = simm26};
        } else {
            // Try to find the corresponding condition, else throw error.
            bool found = false;
            enum BranchCondition condition = AL; // Default value - will be caught by assert.
            for (size_t i = 0; i < sizeof(conditionMappings) / sizeof(struct BranchEntry); i++) {
                if (!strcasecmp(conditionMappings[i].name, line->subMnemonic)) {
                    condition = conditionMappings[i].condition;
                    found = true;
                    break;
                }
            }

            assertFatal(found, "[parseBranch] Invalid condition code!");
            const Literal simm19 = parseLiteral(line->operands[0]);
            branchIR = (Branch_IR) {.type = BRANCH_CONDITIONAL, .data.conditional = {simm19, condition}};
        }
    } else if (!strcmp(line->mnemonic, "br")) {
        uint8_t xn = parseRegisterStr(line->operands[0], NULL);
        branchIR = (Branch_IR) {.type = BRANCH_REGISTER, .data.xn = xn};
    } else {
        throwFatal("[parseBranch] Received invalid branch instruction!");
    }

    return (IR) { BRANCH, .ir.branchIR = branchIR };
}

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
            result = BRANCH_UNCONDITIONAL_C;
            Literal *simm26 = &branch->data.simm26;
            if (simm26->isLabel) {
                // Calculate offset, then divide by 4 to encode.
                BitData *immediate = getMapping(state, simm26->data.label);
                assertFatal(immediate != NULL, "[translateBranch] No mapping for label!");

                simm26->data.immediate = *immediate;
                simm26->data.immediate -= state->address;
                simm26->data.immediate /= 4;
            }

            return result | truncater(simm26->data.immediate, BRANCH_UNCONDITIONAL_SIMM26_N);

        case BRANCH_REGISTER:
            result = BRANCH_REGISTER_C;
            return result | (truncater(branch->data.xn, BRANCH_REGISTER_XN_N) << BRANCH_REGISTER_XN_S);

        case BRANCH_CONDITIONAL:
            result = BRANCH_CONDITIONAL_C;
            Literal *simm19 = &branch->data.conditional.simm19;
            if (simm19->isLabel) {
                // Calculate offset, then divide by 4 to encode.
                BitData *immediate = getMapping(state, simm19->data.label);
                assertFatal(immediate != NULL, "[translateBranch] No mapping for label!");

                simm19->data.immediate = *immediate;
                simm19->data.immediate -= state->address;
                simm19->data.immediate /= 4;
            }

            result |= truncater(simm19->data.immediate, BRANCH_CONDITIONAL_SIMM19_N)
                    << BRANCH_CONDITIONAL_SIMM19_S;
            return result | truncater((uint64_t) &branch->data.conditional.condition, BRANCH_CONDITIONAL_COND_N);
    }

    throwFatal("[translateBranch] Unknown type of branch instruction!");
}
