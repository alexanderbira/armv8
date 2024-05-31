///
/// registerExecute.c
/// Delegates execution of a register instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#include "registerExecute.h"

/// Execute a register group instruction
/// @param regIR IR for a register instruction
/// @param regs Pointer to registers
void executeRegister(Reg_IR regIR, Registers regs) {

    // Determine the type of register instruction using its group
    switch (regIR.group) {

        // Arithmetic instruction
        case ARITHMETIC: {
            arithmeticRegExecute(regIR, regs);
            break;
        }

        // Bit-logic instruction
        case BIT_LOGIC: {
            bitLogicExecute(regIR, regs);
            break;
        }

        // Multiply instruction
        case MULTIPLY: {
            multiplyExecute(regIR, regs);
            break;
        }

    }

}
