///
/// bitLogicExecute.c
/// Executes a bit-logic instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#include "bitLogicExecute.h"

static PState determinePState(bool sf, uint64_t res) {
    PState pState;
    pState.ng = sf ? res > INT64_MAX : res > INT32_MAX;
    pState.zr = res == 0;
    pState.cr = 0;
    pState.ov = 0;
    return pState;
}

/// Execute a bit-logic type instruction
/// @param regIR IR for a register (bit-logic) instruction
/// @param regs Pointer to registers
void bitLogicExecute(Reg_IR regIR, Registers regs) {

    // Operand interpreted as a bit-logic type instruction
    uint8_t operand = regIR.operand.imm6;

    // Get value of register encoded by rm
    uint64_t rm = getReg(regs, regIR.rm);

    // Op2 is rm shifted by operand many bits by the encoded shift
    uint64_t op2 = bitShift(regIR.shift, operand, rm, regIR.sf);

    // Set src value to the 64-bit or 32-bit value of the source register, determined by sf
    uint64_t src = regIR.sf ? getReg(regs, regIR.rn) : (uint32_t) getReg(regs, regIR.rn);

    // Initialise result value
    uint64_t res;

    // Determines if the bitwise operation is of standard type
    if (!regIR.negated) {

        // Determine the type of standard operation
        switch (regIR.opc.logic.standard) {

            // AND
            case AND:
                res = src & op2;
                break;

            // OR (inclusive)
            case ORR:
                res = src | op2;
                break;

            // OR (exclusive)
            case EOR:
                res = src ^ op2;
                break;

            // AND (and set flags)
            case ANDS:
                res = src & op2;
                setRegStates(regs, determinePState(regIR.sf, res));
                break;

        }

    }

    // The bitwise operation is of negated type
    else {

        // Determine the type of negated operation
        switch (regIR.opc.logic.negated) {

            // Bit clear
            case BIC:
                res = src & ~op2;
                break;

            // OR (inclusive) NOT
            case ORN:
                res = src | ~op2;
                break;

            // OR (exclusive) NOT
            case EON:
                res = src ^ ~op2;
                break;

            // Bit clear (and set flags)
            case BICS:
                res = src & ~op2;
                setRegStates(regs, determinePState(regIR.sf, res));
                break;

        }

    }

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(regs, regIR.rd, regIR.sf, res);

}
