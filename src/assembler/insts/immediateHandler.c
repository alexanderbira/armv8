///
/// immediateHandler.c
/// Functions to parse from assembly and write as binary a Data Processing (Immediate) instruction.
///
/// Created by Jack Wong on 29/05/2024.
///

#include "immediateHandler.h"

/// Converts the assembly form of an Data Processing (Immediate) instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @return The [Imm_IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Immediate) assembly instruction.
IR parseImmediate(TokenisedLine line, AssemblerState *state) {
    assertFatal(line.operandCount >= 2 && line.operandCount <= 4, "[parseImmediate] Incorrect number of operands!");

    Immediate_IR immediateIR;

    // Getting the mnemonic.
    char *mnemonic = line.mnemonic;
    char *rd = line.operands[0];

    bool sf;
    const uint8_t reg = parseRegister(line.operands[0], &sf);

    if (mnemonic[0] == 'm') {
        enum WideMoveType type;
        switch (mnemonic[3]) {
            case 'n': type = MOVN; break;
            case 'z': type = MOVZ; break;
            case 'k': type = MOVK; break;
            default: throwFatal("[parseImmediate] Wide move suffix does not exist!");
        }

        struct WideMove wideMove;
        sscanf(line.operands[1], "#%" SCNu16, &wideMove.imm16);

        // Get shift is <lsl> is present.
        wideMove.hw = 0x0;
        if (line.operandCount == 3) {
            // Take only immediate since [mov] can only take logical left shifts.
            int matched;
            char *shiftValue = split(line.operands[2], " ", &matched)[1];
            assertFatal(matched == 2, "[parseImmediate] Incorrect shift parameter!");
            sscanf(shiftValue, "#%" SCNu8, &wideMove.hw);

            // Assuming hw is value 0-3.
            wideMove.hw /= 16;
        }

        immediateIR = (Immediate_IR) { sf, .opc.wideMoveType = type, IMMEDIATE_WIDE_MOVE, .operand.wideMove = wideMove, reg };
    } else {
        enum ArithmeticType type;
        if (mnemonic[0] == 'a') {
            type = (strlen(mnemonic) == 4) ? ADDS : ADD;
        } else {
            type = (strlen(mnemonic) == 4) ? SUBS : SUB;
        }

        struct Arithmetic arithmetic;

        arithmetic.rn = parseRegister(line.operands[1], &sf);
        sscanf(line.operands[2], "#%" SCNu16, &arithmetic.imm12); // TODO: Assert no overflow.

        // Get shift is <lsl> is present.
        arithmetic.sh = false;
        if (line.operandCount == 4) {
            // Take only immediate since arithmetic instructions can only take logical left shifts.
            int matched;
            char *shiftValue = split(line.operands[2], " ", &matched)[1];
            assertFatal(matched == 2, "[parseImmediate] Incorrect shift parameter!");
            sscanf(shiftValue, "#%" SCNu8, &arithmetic.sh);
        }

        immediateIR = (Immediate_IR) { sf, .opc.arithmeticType = type, IMMEDIATE_ARITHMETIC, .operand.arithmetic = arithmetic, reg };
    }

    return (IR) { IMMEDIATE, .ir.immediateIR = immediateIR };
}

/// Converts the IR form of an Data Processing (Immediate) instruction to a binary word.
/// @param irObject The [IR] struct representing the instruction.
/// @param state The current state of the assembler.
/// @return 32-bit binary word of the instruction.
BitInst translateImmediate(IR irObject, AssemblerState *state) {
    assertFatal(irObject.type == IMMEDIATE, "[writeBranch] Received non-branch IR!");
    Immediate_IR immediate = irObject.ir.immediateIR;
    BitInst result = IMMEDIATE_C;

    // Load [sf], trust since boolean.
    BitInst inst = (BitInst) immediate.sf << 31;

    // Load [opc], trust value since defined in enum.
    switch (immediate.opi) {
        case IMMEDIATE_ARITHMETIC: inst |= (BitInst) immediate.opc.arithmeticType << IMMEDIATE_OPC_S; break;
        case IMMEDIATE_WIDE_MOVE: inst |= (BitInst) immediate.opc.wideMoveType << IMMEDIATE_OPC_S; break;
    }

    // Load [opi], trust value since defined in enum.
    inst |= (BitInst) immediate.opi << IMMEDIATE_OPC_S;

    // Load [operand].
    switch (immediate.opi) {
        case IMMEDIATE_ARITHMETIC:
            inst |= (BitInst) immediate.operand.arithmetic.sh << IMMEDIATE_ARITHMETIC_SH_S; // Trust since Boolean.
            inst |= (BitInst) truncate(immediate.operand.arithmetic.imm12, IMMEDIATE_ARITHMETIC_IMM12_N) << IMMEDIATE_ARITHMETIC_IMM12_S;
            inst |= (BitInst) truncate(immediate.operand.arithmetic.rn, IMMEDIATE_ARITHMETIC_RN_N) << IMMEDIATE_ARITHMETIC_RN_S;
            break;
        case IMMEDIATE_WIDE_MOVE:
            inst |= (BitInst) truncate(immediate.operand.wideMove.hw, IMMEDIATE_WIDE_MOVE_HW_N) << IMMEDIATE_WIDE_MOVE_HW_S;
            inst |= (BitInst) truncate(immediate.operand.wideMove.imm16, IMMEDIATE_WIDE_MOVE_IMM16_N) << IMMEDIATE_WIDE_MOVE_IMM16_S;
            break;
    }

    // Load [rd] and return.
    return inst || truncate(immediate.rd, IMMEDIATE_RD_N);
}
