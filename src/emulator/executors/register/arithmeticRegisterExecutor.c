///
/// arithmeticRegExecute.c
/// Executes an arithmetic (register) instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#include "arithmeticRegisterExecutor.h"

/// Execute an arithmetic type instruction
/// @param regIR IR for a register (arithmetic) instruction
/// @param regs Pointer to registers
void arithmeticRegisterExecute(Register_IR *registerIR, Registers regs) {

    // Operand interpreted as an arithmetic type instruction
    uint8_t operand = registerIR->operand.imm6;

    // Set rn value to the 64-bit or 32-bit value of the operand register, determined by sf
    uint64_t rm = getReg(regs, registerIR->rm);
    rm = registerIR->sf ? rm : (uint32_t) rm;

    // Set rn value to the 64-bit or 32-bit value of the source register, determined by sf
    uint64_t rn = getReg(regs, registerIR->rn);
    rn = registerIR->sf ? rn : (uint32_t) rn;

    // Op2 is rm shifted by operand many bits by the encoded shift
    uint64_t op2 = bitShift(registerIR->shift, operand, rm, registerIR->sf);

    // Initialise result value
    uint64_t res;

    PState pState;

    switch (registerIR->opc.arithmetic) {

        // Add
        case ADD:
            res = rn + op2;
            break;

        // Add (and set flags)
        case ADDS:
            res = rn + op2;
            pState.ng = registerIR->sf ? res > INT64_MAX : (uint32_t) res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = registerIR->sf ? op2 > UINT64_MAX - rn : op2 > UINT32_MAX - rn;
            pState.ov = registerIR->sf ? overflow64(rn, op2, res) : overflow32(rn, op2, res);
            setRegStates(regs, pState);
            break;

        // Subtract
        case SUB:
            res = rn - op2;
            break;

        // Subtract (and set flags)
        case SUBS:
            res = rn - op2;
            pState.ng = registerIR->sf ? res > INT64_MAX : (uint32_t) res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = op2 <= rn;
            pState.ov = registerIR->sf ? underflow64(rn, op2, res) : underflow32(rn, op2, res);
            setRegStates(regs, pState);
            break;

    }

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(regs, registerIR->rd, registerIR->sf, res);

}
