///
/// loadStoreDecode.c
/// Decode a load and store instruction to its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 27/05/2024.
///

#include "loadStoreDecode.h"
#include <stdio.h>
#include <stdlib.h>

/// Decode a load and store-group instruction to its IR
/// @param word The instruction to decode
/// @returns The IR of word
IR decodeLoadStore(Instruction word) {
    // Initialise the IR
    IR ir;
    ir.type = LS;
    LS_IR loadStoreIR;

    // Get the values common to both SDT and LL
    loadStoreIR.sf = decompose(word, LS_SF_M);
    loadStoreIR.rt = decompose(word, LS_RT_M);

    // Decode the rest of the instruction as an SDT or an LL
    if ((decompose(word, SDT_M) & SDT_WF_M) == SDT_C) {
        // Single data transfer instruction
        loadStoreIR.ls = SDT;

        // Initialise the SDT struct
        struct Sdt sdt;

        // Set the values common to all addressing modes
        sdt.u = decompose(word, SDT_U_M);
        sdt.l = decompose(word, SDT_L_M);
        sdt.xn = decompose(word, SDT_XN_M);

        // Get the offset from the instruction, used in determining addressing
        // mode
        Component offsetComponent = decompose(word, SDT_OFFSET_M);

        // Initialise the offset
        union Offset offset;

        // Decode the offset depending on the addressing mode
        if (sdt.u == 1) {
            // Unsigned offset addressing mode
            sdt.addressingMode = UNSIGNED_OFFSET;
            offset.uoffset = offsetComponent;

        } else if ((offsetComponent & SDT_RO_M) == SDT_RO_C) {
            // Register offset addressing mode
            sdt.addressingMode = REGISTER_OFFSET;
            offset.xm = decompose(offsetComponent, SDT_RO_XM_M);


        } else if ((offsetComponent & SDT_PPI_M) == SDT_PPI_C) {
            // Pre/post-index addressing mode

            // Initialise pre/post-index
            struct PrePostIndex prePostIndex;

            // Get the 9-bit address offset as a 16-bit unsigned integer
            uint16_t simm9 = decompose(offsetComponent, SDT_PPI_SIMM9_M);
            // Sign-extend the address offset (16 - 9 = 7)
            prePostIndex.simm9 = (simm9 << 7) >> 7;

            // Determine which of pre/post-index this addressing mode uses
            prePostIndex.i = decompose(offsetComponent, SDT_PPI_I_M);

            if (prePostIndex.i == 1) {
                // Pre-indexed addressing mode
                sdt.addressingMode = PRE_INDEXED;
            } else {
                // Post-indexed addressing mode
                sdt.addressingMode = POST_INDEXED;
            }

        } else {
            // Invalid addressing mode
            fprintf(stderr, "Invalid addressing mode");
            exit(-1);
        }

        // Save the SDT to the IR
        loadStoreIR.lsConsts.sdt = sdt;


    } else if ((decompose(word, LL_M) & LL_WF_M) == LL_C) {
        // Load literal instruction
        loadStoreIR.ls = LL;

        // Get the 19-bit offset as a 32-bit unsigned integer
        uint32_t offset = decompose(word, LL_SIMM19_M);
        // Sign-extend the offset (32 - 19 = 13)
        loadStoreIR.lsConsts.simm19 = (offset << 13) >> 13;


    } else {
        // word is not a well-formed load-store instruction
        fprintf(stderr, "Invalid instruction format");
        exit(-1);
    }

    // Save the load/store IR to the IR
    ir.repr.ls = loadStoreIR;

    return ir;
}
