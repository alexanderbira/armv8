///
/// loadStoreExecutor.c
/// Execute a load and store instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#include "loadStoreExecutor.h"

/// Execute a load and store instruction from its intermediate representation
/// @param ir The IR of the instruction to execute
/// @param regs A pointer to the registers
/// @param mem A pointer to the memory
void executeLoadStore(IR *irObject, Registers regs, Memory mem) {

    assertFatal(irObject->type == LOAD_STORE,
                "[executeImmediate] Received non-immediate instruction!");

    LoadStore_IR *loadStoreIR = &irObject->ir.loadStoreIR;

    // Initialise the transfer address
    int64_t transferAddress = getReg(regs, loadStoreIR->data.sdt.xn);

    // Initialise write-back information
    bool writeBack = false;
    BitData writeBackValue;

    // Keep track of whether the operation is a load or a store
    bool isLoad = true;

    // Switch over the instruction type (SDT or LL)
    switch (loadStoreIR->type) {

        case SINGLE_DATA_TRANSFER:

            // Use Xn as the base register for the transfer address
            transferAddress = getReg(regs, loadStoreIR->data.sdt.xn);

            // Determine if the instruction is a load or a store
            isLoad = loadStoreIR->data.sdt.l;

            // Switch over the addressing modes
            switch (loadStoreIR->data.sdt.addressingMode) {

                case REGISTER_OFFSET:
                    transferAddress += getReg(regs, loadStoreIR->data.sdt.offset.xm);
                    break;

                case PRE_INDEXED:
                    writeBack = true;
                    int64_t simm9Pre = loadStoreIR->data.sdt.offset.prePostIndex.simm9;
                    // Sign-extend simm9 from 16 to 64 bits (48 = 64-16)
                    transferAddress += (simm9Pre << 48) >> 48;
                    writeBackValue = transferAddress;
                    break;

                case POST_INDEXED:
                    writeBack = true;
                    int64_t simm9Post = loadStoreIR->data.sdt.offset.prePostIndex.simm9;
                    // Sign-extend simm9 from 16 to 64 bits (48 = 64-16)
                    writeBackValue = transferAddress + ((simm9Post << 48) >> 48);
                    break;

                case UNSIGNED_OFFSET:
                    transferAddress += loadStoreIR->sf ?
                                       loadStoreIR->data.sdt.offset.uoffset * 8 :
                                       loadStoreIR->data.sdt.offset.uoffset * 4;
                    break;

                case LITERAL:
                    throwFatal("[executeLoadStore] Not implemented!");

            }

            break;

        case LOAD_LITERAL:
            transferAddress = getRegPC(regs);
            int64_t simm19 = loadStoreIR->data.simm19.data.immediate;
            // Sign-extend simm19 from 32 to 64 bits (32 = 64-32)
            int64_t simm19Extended = (simm19 << 32) >> 32;
            transferAddress += simm19Extended * 4;
            break;

    }

    if (isLoad) {

        BitData bitsToStore = readMem(mem, loadStoreIR->sf, transferAddress);
        setReg(regs, loadStoreIR->rt, loadStoreIR->sf, bitsToStore);

    } else {

        BitData bitsToLoad = getReg(regs, loadStoreIR->rt);
        writeMem(mem, loadStoreIR->sf, transferAddress, bitsToLoad);

    }

    if (writeBack) {

        // Must be a SDT
        setReg(regs, loadStoreIR->data.sdt.xn, true, writeBackValue);

    }

}
