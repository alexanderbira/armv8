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
    uint64_t ra = registerIR->sf ?
                  getReg(regs, registerIR->operand.multiply.ra) :
                  (uint32_t) getReg(regs, registerIR->operand.multiply.ra);

    // Set rn value to the 64-bit or 32-bit value of the rn-encoded register, determined by sf
    uint64_t rn = registerIR->sf ?
                  getReg(regs, registerIR->rn) :
                  (uint32_t) getReg(regs, registerIR->rn);

    // Set rm value to the 64-bit or 32-bit value of the rm-encoded register, determined by sf
    uint64_t rm = registerIR->sf ?
                  getReg(regs, registerIR->rm) :
                  (uint32_t) getReg(regs, registerIR->rm);

    // Initialise result value
    uint64_t res;

    // Perform multiply-add or multiply-sub depending on the value of x
    registerIR->operand.multiply.x ? (res = ra + rn * rm) : (res = ra - rn * rm);

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(regs, registerIR->rd, registerIR->sf, res);
}
