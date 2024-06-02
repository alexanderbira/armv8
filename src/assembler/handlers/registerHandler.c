//
// Created by Jack on 6/1/2024.
//

#include "registerHandler.h"

/// Converts the assembly form of an Data Processing (Register) instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The [Reg_IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Register) assembly instruction.
IR handleRegister(TokenisedLine line) {
    Register_IR registerIR;
    // Getting the mnemonic.
    char *mnemonic = line.mnemonic;
    bool sf;
    union RegisterOpCode opc;
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
    // decides opc, group, M and negated based on first letter of mnemonic
    switch (mnemonic[0]) {
        case 'a':
        {
            // differentiate between ADD and AND type instructions by the second letter (either d or n)
            // differentiate by string length
            if (mnemonic[1] == 'd') {
                opc.arithmetic = (strlen(mnemonic) == 3) ? ADD : ADDS;
                group = ARITHMETIC;
            }
            // differentiate by string length
            else {
                opc.logic.standard = (strlen(mnemonic) == 3) ? AND : ANDS;
                // both are not negated
                negated = false;
                opr = 0;
            }
            break;
        }
        case 'b':
        {
            // differentiate between bic, bics by string length
            opc.logic.negated = (strlen(mnemonic) == 3) ? BIC : BICS;
            // both are negated
            negated = true;
            opr = 0;
            break;
        }
        case 'e':
        {
            // differentiate between eor, eon by 3rd letter.
            if (mnemonic[2] == 'r') {
                opc.logic.standard = EOR;
                negated = false;
            }
            else {
                opc.logic.negated = EON;
                negated = true;
            }
            opr = 0;
            break;
        }
        case 'm':
        {
            // differentiate by the 2nd letter
            opc.multiply = (mnemonic[1] == 'a') ? MADD : MSUB;
            M = 1;
            group = MULTIPLY;
            break;
        }
        case 'o':
        {
            // differentiate between orr, orn by 3rd letter.
            if (mnemonic[2] == 'r') {
                opc.logic.standard = ORR;
                negated = false;
            }
            else {
                opc.logic.negated = ORN;
                negated = false;
            }
            opr = 0;
            break;
        }
        case 's':
        {
            // differentiate by string length
            opc.arithmetic = (strlen(mnemonic) == 3) ? SUB : SUBS;
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

    // handle operand 4 (either Ra (multiply) or shift + immediate)
    union RegisterOperand operand;
    enum ShiftType shift;
    if (group == MULTIPLY) {
        struct Multiply multiply;
        uint8_t ra;
        // set x depending on MADD or MSUB
        bool x = (registerIR.opc.multiply == MSUB);
        sscanf(line.operands[3], "%*c%hhu", &ra);

        // shift is set as LSL since LSL = 0
        shift = LSL;
        multiply = (struct Multiply) {x, ra};
        operand = (union RegisterOperand) {.multiply = multiply};
    } else {
        uint8_t imm6;
        int matched;
        char **shiftAndValue = split(line.operands[3], " ", &matched);

        // fill imm6
        sscanf(shiftAndValue[2], "%*c%hhu", &imm6);

        // switch based on first letter of shift name
        switch (shiftAndValue[0][0]) {
            case 'l':
            {
                // differentiate based on third letter of shift name
                shift = (shiftAndValue[0][2] == 'l') ? LSL : LSR;
                break;
            }
            case 'a':
            {
                shift = ASR;
                break;
            }
            case 'r':
            {
                shift = ROR;
                break;
            }
            default: throwFatal("Shift supplied was not a shift.");
        }

        operand = (union RegisterOperand) {.imm6 = imm6};
    }

    registerIR = (Register_IR) {sf, opc, M, opr, group, shift, negated, rm, operand, rn ,rd};
    return (IR) { REGISTER, .ir.registerIR = registerIR };
}
