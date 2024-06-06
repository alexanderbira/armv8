///
/// bitLogicExecute.c
/// Executes a bit-logic instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#include "bitLogicExecutor.h"

static PState determinePState(bool sf, uint64_t res);

/// Executes an [IR] of a data processing (register, arithmetic) instruction.
/// @param registerIR The instruction to execute.
/// @param registers The current virtual registers.
void bitLogicExecute(Register_IR *registerIR, Registers registers) {

    // Operand interpreted as a bit-logic type instruction
    uint8_t operand = registerIR->operand.imm6;

    // Get value of register encoded by rm
    uint64_t rm = getReg(registers, registerIR->rm);
    rm = registerIR->sf ? rm : (uint32_t) rm;

    // Set rn value to the 64-bit or 32-bit value of the source register, determined by sf
    uint64_t rn = getReg(registers, registerIR->rn);
    rn = registerIR->sf ? rn : (uint32_t) rn;

    // Op2 is rm shifted by operand many bits by the encoded shift
    uint64_t op2 = bitShift(registerIR->shift, operand, rm, registerIR->sf);

    // Initialise result value
    uint64_t res;

    if (!registerIR->negated) {
        // Standard operation.
        switch (registerIR->opc.logic.standard) {
            case AND:
                res = rn & op2;
                break;

            case ORR:
                res = rn | op2;
                break; // OR (inclusive)

            case EOR:
                res = rn ^ op2;
                break; // OR (exclusive)

            case ANDS:
                // AND (and set flags)
                res = rn & op2;
                setRegStates(registers, determinePState(registerIR->sf, res));
                break;
        }
    } else {
        // Negated operation.
        switch (registerIR->opc.logic.negated) {
            case BIC:
                res = rn & ~op2;
                break; // Bit clear

            case ORN:
                res = rn | ~op2;
                break; // OR (inclusive) NOT

            case EON:
                res = rn ^ ~op2;
                break; // OR (exclusive) NOT

            case BICS:
                // Bit clear (and set flags)
                res = rn & ~op2;
                setRegStates(registers, determinePState(registerIR->sf, res));
                break;
        }
    }

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(registers, registerIR->rd, registerIR->sf, res);
}

/// Determines [PState] given a calculation result.
/// @param sf The bit-width of the register.
/// @param res The calculation result.
/// @returns The resulting [PState].
static PState determinePState(bool sf, uint64_t res) {
    PState pState;

    pState.ng = sf ? res > INT64_MAX : res > INT32_MAX;
    pState.zr = res == 0;
    pState.cr = 0;
    pState.ov = 0;

    return pState;
}
