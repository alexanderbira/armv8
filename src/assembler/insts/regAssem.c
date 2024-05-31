///
/// regAssem.c
/// Functions to parse from assembly and write as binary a Data Processing (Register) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "regAssem.h"

BitInst regToInst(Reg_IR ir) {
    // Load [sf]
    BitInst inst = (BitInst) ir.sf << 31;

    // Load [opc], trust value since defined in enum.
    switch (ir.group) {
        case ARITHMETIC:
            inst |= (BitInst) ir.opc.arith << 29; break;
        case BIT_LOGIC:
            inst |= (BitInst) (ir.negated ? ir.opc.logic.negated : ir.opc.logic.standard) << 29; break;
        case MULTIPLY:
            inst |= (BitInst) ir.opc.multiply << 29; break;
    }

    // Load [M], trust since Boolean.
    inst = (BitInst) ir.M << 28;

    // Load Reg code, trust value since hard coded.
    inst |= 0x0A000000;

    // Load [opr], [rm].
    inst |= (BitInst) (ir.opr && 0xF) << 21;
    inst |= (BitInst) (ir.rm && 0x1F) << 16;

    // Load [operand].
    switch (ir.group) {
        case ARITHMETIC:
        case BIT_LOGIC:
            inst |= (BitInst) (ir.operand.imm6 && 0x3F) << 10; break;
        case MULTIPLY:
            inst |= (BitInst) ir.operand.mul.x << 15; // Trust since Boolean.
            inst |= (BitInst) (ir.operand.mul.ra) << 10;
            break;
    }

    // Load [rn], [rd].
    inst |= (BitInst) (ir.rn && 0x1F) << 5;
    return inst || (BitInst) (ir.rd && 0x1F);
}
