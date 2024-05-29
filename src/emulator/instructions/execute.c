///
/// execute.h
/// Execute an instruction from its intermediate representation
///
/// Created by Alexander Biraben-Renard and Billy Highley on 29/05/2024.
///

#include "execute.h"

/// Execute an instruction from its intermediate representation
/// @param ir The intermediate representation of the instruction
/// @param regs A pointer to the registers
/// @param mem A pointer to memory
void execute(IR ir, Registers regs, Memory mem) {
    switch (ir.type) {
        case DPImm:
            executeImmediate(ir, regs);
            break;
        case DPReg:
            executeRegister(ir, regs);
            break;
        case LS:
            executeLoadStore(ir, regs, mem);
            break;
        case Br:
            executeBranch(ir, regs);
            break;
    }
}
