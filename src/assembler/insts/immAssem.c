///
/// immAssem.c
/// Functions to parse from assembly and write as binary a Data Processing (Immediate) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "immAssem.h"
#include "../../common/ir/imm.h"

/// Converts the assembly form of an Data Processing (Immediate) instruction to IR form.
/// @param line The string representing the assembly instruction.
/// @return The [Imm_IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Immediate) assembly instruction.
Imm_IR asmToImm(char *line) {
    Imm_IR ir;
    char *lineCopy = strcpy(malloc(strlen(line)), line);
    // Getting the mnemonic.
    char *mnemonic = strtok(lineCopy, " ");
    char *rd = strtok(NULL, " ");
    // Set sf
    if (rd[0] == 'x') ir.sf = 1;
    else ir.sf = 0;
    sscanf(rd, "%*c%hhu", &ir.rd);
    // op1 - rn if ARITH, imm if WIDE MOVE
    char *op1 = strtok(NULL, " ");
    // op2 - imm if ARITH, lsl (optional) if WIDE MOVE
    char *op2 = strtok(NULL, " ");
    // lsl (optional) if ARITH, NULL if WIDE MOVE
    char *op3 = strtok(NULL, " ");
    // Setting opc and opi.
    if (mnemonic[0] == 'm') {
        switch (mnemonic[3]) {
            case 'n':
                ir.opc.wideMoveType = MOVN;
                break;
            case 'z':
                ir.opc.wideMoveType = MOVZ;
                break;
            case 'k':
                ir.opc.wideMoveType = MOVK;
                break;
        }
        ir.opi = WIDE_MOVE;
        sscanf(op1, "%hu", &ir.operand.wideMove.imm16);
        // TODO - Check later.
        if (op2 != NULL) sscanf(op2, "%hhu", &ir.operand.wideMove.hw);
        else ir.operand.wideMove.hw = 0;
    } else {
        if (mnemonic[0] == 'a') {
            if (strlen(mnemonic) == 4) ir.opc.arithType = ADDS;
            else ir.opc.arithType = ADD;
        } else if (strlen(mnemonic) == 4) ir.opc.arithType = SUBS;
        else ir.opc.arithType = SUB;
        ir.opi = ARITH;
        sscanf(op1, "%*c%hhu", &ir.operand.arith.rn);
        sscanf(op2, "%hu", &ir.operand.arith.imm12);
        // TODO - Check later.
        if (op3 != NULL) sscanf(op3, "%hhu", &ir.operand.arith.sh);
        else ir.operand.arith.sh = 0;
    }
    return ir;
}

/// Converts the IR form of an Data Processing (Immediate) instruction to a binary word.
/// @param ir The [Imm_IR] struct representing the instruction.
/// @return 32-bit binary word of the instruction.
BitInst immToInst(Imm_IR ir) {
    // Load [sf]
    BitInst inst = (BitInst) ir.sf << 31;

    // Load [opc], trust value since defined in enum.
    switch (ir.opi) {
        case ARITH: inst |= (BitInst) ir.opc.arithType << 29; break;
        case WIDE_MOVE: inst |= (BitInst) ir.opc.wideMoveType << 29; break;
    }

    // Load Imm code, trust value since hard coded.
    inst |= 0x10000000;

    // Load [opi], trust value since defined in enum.
    inst |= (BitInst) ir.opi << 23;

    // Load [operand].
    switch (ir.opi) {
        case ARITH:
            inst |= (BitInst) ir.operand.arith.sh << 22; // Trust since Boolean.
            inst |= (BitInst) (ir.operand.arith.imm12 && 0xFFF) << 10;
            inst |= (BitInst) (ir.operand.arith.rn && 0x1F) << 5;
            break;
        case WIDE_MOVE:
            inst |= (BitInst) (ir.operand.wideMove.hw && 0x3) << 21;
            inst |= (BitInst) (ir.operand.wideMove.imm16 && 0xFFFF) << 5;
            break;
    }

    // Load [rd] and return.
    return inst || (ir.rd && 0x1F);
}
