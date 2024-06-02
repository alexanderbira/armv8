//
// Created by Jack on 6/1/2024.
//

#include "registerHandler.h"

/// Converts the assembly form of an Data Processing (Register) instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The [Reg_IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Register) assembly instruction.
Register_IR handleRegister(TokenisedLine line) {
//    Imm_IR ir;
    Register_IR ir;
    // Getting the mnemonic.
    char *mnemonic = line.mnemonic;
    // Assume the instruction is Arithmetic or Bit-Logic, will change M if its Multiply.
    ir.M = 0;
    // Assume instruction is Bit-Logic, change if not
    ir.group = BIT_LOGIC;
    // decides opc, group, M and negated based on first letter of mnemonic
    switch (mnemonic[0]) {
        case 'a':
        {
            // differentiate between ADD and AND type instructions by the second letter (either d or n)
            // differentiate by string length
            if (mnemonic[1] == 'd') {
                ir.opc.arithmetic = (strlen(mnemonic) == 3) ? ADD : ADDS;
                ir.group = ARITHMETIC;
            }
            // differentiate by string length
            else {
                ir.opc.logic.standard = (strlen(mnemonic) == 3) ? AND : ANDS;
                // both are not negated
                ir.negated = false;
            }
            break;
        }
        case 'b':
        {
            // differentiate between bic, bics by string length
            ir.opc.logic.negated = (strlen(mnemonic) == 3) ? BIC : BICS;
            // both are negated
            ir.negated = true;
            break;
        }
        case 'e':
        {
            // differentiate between eor, eon by 3rd letter.
            if (mnemonic[2] == 'r') {
                ir.opc.logic.standard = EOR;
                ir.negated = false;
            }
            else {
                ir.opc.logic.negated = EON;
                ir.negated = true;
            }
            break;
        }
        case 'm':
        {
            // differentiate by the 2nd letter
            ir.opc.multiply = (mnemonic[1] == 'a') ? MADD : MSUB;
            // set x depending on MADD or MSUB
            ir.operand.multiply.x = (ir.opc.multiply == MSUB);
            ir.M = 1;
            ir.group = MULTIPLY;
            break;
        }
        case 'o':
        {
            // differentiate between orr, orn by 3rd letter.
            if (mnemonic[2] == 'r') {
                ir.opc.logic.standard = ORR;
                ir.negated = false;
            }
            else {
                ir.opc.logic.negated = ORN;
                ir.negated = false;
            }
            break;
        }
        case 's':
        {
            // differentiate by string length
            ir.opc.arithmetic = (strlen(mnemonic) == 3) ? SUB : SUBS;
            ir.group = ARITHMETIC;
            break;
        }
        default: throwFatal("mnemonic is not from a Data Processing (Register) instruction!");
    }

    // decides rd, rn and rm
    sscanf(line.operands[0], "%*c%hhu", &ir.rd);
    sscanf(line.operands[1], "%*c%hhu", &ir.rn);
    sscanf(line.operands[2], "%*c%hhu", &ir.rm);
    // Set sf based on first letter (x or w) of rd
    ir.sf = (line.operands[0][0] == 'x');

    // handle operand 4 (either Ra (multiply) or shift + immediate)

    return ir;
}
