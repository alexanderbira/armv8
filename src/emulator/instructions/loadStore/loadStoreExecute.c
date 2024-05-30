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
void executeLoadStore(IR ir, Registers regs, Memory mem) {
    assert(ir.type == LS);

    // Initialise the transfer address
    int64_t transferAddress;

    // Initialise write-back information
    bool writeBack = false;
    BitData writeBackValue;

    // Keep track of whether the operation is a load or a store
    bool isLoad = true;

    // Switch over the instruction type (SDT or LL)
    switch (ir.repr.ls.ls) {

        case SDT:
            // Use Xn as the base register for the transfer address
            transferAddress = getReg(regs, ir.repr.ls.lsConsts.sdt.xn);

            // Determine if the instruction is a load or a store
            isLoad = ir.repr.ls.lsConsts.sdt.l;

            // Switch over the addressing modes
            switch (ir.repr.ls.lsConsts.sdt.addressingMode) {

                case REGISTER_OFFSET:
                    transferAddress += getReg(regs, ir.repr.ls.lsConsts.sdt.offset.xm);
                    break;

                case PRE_INDEXED:
                    writeBack = true;

                    uint64_t simm9Pre = ir.repr.ls.lsConsts.sdt.offset.prePostIndex.simm9;
                    // Sign-extend simm9 from 16 to 64 bits (48 = 64-16)
                    transferAddress += (simm9Pre << 48) >> 48;

                    writeBackValue = transferAddress;
                    break;

                case POST_INDEXED:
                    writeBack = true;

                    uint64_t simm9Post = ir.repr.ls.lsConsts.sdt.offset.prePostIndex.simm9;
                    // Sign-extend simm9 from 16 to 64 bits (48 = 64-16)
                    writeBackValue = transferAddress + ((simm9Post << 48) >> 48);

                    break;

                case UNSIGNED_OFFSET:
                    transferAddress += ir.repr.ls.sf ?
                            ir.repr.ls.lsConsts.sdt.offset.uoffset * 8 :
                            ir.repr.ls.lsConsts.sdt.offset.uoffset * 4;
                    break;

            }

            break;

        case LL:
            transferAddress = getRegPC(regs);

            uint64_t simm19 = ir.repr.ls.lsConsts.simm19;
            // Sign-extend simm19 from 32 to 64 bits (32 = 64-32)
            int64_t simm19Extended = (simm19 << 32) >> 32;

            transferAddress += simm19Extended * 4;
            break;
    }

    if (isLoad) {
        BitData bitsToLoad = getReg(regs, ir.repr.ls.rt);
        writeMem(mem, ir.repr.ls.sf, transferAddress, bitsToLoad);
    } else {
        BitData bitsToStore = readMem(mem, ir.repr.ls.sf, transferAddress);
        setReg(regs, ir.repr.ls.rt, ir.repr.ls.sf, bitsToStore);
    }

    if (writeBack) {
        // Must be a SDT
        setReg(regs, ir.repr.ls.lsConsts.sdt.xn, true, writeBackValue);
    }
}
