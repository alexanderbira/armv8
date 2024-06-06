///
/// loadStoreDecoder.c
/// Decode a load and store instruction to its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 27/05/2024.
///

#include "loadStoreDecoder.h"

/// Decode a load and store-group instruction to its IR
/// @param word The instruction to decode
/// @returns The IR of word
IR decodeLoadStore(Instruction word) {

    LoadStore_IR loadStoreIR = (LoadStore_IR) {
            .sf = decompose(word, LOAD_STORE_SF_M),
            .rt = decompose(word, LOAD_STORE_RT_M),
    };

    // Decode the rest of the instruction as an SDT or an LL
    if ((word & LOAD_STORE_DATA_M) == LOAD_STORE_DATA_B) {

        struct SingleDataTransfer data = (struct SingleDataTransfer) {
                .u = decompose(word, LOAD_STORE_DATA_U_M),
                .l = decompose(word, LOAD_STORE_DATA_L_M),
                .xn = decompose(word, LOAD_STORE_DATA_XN_M),
        };

        Component offsetComponent = decompose(word, LOAD_STORE_DATA_OFFSET_M);

        union Offset offset;

        if (data.u == 1) {

            // Unsigned offset addressing mode
            data.addressingMode = UNSIGNED_OFFSET;
            offset.uoffset = offsetComponent;

        } else if ((offsetComponent & LOAD_STORE_DATA_OFFSET_REGISTER_M) == LOAD_STORE_DATA_OFFSET_REGISTER_C) {

            // Register offset addressing mode
            data.addressingMode = REGISTER_OFFSET;
            offset.xm = decompose(word, LOAD_STORE_DATA_XM_REGISTER_M);

        } else if ((offsetComponent & LOAD_STORE_DATA_OFFSET_INDEXED_M) == LOAD_STORE_DATA_OFFSET_INDEXED_C) {

            // Pre/post-index addressing mode
            struct PrePostIndex prePostIndex;

            // Get the 9-bit address offset as a 16-bit unsigned integer
            int16_t simm9 = decompose(word, LOAD_STORE_DATA_SIMM9_INDEXED_M);
            prePostIndex.simm9 = (int16_t) signExtend(simm9, LOAD_STORE_DATA_SIMM9_INDEXED_N);
            prePostIndex.i = decompose(word, LOAD_STORE_DATA_I_INDEXED_M);
            offset.prePostIndex = prePostIndex;
            data.addressingMode = (prePostIndex.i == 1) ? PRE_INDEXED : POST_INDEXED;

        } else {

            // Invalid addressing mode
            throwFatal("[decodeLoadStore] Invalid addressing mode!");

        }

        data.offset = offset;
        // Save the SDT to the IR
        loadStoreIR.data.sdt = data;

    } else if ((word & LOAD_STORE_LITERAL_M) == LOAD_STORE_LITERAL_B) {

        // Get the 19-bit offset as a 32-bit unsigned integer
        int32_t offset = decompose(word, LOAD_STORE_LITERAL_SIMM19_M);
        offset = signExtend(offset, LOAD_STORE_LITERAL_SIMM19_N);

        loadStoreIR.type = LOAD_LITERAL;
        loadStoreIR.data.simm19.data.immediate = offset;

    } else {

        throwFatal("[decodeLoadStore] Invalid instruction format!");

    }

    return (IR) {.type = LOAD_STORE, .ir.loadStoreIR = loadStoreIR};
}
