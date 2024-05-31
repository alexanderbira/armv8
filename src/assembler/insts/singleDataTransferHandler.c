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

BitInst writeSingleDataTransfer(IR ir, AssemblerState *state) {
    assertFatal(ir.type == SINGLE_DATA_TRANSFER, "[writeSingleDataTransfer] Received non-single data transfer IR!");
    SDT_IR sdt = ir.repr.sdt;
    BitInst result;

    switch (sdt.type) {
        case SDT:
        case LL:
            result = SINGLE_DATA_TRANSFER_LITERAL;
            result |= sdt.sf << SINGLE_DATA_TRANSFER_SF_S; // Trust since boolean.
            if (sdt.sdtGroup.simm19.isLabel) {
                BitData *address = NULL;
                address = getMapping(state, sdt.sdtGroup.simm19.data.label);
                assertFatal(address != NULL, "[writeBranch] No mapping for label!");
                result |= truncate(*address, SINGLE_DATA_TRANSFER_LITERAL_SIMM19_N)
                        << SINGLE_DATA_TRANSFER_LITERAL_SIMM19_S;
            }
            result |= truncate(sdt.sdtGroup.simm19.data.immediate, SINGLE_DATA_TRANSFER_LITERAL_SIMM19_N)
                    << SINGLE_DATA_TRANSFER_LITERAL_SIMM19_S;
            return result | truncate(sdt.rt, SINGLE_DATA_TRANSFER_RT_N);
    }
}