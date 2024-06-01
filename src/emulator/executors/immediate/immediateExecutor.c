///
/// immediateExecutor.c
/// Delegates execution of an immediate instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#include "immediateExecutor.h"

/// Execute an immediate group instruction
/// @param immIR IR for an immediate instruction
/// @param regs Pointer to registers
void executeImmediate(IR *irObject, Registers regs, unused Memory mem) {
    assertFatal(irObject->type == IMMEDIATE,
                "[executeImmediate] Received non-immediate instruction!");
    Immediate_IR *immediateIR = &irObject->ir.immediateIR;

    switch (immediateIR->opi) {
        case IMMEDIATE_ARITHMETIC:
            arithmeticExecute(immediateIR, regs);
            break;
        case IMMEDIATE_WIDE_MOVE:
            wideMoveExecute(immediateIR, regs);
            break;
    }
}
