///
/// wideMoveExecutor.c
/// Executes a wide move instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#include "wideMoveExecutor.h"

/// Executes an [IR] of a data processing (immediate, wide move) instruction.
/// @param immediateIR The instruction to execute.
/// @param registers The current virtual registers.
void wideMoveExecute(Immediate_IR *immediateIR, Registers registers) {
    struct WideMove *operand = &immediateIR->operand.wideMove;

    // Retrieve rd value as a 64-bit or 32-bit value from the destination register, determined by sf
    uint64_t rd = getReg(registers, immediateIR->rd);
    rd = immediateIR->sf ? rd : (uint32_t) rd;

    // Op is imm16 shifted left by either 0, 16, 32 or 48 bits, determined by hw
    uint64_t op = (uint64_t) operand->imm16 << (operand->hw * 16);
    uint64_t res;

    // Determine the type of wide move instruction
    switch (immediateIR->opc.wideMoveType) {
        // Move wide with NOT
        case MOVN:
            res = ~op;
            break;

        // Move wide with zero
        case MOVZ:
            res = op;
            break;

        // Move wide with keep
        case MOVK:
            res = op | (rd & ~((uint64_t) UINT16_MAX << (operand->hw * 16)));
            break;
    }

    setReg(registers, immediateIR->rd, immediateIR->sf, res);
}
