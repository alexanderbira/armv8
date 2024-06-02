//
// Created by Jack on 6/1/2024.
//

#include "registerHandler.h"

/// Converts the assembly form of an Data Processing (Register) instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The [Reg_IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Register) assembly instruction.
Register_IR handleReg(TokenisedLine line) {
//    Imm_IR ir;
    Register_IR ir;
    // Getting the mnemonic.
//    int throwaway;
    char *mnemonic = line.mnemonic;
    switch (mnemonic[0]) {
        case 'a':
        {
            // differentiate between and, ands by string length
            ir.opc.logic.standard = (strlen(mnemonic) == 3) ? AND : ANDS;
        }
        case 'b':
        {
            // differentiate between bic, bics by string length
            ir.opc.logic.negated = (strlen(mnemonic) == 3) ? BIC : BICS;
        }
        case 'e':
        {
            // differentiate between eor, eon by 3rd letter.
            if (mnemonic[2] == 'r') ir.opc.logic.standard = EOR;
            else ir.opc.logic.negated = EON;
        }
        case 'o':
        {
            // differentiate between orr, orn by 3rd letter.
            if (mnemonic[2] == 'r') ir.opc.logic.standard = ORR;
            else ir.opc.logic.negated = ORN;
        }
        case 'm':
        {
            ir.opc.multiply = (mnemonic[1] == 'a') ? MADD : MSUB;
        }

    }
    char *rd = line.operands[0];
    ir.rn
//    sscanf(rd, "%*c%hhu", &ir.rd);
//    // Set sf
//
//    ir.sf = (rd[0] == 'x');
//
//    if (mnemonic[0] == 'm') {
//        switch (mnemonic[3]) {
//            case 'n':
//                ir.opc.wideMoveType = MOVN;
//                break;
//            case 'z':
//                ir.opc.wideMoveType = MOVZ;
//                break;
//            case 'k':
//                ir.opc.wideMoveType = MOVK;
//                break;
//        }
//
//        ir.opi = WIDE_MOVE;
//        // Split remaining string into: imm and lsl value (if applicable)
//        char *imm = line.operands[1];
//        sscanf(imm, "#%hu", &ir.operand.wideMove.imm16);
//        // Check if lsl is present, and if present - get shift value.
//        if (line.operandCount > 2) {
//            char *shiftValue = split(line.operands[2], " ", &throwaway)[1];
//            sscanf(shiftValue, "#%hhu", &ir.operand.wideMove.hw);
//            // Assuming hw is value 0-3.
//            ir.operand.wideMove.hw /= 16;
//        }
//        else ir.operand.wideMove.hw = 0;
//    } else {
//        if (mnemonic[0] == 'a') {
//            if (strlen(mnemonic) == 4) ir.opc.arithType = ADDS;
//            else ir.opc.arithType = ADD;
//        } else if (strlen(mnemonic) == 4) ir.opc.arithType = SUBS;
//        else ir.opc.arithType = SUB;
//
//        ir.opi = ARITH;
//        // Split remaining string into: Rn, imm and lsl value (if applicable)
//        char *rn = line.operands[1];
//        char *imm = line.operands[2];
//        uint8_t sh = 0;
//
//        sscanf(rn, "%*c%hhu", &ir.operand.arith.rn);
//        sscanf(imm, "#%hu", &ir.operand.arith.imm12);
//        // sh is 0 or 1 depending on whether lsl value is 0 or 12.
//        if (line.operandCount > 3) {
//            char *shiftValue = split(line.operands[3], " ", &throwaway)[1];
//            sscanf(shiftValue, "#%hhu", &sh);
//            ir.operand.arith.sh = (sh == 12);
//        }
//    }
//    return ir;
}
