///
/// singleDataTransferHandler.c
/// Functions to parse from assembly and write as binary a Single Data Transfer instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "singleDataTransferHandler.h"

/// Translates an assembly single data transfer instruction into its IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The IR form of the branch instruction.
/// @pre [line]'s mnemonic is one of "ldr" or "str".
IR parseSingleDataTransfer(TokenisedLine line, AssemblerState *state) {
    assertFatal(line.operandCount == 2 || line.operandCount == 3,
                "[parseSingleDataTransfer] Incorrect number of operands!");

    SDT_IR sdt;
    bool sf;
    const uint8_t reg = parseRegister(line.operands[0], &sf);

    if (line.operandCount == 2) {
        // Only a <literal> is possible.
        const Literal literal = parseLiteral(line.operands[1]);
        sdt = (SDT_IR) {sf, LL, .sdtGroup.simm19 = literal, reg};
    } else {
        bool u;
        enum AddressingMode mode;
        union Offset offset;
        uint8_t destination;

        // TODO: Finish parsing the offset.

        sdt = (SDT_IR) {
                sf, SDT,
                .sdtGroup.address = {u, !strcmp(line.mnemonic, "ldr"), mode, offset, destination},
                reg
        };
    }

    return (IR) {SINGLE_DATA_TRANSFER, .repr.sdt = sdt};
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
                address = getMapping(state, branch.branch.simm26.data.label);
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
                address = getMapping(state, branch.branch.conditional.simm19.data.label);
                assertFatal(address != NULL, "[writeBranch] No mapping for label!");
                result |= truncate(*address, BRANCH_CONDITIONAL_SIMM19_N);
            } else {
                result |= truncate(branch.branch.conditional.simm19.data.immediate, BRANCH_CONDITIONAL_SIMM19_N);
            }

            return result | truncate(branch.branch.conditional.cond, BRANCH_CONDITIONAL_COND_N);
    }
}