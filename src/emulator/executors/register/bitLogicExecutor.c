///
/// bitLogicExecute.c
/// Executes a bit-logic instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#include "bitLogicExecutor.h"

static PState determinePState(bool sf, uint64_t res);

/// Execute a bit-logic type instruction
/// @param regIR IR for a register (bit-logic) instruction
/// @param regs Pointer to registers
void bitLogicExecute(Register_IR *registerIR, Registers regs) {

    // Operand interpreted as a bit-logic type instruction
    uint8_t operand = registerIR->operand.imm6;

    // Get value of register encoded by rm
    uint64_t rm = getReg(regs, registerIR->rm);
    rm = registerIR->sf ? rm : (uint32_t) rm;

    // Set rn value to the 64-bit or 32-bit value of the source register, determined by sf
    uint64_t rn = getReg(regs, registerIR->rn);
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
                setRegStates(regs, determinePState(registerIR->sf, res));
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
                setRegStates(regs, determinePState(registerIR->sf, res));
                break;

        }

    }

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(regs, registerIR->rd, registerIR->sf, res);

}

static PState determinePState(bool sf, uint64_t res) {

    PState pState;

    pState.ng = sf ? res > INT64_MAX : res > INT32_MAX;
    pState.zr = res == 0;
    pState.cr = 0;
    pState.ov = 0;

    return pState;

}
