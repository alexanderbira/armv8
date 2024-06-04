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
IR parseImmediate(TokenisedLine *line, unused AssemblerState *state) {
    assertFatal(line->operandCount >= 2 && line->operandCount <= 4,
                "[parseImmediate] Incorrect number of operands!");

    Immediate_IR immediateIR;

    bool sf;
    const uint8_t reg = parseRegisterStr(line->operands[0], &sf);

    if (*(line->mnemonic) == 'm') {
        enum WideMoveType type;
        switch (line->mnemonic[3]) {
            case 'n': type = MOVN; break;
            case 'z': type = MOVZ; break;
            case 'k': type = MOVK; break;
            default: throwFatal("[parseImmediate] Wide move suffix does not exist!");
        }

        struct WideMove wideMove;
        wideMove.imm16 = parseImmediateStr(line->operands[1], IMMEDIATE_WIDE_MOVE_IMM16_N);

        // Get shift is <lsl> is present.
        wideMove.hw = 0x0;
        if (line->operandCount == 3) {
            // Take only immediate since [mov] can only take logical left shifts.
            int matched;
            char *shiftValue = split(line->operands[2], " ", &matched)[1];
            assertFatal(matched == 2, "[parseImmediate] Incorrect shift parameter!");

            // The "hw" in assembly is actually 16 times the value in the binary instruction.
            uint8_t hwTemp = parseImmediateStr(shiftValue, 8 * sizeof(uint8_t));
            assertFatal(hwTemp % 16 == 0, "[parseImmediate] Wide move shift is not multiple of 16!");

            // The maximum value [hw] can be is 0b11, i.e., 3. (3 * 16 = 48)
            assertFatal(hwTemp <= 48, "[parseImmediate] Wide move shift value is too high!");
            wideMove.hw = hwTemp / 16;
        }

        immediateIR = (Immediate_IR) {
                .sf = sf,
                .opc.wideMoveType = type,
                .opi = IMMEDIATE_WIDE_MOVE,
                .operand.wideMove = wideMove,
                .rd = reg
        };
    } else {
        // Must be arithmetic instruction: see precondition.
        enum ArithmeticType type;
        if (*(line->mnemonic) == 'a') {
            type = (strlen(line->mnemonic) == 4) ? ADDS : ADD;
        } else {
            type = (strlen(line->mnemonic) == 4) ? SUBS : SUB;
        }

        struct Arithmetic arithmetic;

        arithmetic.rn = parseRegisterStr(line->operands[1], &sf);
        arithmetic.imm12 = parseImmediateStr(line->operands[2], IMMEDIATE_ARITHMETIC_IMM12_N);

        // Get shift is <lsl> is present.
        arithmetic.sh = false;
        if (line->operandCount == 4) {
            arithmetic.sh = true;
            // Take only immediate since arithmetic instructions can only take logical left shifts.
            int matched;
            char *shiftValue = split(line->operands[3], " ", &matched)[1];
            assertFatal(matched == 2, "[parseImmediate] Incorrect shift parameter!");
            uint8_t shiftAmount = parseImmediateStr(shiftValue, 8 * sizeof(uint8_t));
            assertFatal(shiftAmount == 0xC, "[parseImmediate] Incorrect shift amount!");
        }

        immediateIR = (Immediate_IR) {
                .sf = sf,
                .opc.arithmeticType = type,
                .opi = IMMEDIATE_ARITHMETIC,
                .operand.arithmetic = arithmetic,
                .rd = reg
        };
    }

    return (IR) { IMMEDIATE, .ir.immediateIR = immediateIR };
}

/// Converts the IR form of an Data Processing (Immediate) instruction to a binary word.
/// @param irObject The [IR] struct representing the instruction.
/// @param state The current state of the assembler.
/// @return 32-bit binary word of the instruction.
Instruction translateImmediate(IR *irObject, unused AssemblerState *state) {
    assertFatal(irObject->type == IMMEDIATE, "[translateImmediate] Received non-branch IR!");
    Immediate_IR *immediate = &irObject->ir.immediateIR;
    Instruction result = IMMEDIATE_C;

    // Load [sf], trust since boolean.
    result |= (Instruction) immediate->sf << IMMEDIATE_SF_S;

    // Load [opc], trust value since defined in enum.
    union ImmediateOpCode *opc = &immediate->opc;
    switch (immediate->opi) {
        case IMMEDIATE_ARITHMETIC:
            result |= (Instruction) opc->arithmeticType << IMMEDIATE_OPC_S;
            break;
        case IMMEDIATE_WIDE_MOVE:
            result |= (Instruction) opc->wideMoveType << IMMEDIATE_OPC_S;
            break;
    }

    // Load [opi], trust value since defined in enum.
    result |= (Instruction) immediate->opi << IMMEDIATE_OPI_S;

    // Load [operand].
    switch (immediate->opi) {
        case IMMEDIATE_ARITHMETIC : {
            struct Arithmetic *arithmetic = &immediate->operand.arithmetic;
            result |= (Instruction) arithmetic->sh << IMMEDIATE_ARITHMETIC_SH_S; // Trust since Boolean.
            result |= (Instruction) truncater(arithmetic->imm12, IMMEDIATE_ARITHMETIC_IMM12_N)
                      << IMMEDIATE_ARITHMETIC_IMM12_S;
            result |= (Instruction) truncater(arithmetic->rn, IMMEDIATE_ARITHMETIC_RN_N) << IMMEDIATE_ARITHMETIC_RN_S;
            break;
        }
        case IMMEDIATE_WIDE_MOVE : {
            struct WideMove *wideMove = &immediate->operand.wideMove;
            result |= (Instruction) truncater(wideMove->hw, IMMEDIATE_WIDE_MOVE_HW_N) << IMMEDIATE_WIDE_MOVE_HW_S;
            result |= (Instruction) truncater(wideMove->imm16, IMMEDIATE_WIDE_MOVE_IMM16_N)
                      << IMMEDIATE_WIDE_MOVE_IMM16_S;
            break;
        }
    }

    // Load [rd] and return.
    return result | truncater(immediate->rd, IMMEDIATE_RD_N);
}
