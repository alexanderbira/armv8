///
/// branchParser.c
/// Transform a [TokenisedLine] to an [IR] of a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "branchParser.h"

static const BranchEntry mappings[] = {
        {"al", AL},
        {"eq", EQ},
        {"ge", GE},
        {"gt", GT},
        {"le", LE},
        {"lt", LT},
        {"ne", NE},
};

/// Performs [strcmp] on the [mnemonic]s of [BranchEntry]s, but takes in [void *]s.
/// @param v1 The first item.
/// @param v2 The second item.
/// @returns [int] of comparison.
static int branchCmp(const void *v1, const void *v2) {
    const BranchEntry *p1 = (const BranchEntry *) v1;
    const BranchEntry *p2 = (const BranchEntry *) v2;
    return strcmp(p1->subMnemonic, p2->subMnemonic);
}

/// Transform a [TokenisedLine] to an [IR] of a Branch instruction.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @return The IR form of the branch instruction.
/// @pre The [line] mnemonic is that of a Branch instruction.
IR parseBranch(TokenisedLine *line, unused AssemblerState *state) {
    assertFatal(line->operandCount == 1, "Incorrect number of operands!");
    Branch_IR branchIR;

    if (!strcmp(line->mnemonic, "b")) {
        // Either branch unconditional or conditional.
        const Literal simm = parseLiteral(line->operands[0]);

        if (line->subMnemonic == NULL) {
            branchIR = (Branch_IR) {.type = BRANCH_UNCONDITIONAL, .data.simm26 = simm};
        } else {
            BranchEntry *condition = bsearch(line->subMnemonic, mappings,
                                             sizeof(mappings) / sizeof(BranchEntry), sizeof(BranchEntry), branchCmp);
            assertFatal(condition != NULL, "Invalid condition code!");

            branchIR = (Branch_IR) {
                    .type = BRANCH_CONDITIONAL,
                    .data.conditional = {simm, condition->code}
            };
        }
    } else if (!strcmp(line->mnemonic, "br")) {
        // Branch register
        uint8_t xn = parseRegisterStr(line->operands[0], NULL);
        branchIR = (Branch_IR) {.type = BRANCH_REGISTER, .data.xn = xn};
    } else {
        throwFatal("Received invalid branch instruction!");
    }

    return (IR) {.type = BRANCH, .ir.branchIR = branchIR};
}
