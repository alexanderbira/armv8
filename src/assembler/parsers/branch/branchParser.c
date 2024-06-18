///
/// branchParser.c
/// Transform a [TokenisedLine] to an [IR] of a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "branchParser.h"

/// The mappings between condition strings and condition codes.
static const BranchEntry mappings[] = {
    { "al", AL },
    { "eq", EQ },
    { "ge", GE },
    { "gt", GT },
    { "le", LE },
    { "lt", LT },
    { "ne", NE },
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

/// Transform a [TokenisedLine] to an [IR] of a branch instruction.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @returns The [IR] form of the branch instruction.
/// @pre The [line]'s mnemonic is that of a branch instruction.
IR parseBranch(TokenisedLine *line, unused AssemblerState *state) {
    assertFatal(line->operandCount == 1, "Incorrect number of operands; branch instructions need 1!");
    Branch_IR branchIR;

    if (!strcmp(line->mnemonic, "b")) {
        // Either branch unconditional or conditional
        const Literal simm = parseLiteral(line->operands[0]);

        if (line->subMnemonic == NULL) {
            // Branch unconditional
            branchIR = (Branch_IR) { .type = BRANCH_UNCONDITIONAL, .data.simm26 = simm };
        } else {
            // Branch conditional
            BranchEntry target = (BranchEntry) { line->subMnemonic, .code = -1 }; // Throwaway target.
            BranchEntry *condition = bsearch(&target, mappings,
                                             sizeof(mappings) / sizeof(BranchEntry), sizeof(BranchEntry), branchCmp);
            assertFatalNotNullWithArgs(condition, "Invalid condition code <%s>!", line->subMnemonic);

            branchIR = (Branch_IR) {
                .type = BRANCH_CONDITIONAL,
                .data.conditional = { .simm19 = simm, .condition = condition->code }
            };
        }
    } else if (!strcmp(line->mnemonic, "br")) {
        // Branch register
        uint8_t xn = parseRegisterStr(line->operands[0], NULL);
        branchIR = (Branch_IR) { .type = BRANCH_REGISTER, .data.xn = xn };
    } else {
        throwFatalWithArgs("Received invalid branch instruction <%s>!", line->mnemonic);
    }

    return (IR) { .type = BRANCH, .ir.branchIR = branchIR };
}
