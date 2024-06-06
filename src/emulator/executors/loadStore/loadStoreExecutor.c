///
/// loadStoreExecutor.c
/// Execute a load and store instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#include "loadStoreExecutor.h"

/// Executes an [IR] of a data processing (immediate) instruction.
/// @param immediateIR The instruction to execute.
/// @param registers The current virtual registers.
/// @param memory The current virtual memory.
void executeLoadStore(IR *irObject, Registers registers, Memory memory) {

    assertFatal(irObject->type == LOAD_STORE,
                "[executeImmediate] Received non-immediate instruction!");

    LoadStore_IR *loadStoreIR = &irObject->ir.loadStoreIR;

    // Initialise the transfer address
    int64_t transferAddress = getReg(registers, loadStoreIR->data.sdt.xn);

    // Initialise write-back information
    bool writeBack = false;
    BitData writeBackValue;

    // Keep track of whether the operation is a load or a store
    bool isLoad = true;

    // Switch over the instruction type (SDT or LL)
    switch (loadStoreIR->type) {
        case SINGLE_DATA_TRANSFER:
            // Use Xn as the base register for the transfer address
            transferAddress = getReg(registers, loadStoreIR->data.sdt.xn);

            // Determine if the instruction is a load or a store
            isLoad = loadStoreIR->data.sdt.l;

            // Switch over the addressing modes
            switch (loadStoreIR->data.sdt.addressingMode) {
                case REGISTER_OFFSET:
                    transferAddress += getReg(registers, loadStoreIR->data.sdt.offset.xm);
                    break;

                case PRE_INDEXED:
                    writeBack = true;
                    int64_t simm9Pre = loadStoreIR->data.sdt.offset.prePostIndex.simm9;
                    transferAddress += signExtend(simm9Pre, 8 * sizeof(uint16_t));
                    writeBackValue = transferAddress;
                    break;

                case POST_INDEXED:
                    writeBack = true;
                    int64_t simm9Post = loadStoreIR->data.sdt.offset.prePostIndex.simm9;
                    writeBackValue = transferAddress + signExtend(simm9Post, 8 * sizeof(uint16_t));
                    break;

                case UNSIGNED_OFFSET:
                    transferAddress += loadStoreIR->sf ?
                                       loadStoreIR->data.sdt.offset.uoffset * 8 :
                                       loadStoreIR->data.sdt.offset.uoffset * 4;
                    break;

                default: throwFatal("Not implemented!");
            }
            break;

        case LOAD_LITERAL:
            transferAddress = getRegPC(registers);
            int64_t simm19 = loadStoreIR->data.simm19.data.immediate;
            int64_t simm19Extended = signExtend(simm19, 8 * sizeof(uint32_t));
            transferAddress += simm19Extended * 4;
            break;
    }

    if (isLoad) {
        BitData bitsToStore = readMem(memory, loadStoreIR->sf, transferAddress);
        setReg(registers, loadStoreIR->rt, loadStoreIR->sf, bitsToStore);
    } else {
        BitData bitsToLoad = getReg(registers, loadStoreIR->rt);
        writeMem(memory, loadStoreIR->sf, transferAddress, bitsToLoad);
    }

    if (writeBack) {
        // Must be single data transfer
        setReg(registers, loadStoreIR->data.sdt.xn, true, writeBackValue);
    }
}
