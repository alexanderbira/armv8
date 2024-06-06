///
/// arithmeticImmediateExecutor.c
/// Executes an arithmetic (immediate) instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#include "arithmeticImmediateExecutor.h"

static bool overflow64(int64_t rn, int64_t op2, int64_t res);
static bool overflow32(int32_t rn, int32_t op2, int32_t res);
static bool underflow64(int64_t rn, int64_t op2, int64_t res);
static bool underflow32(int32_t rn, int32_t op2, int32_t res);

/// Executes an [IR] of a data processing (immediate, arithmetic) instruction.
/// @param immediateIR The instruction to execute.
/// @param registers The current virtual registers.
void arithmeticExecute(Immediate_IR *immediateIR, Registers registers) {
    // Operand interpreted as an arithmetic type instruction
    struct Arithmetic *operand = &immediateIR->operand.arithmetic;

    // Set rn value to the 64-bit or 32-bit value of the source register, determined by sf
    uint64_t rn = getReg(registers, operand->rn);
    rn = immediateIR->sf ? rn : (uint32_t) rn;

    // [imm12] is left-shifted by 12 if [sh] is true.
    uint32_t op2 = operand->imm12 << (operand->sh * 12);

    uint64_t res;
    PState pState;

    // Determine the type of arithmetic instruction
    switch (immediateIR->opc.arithmeticType) {
        case ADD:
            res = rn + op2;
            break;

        case ADDS:
            res = rn + op2;
            pState.ng = immediateIR->sf ? res > INT64_MAX : (uint32_t) res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = immediateIR->sf ? op2 > UINT64_MAX - rn : op2 > UINT32_MAX - rn;
            pState.ov = immediateIR->sf ? overflow64(rn, op2, res) : overflow32(rn, op2, res);
            setRegStates(registers, pState);
            break;

        case SUB:
            res = rn - op2;
            break;

        case SUBS:
            res = rn - op2;
            pState.ng = immediateIR->sf ? res > INT64_MAX : (uint32_t) res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = op2 <= rn;
            pState.ov = immediateIR->sf ? underflow64(rn, op2, res) : underflow32(rn, op2, res);
            setRegStates(registers, pState);
            break;
    }

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(registers, immediateIR->rd, immediateIR->sf, res);
}

/// Determines whether signed overflow has occurred when performing a 64-bit addition
/// @param rn The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the addition
/// @returns Whether signed overflow has occurred
static bool overflow64(int64_t rn, int64_t op2, int64_t res) {
    return (rn > 0 && op2 > 0 && res < 0) || (rn < 0 && op2 < 0 && res > 0);
}

/// Determines whether signed overflow has occurred when performing a 32-bit addition
/// @param rn The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the addition
/// @returns Whether signed overflow has occurred
static bool overflow32(int32_t rn, int32_t op2, int32_t res) {
    return (rn > 0 && op2 > 0 && res < 0) || (rn < 0 && op2 < 0 && res > 0);
}

/// Determines whether signed underflow has occurred when performing a 64-bit subtraction
/// @param rn The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the subtraction
/// @returns Whether signed underflow has occurred
static bool underflow64(int64_t rn, int64_t op2, int64_t res) {
    return (rn > 0 && op2 < 0 && res < 0) || (rn < 0 && op2 > 0 && res > 0);
}

/// Determines whether signed underflow has occurred when performing a 32-bit subtraction
/// @param rn The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the subtraction
/// @returns Whether signed underflow has occurred
static bool underflow32(int32_t rn, int32_t op2, int32_t res) {
    return (rn > 0 && op2 < 0 && res < 0) || (rn < 0 && op2 > 0 && res > 0);
}
