///
/// registerExecutor.c
/// Delegates execution of a register instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#include "registerExecutor.h"

/// Execute a register group instruction
/// @param regIR IR for a register instruction
/// @param regs Pointer to registers
void executeRegister(IR *irObject, Registers regs, unused Memory mem) {
    assertFatal(irObject->type == IMMEDIATE,
                "[executeImmediate] Received non-immediate instruction!");
    Register_IR *registerIR = &irObject->ir.registerIR;

    switch (registerIR->group) {
        case ARITHMETIC:
            arithmeticRegisterExecute(registerIR, regs);
            break;
        case BIT_LOGIC:
            bitLogicExecute(registerIR, regs);
            break;
        case MULTIPLY:
            multiplyExecute(registerIR, regs);
            break;
    }
}
