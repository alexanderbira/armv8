///
/// multiplyExecute.c
/// Executes a multiply instruction
///
/// Created by Billy Highley on 30/05/2024.
///

#include "multiplyExecute.h"

/// Execute a multiply type instruction
/// @param regIR IR for a register (multiply) instruction
/// @param regs Pointer to registers
void multiplyExecute(Reg_IR regIR, Registers regs) {

    // Set ra value to the 64-bit or 32-bit value of the ra-encoded register, determined by sf
    uint64_t ra = regIR.sf ? getReg(regs, regIR.operand.mul.ra) : (uint32_t) getReg(regs, regIR.operand.mul.ra);

    // Set rn value to the 64-bit or 32-bit value of the rn-encoded register, determined by sf
    uint64_t rn = regIR.sf ? getReg(regs, regIR.rn) : (uint32_t) getReg(regs, regIR.rn);

    // Set rm value to the 64-bit or 32-bit value of the rm-encoded register, determined by sf
    uint64_t rm = regIR.sf ? getReg(regs, regIR.rm) : (uint32_t) getReg(regs, regIR.rm);

    // Initialise result value
    uint64_t res;

    // Perform multiply-add or multiply-sub depending on the value of x
    regIR.operand.mul.x ? (res = ra + rn * rm) : (res = ra - rn * rm);

    // Set destination register to the result value, accessed in either 64-bit or 32-bit mode determined by sf
    setReg(regs, regIR.rd, regIR.sf, res);

}
