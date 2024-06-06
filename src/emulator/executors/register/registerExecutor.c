///
/// registerExecutor.c
/// Delegates execution of a register instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#include "registerExecutor.h"

/// Executes an [IR] of a data processing (register) instruction.
/// @param immediateIR The instruction to execute.
/// @param registers The current virtual registers.
/// @param memory The current virtual memory.
void executeRegister(IR *irObject, Registers registers, unused Memory memory) {
    assertFatal(irObject->type == REGISTER,
                "[executeImmediate] Received non-register instruction!");
    Register_IR *registerIR = &irObject->ir.registerIR;

    switch (registerIR->group) {
        case ARITHMETIC:
            arithmeticRegisterExecute(registerIR, registers);
            break;

        case BIT_LOGIC:
            bitLogicExecute(registerIR, registers);
            break;

        case MULTIPLY:
            multiplyExecute(registerIR, registers);
            break;
    }
}
