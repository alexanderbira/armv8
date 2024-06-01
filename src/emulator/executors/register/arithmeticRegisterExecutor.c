///
/// arithmeticRegExecute.c
/// Executes an arithmetic (register) instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#include "arithmeticRegisterExecutor.h"

static bool overflow64(int64_t src, int64_t op2, int64_t res);

static bool overflow32(int32_t src, int32_t op2, int32_t res);

static bool underflow64(int64_t src, int64_t op2, int64_t res);

static bool underflow32(int32_t src, int32_t op2, int32_t res);

/// Execute an arithmetic type instruction
/// @param regIR IR for a register (arithmetic) instruction
/// @param regs Pointer to registers
void arithmeticRegisterExecute(Register_IR *registerIR, Registers regs) {

    // Operand interpreted as an arithmetic type instruction
    uint8_t operand = registerIR->operand.imm6;

    // Get value of register encoded by rm
    uint64_t rm = getReg(regs, registerIR->rm);

    // Set src value to the 64-bit or 32-bit value of the source register, determined by sf
    uint64_t src = registerIR->sf ?
                   getReg(regs, registerIR->rn) :
                   (uint32_t) getReg(regs, registerIR->rn);

    // Op2 is rm shifted by operand many bits by the encoded shift
    uint64_t op2 = registerIR->sf ?
                   bitShift(registerIR->shift, operand, rm, registerIR->sf) :
                   (uint32_t) bitShift(registerIR->shift, operand, rm, registerIR->sf);

    // Initialise result value
    uint64_t res;

    PState pState;
    switch (registerIR->opc.arithmetic) {
        case ADD:
            res = src + op2;
            break;
        case ADDS:
            // Add (and set flags)
            res = src + op2;
            pState.ng = registerIR->sf ? res > INT64_MAX : (uint32_t) res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = registerIR->sf ? op2 > UINT64_MAX - src : op2 > UINT32_MAX - src;
            pState.ov = registerIR->sf ? overflow64(src, op2, res) : overflow32(src, op2, res);
            setRegStates(regs, pState);
            break;
        case SUB:
            res = src - op2;
            break;
        case SUBS:
            // Subtract (and set flags)
            res = src - op2;
            pState.ng = registerIR->sf ? res > INT64_MAX : (uint32_t) res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = op2 <= src;
            pState.ov = registerIR->sf ? underflow64(src, op2, res) : underflow32(src, op2, res);
            setRegStates(regs, pState);
            break;
    }

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(regs, registerIR->rd, registerIR->sf, res);
}

/// Determines whether signed overflow has occurred when performing a 64-bit addition
/// @param src The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the addition
/// @return Whether signed overflow has occurred
static bool overflow64(int64_t src, int64_t op2, int64_t res) {
    return (src > 0 && op2 > 0 && res < 0) || (src < 0 && op2 < 0 && res > 0);
}

/// Determines whether signed overflow has occurred when performing a 32-bit addition
/// @param src The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the addition
/// @return Whether signed overflow has occurred
static bool overflow32(int32_t src, int32_t op2, int32_t res) {
    return (src > 0 && op2 > 0 && res < 0) || (src < 0 && op2 < 0 && res > 0);
}

/// Determines whether signed underflow has occurred when performing a 64-bit subtraction
/// @param src The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the subtraction
/// @return Whether signed underflow has occurred
static bool underflow64(int64_t src, int64_t op2, int64_t res) {
    return (src > 0 && op2 < 0 && res < 0) || (src < 0 && op2 > 0 && res > 0);
}

/// Determines whether signed underflow has occurred when performing a 32-bit subtraction
/// @param src The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the subtraction
/// @return Whether signed underflow has occurred
static bool underflow32(int32_t src, int32_t op2, int32_t res) {
    return (src > 0 && op2 < 0 && res < 0) || (src < 0 && op2 > 0 && res > 0);
}
