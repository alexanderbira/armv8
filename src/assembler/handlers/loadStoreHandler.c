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
    const uint8_t reg = parseRegisterStr(line->operands[0], &sf);

    // Only a <literal> or <unsigned offset> is possible.
    if (line->operands[1][0] == '[') {
        // SDT
        bool u = false;
        enum AddressingMode mode;
        union Offset offset;
        uint8_t xn;
        assertFatal(sscanf("[*%" SCNu8, line->operands[1], &xn) == 1,
                    "[parseLoadStore] Could not scan <xn>!");

        if (line->operandCount == 2) {
            // Zero Unsigned Offset
            u = true;
            mode = UNSIGNED_OFFSET;
            offset.uoffset = 0;



        } else {

            char* lastOperand = line->operands[2];

            switch (lastOperand[strlen(lastOperand)-1]) {

                case '!': {
                    // Pre-indexed
                    mode = PRE_INDEXED;
                    offset.prePostIndex.i = true;
                    int16_t simm9;
                    assertFatal(sscanf("#%" SCNd16, line->operands[2], &simm9) == 1,
                                "[parseLoadStore] Could not scan <simm>!");
                    offset.prePostIndex.simm9 = simm9;
                    break;
                }

                case ']':
                    if (lastOperand[0] == '#') {
                        // Unsigned Offset
                        u = true;
                        mode = UNSIGNED_OFFSET;
                        uint16_t imm12;
                        assertFatal(sscanf("#%" SCNu16, line->operands[2], &imm12) == 1,
                                    "[parseLoadStore] Could not scan <imm>!");
                        offset.uoffset = imm12;

                    } else {
                        //Register-offset
                        mode = REGISTER_OFFSET;
                        uint8_t xm;
                        assertFatal(sscanf("*%" SCNu8, line->operands[1], &xm) == 1,
                                    "[parseLoadStore] Could not scan <xm>!");
                        offset.xm = xm;
                    }
                    break;

                default: {
                    // Post-index
                    mode = POST_INDEXED;
                    offset.prePostIndex.i = false;
                    int16_t simm9;
                    assertFatal(sscanf("#%" SCNd16, line->operands[2], &simm9) == 1,
                                "[parseLoadStore] Could not scan <simm>!");
                    offset.prePostIndex.simm9 = simm9;
                }

            }
        }

        loadStoreIR = (LoadStore_IR) {
                .sf = sf,
                .type = SINGLE_DATA_TRANSFER,
                .data.sdt = {
                        .u = u,
                        .l = !strcmp(line->mnemonic, "ldr"),
                        .addressingMode = mode,
                        .offset = offset,
                        .xn = xn
                },
                .rt = reg
        };

    } else {
        // LL
        const Literal literal = parseLiteral(line->operands[1]);
        loadStoreIR = (LoadStore_IR) { sf, LOAD_LITERAL, .data.simm19 = literal, reg };
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
            result  = LOAD_STORE_DATA;
            result |= loadStore->sf << LOAD_STORE_SF_S;
            result |= loadStore->data.sdt.u << LOAD_STORE_DATA_U_S;
            result |= loadStore->data.sdt.l << LOAD_STORE_DATA_L_S;

            switch (loadStore->data.sdt.addressingMode) {
                case UNSIGNED_OFFSET:
                    result |= loadStore->data.sdt.offset.uoffset << LOAD_STORE_DATA_OFFSET_S;
                    break;

                case PRE_INDEXED:
                case POST_INDEXED:
                    result |= LOAD_STORE_DATA_PRE_POST_INDEX;
                    result |= loadStore->data.sdt.offset.prePostIndex.i << LOAD_STORE_DATA_I_INDEXED_S;
                    result |= loadStore->data.sdt.offset.prePostIndex.simm9 << LOAD_STORE_DATA_OFFSET_S;
                    break;

                case REGISTER_OFFSET:
                    result |= LOAD_STORE_DATA_OFFSET_REGISTER;
                    result |= loadStore->data.sdt.offset.xm << LOAD_STORE_DATA_XM_REGISTER_S;
                    break;
            }

            result |= loadStore->data.sdt.xn << LOAD_STORE_DATA_XN_S;
            result |= loadStore->rt;
            return result;

        case LOAD_LITERAL:
            result  = LOAD_STORE_LITERAL;
            result |= loadStore->sf << LOAD_STORE_SF_S;

            Literal *simm19 = &loadStore->data.simm19;
            if (simm19->isLabel) {
                BitData *address = NULL;
                address = getMapping(state, simm19->data.label);
                assertFatal(address != NULL, "[writeBranch] No mapping for label!");
                result |= truncater(*address, LOAD_STORE_LITERAL_SIMM19_N)
                          << LOAD_STORE_LITERAL_SIMM19_S;
            } else {
                result |= truncater(simm19->data.immediate, LOAD_STORE_LITERAL_SIMM19_N) << LOAD_STORE_LITERAL_SIMM19_S;
            }

            result |= truncater(loadStore->rt, LOAD_STORE_RT_N);
            return result;
    }
}
