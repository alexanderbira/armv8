///
/// immediateTranslator.c
/// Transform a [IR] of a Data Processing (Immediate) instruction to a binary instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "immediateTranslator.h"

/// Converts the IR form of a data processing (immediate) instruction to a binary word.
/// @param irObject The [IR] struct representing the instruction.
/// @param state The current state of the assembler.
/// @return 32-bit binary word of the instruction.
Instruction translateImmediate(IR *irObject, unused AssemblerState *state) {
    assertFatal(irObject->type == IMMEDIATE, "Received non-immediate IR!");
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
