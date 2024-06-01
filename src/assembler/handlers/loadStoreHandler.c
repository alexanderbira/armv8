///
/// loadStoreHandler.c
/// Functions to parse from assembly and write as binary a Load Store instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "loadStoreHandler.h"

/// Translates an assembly single data transfer instruction into its IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The IR form of the branch instruction.
/// @pre [line]'s mnemonic is one of "ldr" or "str".
IR parseLoadStore(TokenisedLine *line, unused AssemblerState *state) {
    assertFatal(line->operandCount == 2 || line->operandCount == 3,
                "[parseSingleDataTransfer] Incorrect number of operands!");

    LoadStore_IR loadStoreIR;

    bool sf;
    const uint8_t reg = parseRegister(line->operands[0], &sf);

    if (line->operandCount == 2) {
        // Only a <literal> is possible.
        const Literal literal = parseLiteral(line->operands[1]);
        loadStoreIR = (LoadStore_IR) { sf, LOAD_LITERAL, .data.simm19 = literal, reg };
    } else {
        bool u;
        enum AddressingMode mode;
        union Offset offset;
        uint8_t destination;

        // TODO: Finish parsing the offset.
        // Dummy initialisations to proceed with debug of warnings.
        u = false;
        mode = ZERO_UNSIGNED_OFFSET;
        offset.xm = 0;
        destination = 0;

        loadStoreIR = (LoadStore_IR) {
                .sf = sf,
                .type = SINGLE_DATA_TRANSFER,
                .data.sdt = {
                        .u = u,
                        .l = !strcmp(line->mnemonic, "ldr"),
                        .addressingMode = mode,
                        .offset = offset,
                        .xn = destination
                },
                .rt = reg
        };
    }

    return (IR) { LOAD_STORE, .ir.loadStoreIR = loadStoreIR };
}

/// Converts the IR form of an Load Store instruction to a binary word.
/// @param irObject The [IR] struct representing the instruction.
/// @param state The current state of the assembler.
/// @return 32-bit binary word of the instruction.
Instruction translateLoadStore(IR *irObject, AssemblerState *state) {
    assertFatal(irObject->type == SINGLE_DATA_TRANSFER,
                "[writeSingleDataTransfer] Received non-single data transfer IR!");
    LoadStore_IR *loadStore = &irObject->ir.loadStoreIR;
    Instruction result;

    switch (loadStore->type) {
        case SINGLE_DATA_TRANSFER:
            // TODO: Implement
        case LOAD_LITERAL:
            result = LOAD_STORE_LITERAL;
            result |= loadStore->sf << LOAD_STORE_SF_S; // Trust since boolean.

            Literal *simm19 = &loadStore->data.simm19;
            if (simm19->isLabel) {
                BitData *address = NULL;
                address = getMapping(state, simm19->data.label);
                assertFatal(address != NULL, "[writeBranch] No mapping for label!");
                result |= truncater(*address, LOAD_STORE_LITERAL_SIMM19_N)
                          << LOAD_STORE_LITERAL_SIMM19_S;
            }
            result |= truncater(simm19->data.immediate, LOAD_STORE_LITERAL_SIMM19_N) << LOAD_STORE_LITERAL_SIMM19_S;
            return result | truncater(loadStore->rt, LOAD_STORE_RT_N);
    }
}
