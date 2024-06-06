///
/// arithmeticImmediateExecutor.c
/// Executes an arithmetic (immediate) instruction
///
/// Created by Billy Highley on 27/05/2024.
///

#include "arithmeticImmediateExecutor.h"

/// Execute an arithmetic type instruction
/// @param immIR IR for an immediate (arithmetic) instruction
/// @param regs Pointer to registers
void arithmeticExecute(Immediate_IR *immediateIR, Registers regs) {

    // Operand interpreted as an arithmetic type instruction
    struct Arithmetic *operand = &immediateIR->operand.arithmetic;

    // Set rn value to the 64-bit or 32-bit value of the source register, determined by sf
    uint64_t rn = getReg(regs, operand->rn);
    rn = immediateIR->sf ? rn : (uint32_t) rn;

    // Op2 is imm12 possibly shifted left by 12 bits, determined by sh
    uint32_t op2 = operand->imm12 << (operand->sh * 12);

    // Initialise result value
    uint64_t res;

    // Determine the type of arithmetic instruction
    switch (immediateIR->opc.arithmeticType) {

        // Add
        case ADD: {
            res = rn + op2;
            break;
        }

        // Add (and set flags)
        case ADDS: {
            res = rn + op2;
            PState pState;
            pState.ng = immediateIR->sf ? res > INT64_MAX : (uint32_t) res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = immediateIR->sf ? op2 > UINT64_MAX - rn : op2 > UINT32_MAX - rn;
            pState.ov = immediateIR->sf ? overflow64(rn, op2, res) : overflow32(rn, op2, res);
            setRegStates(regs, pState);
            break;
        }

        // Subtract
        case SUB: {
            res = rn - op2;
            break;
        }

        // Subtract (and set flags)
        case SUBS: {
            res = rn - op2;
            PState pState;
            pState.ng = immediateIR->sf ? res > INT64_MAX : (uint32_t) res > INT32_MAX;
            pState.zr = res == 0;
            pState.cr = op2 <= rn;
            pState.ov = immediateIR->sf ? underflow64(rn, op2, res) : underflow32(rn, op2, res);
            setRegStates(regs, pState);
            break;
        }

    }

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(regs, immediateIR->rd, immediateIR->sf, res);

}