///
/// arithmeticRegExecute.c
/// Executes an arithmetic (register) instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#include "arithmeticRegExecute.h"

/// Determines whether signed overflow has occurred when performing a 64-bit addition
/// @param src The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the addition
/// @return Whether signed overflow has occurred
bool overflow64(int64_t src, int64_t op2, int64_t res) {
    return (src > 0 && op2 > 0 && res < 0) || (src < 0 && op2 < 0 && res > 0);
}

/// Determines whether signed overflow has occurred when performing a 32-bit addition
/// @param src The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the addition
/// @return Whether signed overflow has occurred
bool overflow32(int32_t src, int32_t op2, int32_t res) {
    return (src > 0 && op2 > 0 && res < 0) || (src < 0 && op2 < 0 && res > 0);
}

/// Determines whether signed underflow has occurred when performing a 64-bit subtraction
/// @param src The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the subtraction
/// @return Whether signed underflow has occurred
bool underflow64(int64_t src, int64_t op2, int64_t res) {
    return (src > 0 && op2 < 0 && res < 0) || (src < 0 && op2 > 0 && res > 0);
}

/// Determines whether signed underflow has occurred when performing a 32-bit subtraction
/// @param src The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the subtraction
/// @return Whether signed underflow has occurred
bool underflow32(int32_t src, int32_t op2, int32_t res) {
    return (src > 0 && op2 < 0 && res < 0) || (src < 0 && op2 > 0 && res > 0);
}

/// Execute an arithmetic type instruction
/// @param regIR IR for a register (arithmetic) instruction
/// @param regs Pointer to registers
void arithmeticRegExecute(Reg_IR regIR, Registers regs) {

    // Operand interpreted as an arithmetic type instruction
    uint8_t operand = regIR.operand.imm6;

    // Get value of register encoded by rm
    uint64_t rm = getReg(regs, regIR.rm);

    // Set src value to the 64-bit or 32-bit value of the source register, determined by sf
    uint64_t src = regIR.sf ? getReg(regs, regIR.rn) : (uint32_t) getReg(regs, regIR.rn);

    // Op2 is rm shifted by operand many bits by the encoded shift
    uint64_t op2 = bitShift(regIR.shift, operand, rm, regIR.sf);

    // Initialise result value
    uint64_t res;

    // Determine the type of arithmetic instruction
    switch (regIR.opc.arithType) {

        // Add
        case ADD: {
            res = src + op2;
            break;
        }

        // Add (and set flags)
        case ADDS: {
            res = src + op2;
            PState pState;
            pState.ng = regIR.sf ? res > INT64_MAX : res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = regIR.sf ? op2 > UINT64_MAX - src : op2 > UINT32_MAX - src;
            pState.ov = regIR.sf ? overflow64(src, op2, res) : overflow32(src, op2, res);
            setRegStates(regs, pState);
            break;
        }

        // Subtract
        case SUB: {
            res = src - op2;
            break;
        }

        // Subtract (and set flags)
        case SUBS: {
            res = src - op2;
            PState pState;
            pState.ng = regIR.sf ? res > INT64_MAX : res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = op2 > src;
            pState.ov = regIR.sf ? underflow64(src, op2, res) : underflow32(src, op2, res);
            setRegStates(regs, pState);
            break;
        }

    }

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(regs, regIR.rd, regIR.sf, res);

}
