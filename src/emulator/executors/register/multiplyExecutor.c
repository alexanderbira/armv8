///
/// multiplyExecutor.c
/// Executes a multiply instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#include "multiplyExecutor.h"

/// Execute a multiply type instruction
/// @param regIR IR for a register (multiply) instruction
/// @param regs Pointer to registers
void multiplyExecute(Register_IR *registerIR, Registers regs) {
    // Set ra value to the 64-bit or 32-bit value of the ra-encoded register, determined by sf
    uint64_t ra = getReg(regs, registerIR->operand.multiply.ra);
    ra = registerIR->sf ? ra : (uint32_t) ra;

    // Set rn value to the 64-bit or 32-bit value of the rn-encoded register, determined by sf
    uint64_t rn = getReg(regs, registerIR->rn);
    rn = registerIR->sf ? rn : (uint32_t) rn;

    // Set rm value to the 64-bit or 32-bit value of the rm-encoded register, determined by sf
    uint64_t rm = getReg(regs, registerIR->rm);
    rm = registerIR->sf ? rm : (uint32_t) rm;

    // Initialise result value
    uint64_t res;

    // Perform multiply-add or multiply-sub depending on the value of x
    registerIR->operand.multiply.x ? (res = ra - rn * rm) : (res = ra + rn * rm);

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(regs, registerIR->rd, registerIR->sf, res);
}
