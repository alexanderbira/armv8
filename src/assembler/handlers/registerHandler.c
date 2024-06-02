//
// Created by Jack on 6/1/2024.
//

#include "registerHandler.h"

/// Converts the assembly form of an Data Processing (Register) instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The [Reg_IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Register) assembly instruction.
IR handleRegister(TokenisedLine line) {
//    Imm_IR ir;
    Register_IR registerIR;
    // Getting the mnemonic.
    char *mnemonic = line.mnemonic;
    // Assume the instruction is Arithmetic or Bit-Logic, will change M if its Multiply.
    bool M = 0;
    // Assume instruction is Bit-Logic, change if not
    enum RegisterType group = BIT_LOGIC;
    // Assume instruction is Airthmetic or Multiply, change if not (all x in spec will be put as 0)
    uint8_t opr = 8; // bit 1000
    bool negated;
    uint8_t rd;
    uint8_t rn;
    uint8_t rm;
    bool sf;
    // decides opc, group, M and negated based on first letter of mnemonic
    switch (mnemonic[0]) {
        case 'a':
        {
            // differentiate between ADD and AND type instructions by the second letter (either d or n)
            // differentiate by string length
            if (mnemonic[1] == 'd') {
                registerIR.opc.arithmetic = (strlen(mnemonic) == 3) ? ADD : ADDS;
                group = ARITHMETIC;
            }
            // differentiate by string length
            else {
                registerIR.opc.logic.standard = (strlen(mnemonic) == 3) ? AND : ANDS;
                // both are not negated
                negated = false;
                opr = 0;
            }
            break;
        }
        case 'b':
        {
            // differentiate between bic, bics by string length
            registerIR.opc.logic.negated = (strlen(mnemonic) == 3) ? BIC : BICS;
            // both are negated
            negated = true;
            opr = 0;
            break;
        }
        case 'e':
        {
            // differentiate between eor, eon by 3rd letter.
            if (mnemonic[2] == 'r') {
                registerIR.opc.logic.standard = EOR;
                negated = false;
            }
            else {
                registerIR.opc.logic.negated = EON;
                negated = true;
            }
            opr = 0;
            break;
        }
        case 'm':
        {
            // differentiate by the 2nd letter
            registerIR.opc.multiply = (mnemonic[1] == 'a') ? MADD : MSUB;
            M = 1;
            group = MULTIPLY;
            break;
        }
        case 'o':
        {
            // differentiate between orr, orn by 3rd letter.
            if (mnemonic[2] == 'r') {
                registerIR.opc.logic.standard = ORR;
                negated = false;
            }
            else {
                registerIR.opc.logic.negated = ORN;
                negated = false;
            }
            opr = 0;
            break;
        }
        case 's':
        {
            // differentiate by string length
            registerIR.opc.arithmetic = (strlen(mnemonic) == 3) ? SUB : SUBS;
            group = ARITHMETIC;
            break;
        }
        default: throwFatal("mnemonic is not from a Data Processing (Register) instruction!");
    }

    // decides rd, rn and rm in that order.
    sscanf(line.operands[0], "%*c%hhu", &rd);
    sscanf(line.operands[1], "%*c%hhu", &rn);
    sscanf(line.operands[2], "%*c%hhu", &rm);
    // Set sf based on first letter (x or w) of rd
    sf = (line.operands[0][0] == 'x');

    // set x depending on MADD or MSUB
    registerIR.operand.multiply.x = (registerIR.opc.multiply == MSUB);
    // handle operand 4 (either Ra (multiply) or shift + immediate)
    if (group == MULTIPLY) sscanf(line.operands[3], "%*c%hhu", &registerIR.operand.multiply.ra);

    return (IR) { REGISTER, .ir.registerIR = registerIR };;
}
