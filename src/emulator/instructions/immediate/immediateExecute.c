///
/// immediateExecute.c
/// Delegates execution of an immediate instruction to its specific type
///
/// Created by Billy Highley on 27/05/2024.
///

#include "immediateExecute.h"

/// Execute an immediate group instruction
/// @param immIR IR for an immediate instruction
/// @param regs Pointer to registers
void executeImmediate(Imm_IR immIR, Registers regs) {

    // Determine the type of immediate instruction using opi
    switch (immIR.opi) {

        // Arithmetic instruction
        case ARITH: {
            arithmeticExecute(immIR, regs);
            break;
        }

        // Wide move instruction
        case WIDE_MOVE: {
            wideMoveExecute(immIR, regs);
            break;
        }

    }

}
