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
                "[parseLoadStore] Incorrect number of operands!");

    LoadStore_IR loadStoreIR;

    bool sf;
    const uint8_t reg = parseRegisterStr(line->operands[0], &sf);

    if (line->operands[1][0] == '[') {
        // SDT
        bool u = false;
        enum AddressingMode mode;
        union Offset offset;
        uint8_t xn;
        assertFatal(sscanf(line->operands[1], "[%*c%" SCNu8, &xn) == 1,
                    "[parseLoadStore] Could not scan <xn>!");
        if (line->operandCount == 2) {
            // Zero Unsigned Offset
            u = true;
            mode = UNSIGNED_OFFSET;
            offset.uoffset = 0;
        } else {
            char* lastOperand = line->operands[2];

            switch (lastOperand[strlen(lastOperand)-1]) {
                case '!':
                    // Pre-Index
                    mode = PRE_INDEXED;
                    offset.prePostIndex.i = true;
                    offset.prePostIndex.simm9 = parseImmediateStr(line->operands[2],
                                                                  LOAD_STORE_DATA_SIMM9_INDEXED_N);
                    break;
                case ']':
                    if (lastOperand[0] == '#') {
                        // Unsigned Offset
                        mode = UNSIGNED_OFFSET;
                        u = true;
                        offset.uoffset = parseImmediateStr(line->operands[2],
                                                           LOAD_STORE_DATA_OFFSET_N);
                    } else {
                        //Register Offset
                        mode = REGISTER_OFFSET;
                        offset.xm = parseRegisterStr(line->operands[2], NULL);
                    }
                    break;
                default:
                    // Post-Index
                    mode = POST_INDEXED;
                    offset.prePostIndex.i = false;
                    offset.prePostIndex.simm9 = parseImmediateStr(line->operands[2],
                                                                  LOAD_STORE_DATA_SIMM9_INDEXED_N);
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

    assertFatal(irObject->type == LOAD_STORE,
                "[translateLoadStore] Received non-single data transfer IR!");
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
                    // Divide by 8 if registers accessed as 64-bit, otherwise (if 32-bit) divide by 4
                    result |= loadStore->data.sdt.offset.uoffset << (LOAD_STORE_DATA_OFFSET_S - (2 + loadStore->sf));
                    break;

                case PRE_INDEXED:
                case POST_INDEXED:
                    result |= LOAD_STORE_DATA_PRE_POST_INDEX;
                    result |= loadStore->data.sdt.offset.prePostIndex.i << LOAD_STORE_DATA_I_INDEXED_S;
                    result |= loadStore->data.sdt.offset.prePostIndex.simm9 << LOAD_STORE_DATA_SIMM9_INDEXED_S;
                    break;

                case REGISTER_OFFSET:
                    result |= LOAD_STORE_DATA_OFFSET_REGISTER;
                    result |= loadStore->data.sdt.offset.xm << LOAD_STORE_DATA_XM_REGISTER_S;
                    break;
            }

            result |= loadStore->data.sdt.xn << LOAD_STORE_DATA_XN_S;
            result |= loadStore->rt;
            break;

        case LOAD_LITERAL:
            result  = LOAD_STORE_LITERAL;
            result |= loadStore->sf << LOAD_STORE_SF_S;

            Literal *simm19 = &loadStore->data.simm19;
            if (simm19->isLabel) {
                // Calculate offset, then divide by 4 to encode.
                BitData *immediate = getMapping(state, simm19->data.label);
                assertFatal(immediate != NULL, "[translateLoadStore] No mapping for label!");

                simm19->data.immediate = *immediate;
                simm19->data.immediate -= state->address;
                simm19->data.immediate /= 4;
            }

            result |= truncater(simm19->data.immediate, LOAD_STORE_LITERAL_SIMM19_N)
                    << LOAD_STORE_LITERAL_SIMM19_S;
            result |= truncater(loadStore->rt, LOAD_STORE_RT_N);
            break;
    }
    return result;
}
