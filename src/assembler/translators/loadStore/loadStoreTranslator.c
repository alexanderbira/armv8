///
/// loadStoreHandler.c
/// Transform a [IR] of a Load-Store instruction to a binary instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "loadStoreTranslator.h"

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
                    result |= truncater(loadStore->data.sdt.offset.prePostIndex.simm9, LOAD_STORE_DATA_SIMM9_INDEXED_N)
                            << LOAD_STORE_DATA_SIMM9_INDEXED_S;
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
                parseOffset(&simm19->data, state);
            }

            result |= truncater(simm19->data.immediate, LOAD_STORE_LITERAL_SIMM19_N)
                    << LOAD_STORE_LITERAL_SIMM19_S;
            result |= truncater(loadStore->rt, LOAD_STORE_RT_N);
            break;
    }
    return result;
}
