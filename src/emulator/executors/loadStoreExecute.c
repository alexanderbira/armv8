///
/// loadStoreExecute.c
/// Execute a load and store instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#include "loadStoreExecute.h"

/// Execute a load and store instruction from its intermediate representation
/// @param ir The IR of the instruction to execute
/// @param regs A pointer to the registers
/// @param mem A pointer to the memory
void executeLoadStore(LS_IR lsIr, Registers regs, Memory mem) {

    // Initialise the transfer address
    int64_t transferAddress;

    // Initialise write-back information
    bool writeBack = false;
    BitData writeBackValue;

    // Keep track of whether the operation is a load or a store
    bool isLoad = true;

    // Switch over the instruction type (SDT or LL)
    switch (lsIr.ls) {

        case SDT:
            // Use Xn as the base register for the transfer address
            transferAddress = getReg(regs, lsIr.lsConsts.sdt.xn);

            // Determine if the instruction is a load or a store
            isLoad = lsIr.lsConsts.sdt.l;

            // Switch over the addressing modes
            switch (lsIr.lsConsts.sdt.addressingMode) {

                case REGISTER_OFFSET:
                    transferAddress += getReg(regs, lsIr.lsConsts.sdt.offset.xm);
                    break;

                case PRE_INDEXED:
                    writeBack = true;

                    uint64_t simm9Pre = lsIr.lsConsts.sdt.offset.prePostIndex.simm9;
                    // Sign-extend simm9 from 16 to 64 bits (48 = 64-16)
                    transferAddress += (simm9Pre << 48) >> 48;

                    writeBackValue = transferAddress;
                    break;

                case POST_INDEXED:
                    writeBack = true;

                    uint64_t simm9Post = lsIr.lsConsts.sdt.offset.prePostIndex.simm9;
                    // Sign-extend simm9 from 16 to 64 bits (48 = 64-16)
                    writeBackValue = transferAddress + ((simm9Post << 48) >> 48);

                    break;

                case UNSIGNED_OFFSET:
                    transferAddress += lsIr.sf ?
                            lsIr.lsConsts.sdt.offset.uoffset * 8 :
                            lsIr.lsConsts.sdt.offset.uoffset * 4;
                    break;

            }

            break;

        case LL:
            transferAddress = getRegPC(regs);

            uint64_t simm19 = lsIr.lsConsts.simm19;
            // Sign-extend simm19 from 32 to 64 bits (32 = 64-32)
            int64_t simm19Extended = (simm19 << 32) >> 32;

            transferAddress += simm19Extended * 4;
            break;
    }

    if (isLoad) {
        BitData bitsToLoad = getReg(regs, lsIr.rt);
        writeMem(mem, lsIr.sf, transferAddress, bitsToLoad);
    } else {
        BitData bitsToStore = readMem(mem, lsIr.sf, transferAddress);
        setReg(regs, lsIr.rt, lsIr.sf, bitsToStore);
    }

    if (writeBack) {
        // Must be a SDT
        setReg(regs, lsIr.lsConsts.sdt.xn, true, writeBackValue);
    }
}
