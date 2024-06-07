///
/// immediateExecutor.c
/// Delegates execution of an immediate instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#include "immediateExecutor.h"

/// Executes an [IR] of a data processing (immediate) instruction.
/// @param immediateIR The instruction to execute.
/// @param registers The current virtual registers.
/// @param memory The current virtual memory.
void executeImmediate(IR *irObject, Registers registers, unused Memory memory) {
    assertFatal(irObject->type == IMMEDIATE,
                "Received non-immediate instruction!");

    Immediate_IR *immediateIR = &irObject->ir.immediateIR;

    immediateIR->opi == IMMEDIATE_ARITHMETIC
    ? arithmeticExecute(immediateIR, registers)
    : wideMoveExecute(immediateIR, registers);
}
