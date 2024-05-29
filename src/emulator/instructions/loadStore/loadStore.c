///
/// loadStore.c
/// Decode and execute a load and store instruction
///
/// Created by Alexander Biraben-Renard on 27/05/2024.
///

#include "loadStore.h"
#include <stdio.h>

/// Decode and execute a load and store-group instruction
/// @param word The instruction to process
/// @param regs A pointer to the Registers struct
/// @param mem A pointer to the memory
void processLoadStore(Instruction word, Registers regs, Memory mem) {
    bool sf = decompose(word, LS_SF_M);
    size_t target_reg = decompose(word, LS_RT_M);


    if ((decompose(word, SDT_M) & SDT_WF_M) == SDT_C) {
        // Single data transfer instruction

        Component u = decompose(word, SDT_U_M);
        Component l = decompose(word, SDT_L_M);
        Component offset = decompose(word, SDT_OFFSET_M);
        Component xn = decompose(word, SDT_XN_M);

        // Whether we need to write back to Xn
        bool writeBack = false;
        // The value to write back to Xn, if a write back is required
        BitData writeBackVal;

        // The memory address to load from/store to, initialised to the value
        // of the xn register
        size_t transferAddress = getReg(regs, xn);


        if (u == 1) {
            // Unsigned offset addressing mode

            // In the 64-bit variant, uoffset is given by offset*8
            // In the 64-bit variant, uoffset is given by offset*4
            uint64_t uOffset = sf == 1 ? offset*8 : offset*4;
            transferAddress += uOffset;


        } else if ((offset & SDT_RO_M) == SDT_RO_C) {
            // Register offset addressing mode
            size_t xm = decompose(offset, SDT_RO_XM_M);
            transferAddress += getReg(regs, xm);


        } else if ((offset & SDT_PPI_M) == SDT_PPI_C) {
            // Pre/post-index addressing mode

            // Get the 9-bit offset as a 64-bit unsigned integer
            uint64_t simm9 = decompose(offset, SDT_PPI_SIMM9_M);
            // Sign-extend the offset (64 - 9 = 55)
            int64_t signedSimm9 = (simm9 << 55) >> 55;

            // Set the write-back variables
            writeBack = true;
            writeBackVal = transferAddress + signedSimm9;

            // Determine if it's pre or post-indexed
            Component i = decompose(offset, SDT_PPI_I_M);

            if (i == 1) {
                // Pre-indexed addressing mode
                transferAddress += signedSimm9;
            }

            // No change needed if the addressing mode is post-indexed


        } else {
            // Invalid addressing mode
            fprintf(stderr, "Invalid addressing mode");
            return;
        }


        // Perform the load/store
        if (l == 1) {
            // Load operation
            BitData loadData = readMem(mem, sf, transferAddress);
            setReg(regs, target_reg, sf, loadData);
        } else {
            // Store operation
            BitData storeData = getReg(regs, target_reg);
            writeMem(mem, sf, transferAddress, storeData);
        }

        // Perform a write back if required
        if (writeBack) {
            setReg(regs, xn, 1, writeBackVal);
        }


    } else if ((decompose(word, LL_M) & LL_WF_M) == LL_C) {
        // Load literal instruction

        int64_t pcValue = getRegPC(regs);

        // Get the 19-bit offset as a 64-bit unsigned integer
        uint64_t offset = decompose(word, LL_SIMM19_M);
        // Sign-extend the offset (64 - 19 = 45)
        int64_t signedOffset = (offset << 45) >> 45;

        // Load from the memory
        BitData loadValue = readMem(mem, sf, pcValue + signedOffset*4);

        // Write the loaded data to the target register
        setReg(regs, target_reg, sf, loadValue);


    } else {
        // word is not a well-formed load-store instruction
        fprintf(stderr, "Invalid instruction format");
        return;
    }
}
