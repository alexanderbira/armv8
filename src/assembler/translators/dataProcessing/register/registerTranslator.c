///
/// loadStoreTranslator.c
/// Transform a [IR] of a Load-Store instruction to a binary instruction.
///
/// Created by Jack Wong on 01/06/2024.
///

#include "registerTranslator.h"

/// Converts the IR form of a data processing (register) instruction to a binary word.
/// @param irObject The [IR] struct representing the instruction.
/// @param state The current state of the assembler.
/// @returns 32-bit binary word of the instruction.
Instruction translateRegister(IR *irObject, unused AssemblerState *state) {
    assertFatal(irObject->type == REGISTER, "Received non-register IR!");
    Register_IR *registerIR = &irObject->ir.registerIR;
    Instruction instruction = REGISTER_;

    // Load [sf]
    instruction |= (Instruction) registerIR->sf << IMMEDIATE_SF_S;

    // Load [opc], trust value since defined in enum.
    switch (registerIR->group) {
        case ARITHMETIC:
            instruction |= (Instruction) truncater(registerIR->opc.arithmetic, REGISTER_OPC_N) << REGISTER_OPC_S;
            break;

        case BIT_LOGIC:
            instruction |= (Instruction)
                    truncater(registerIR->negated
                              ? registerIR->opc.logic.negated
                              : registerIR->opc.logic.standard, REGISTER_OPC_N)
                    << REGISTER_OPC_S;
            break;

        case MULTIPLY: break;
    }

    // Load [M], trust since Boolean.
    instruction |= (Instruction) registerIR->M << REGISTER_M_S;

    // Load [opr], [rm].
    instruction |= (Instruction) truncater(registerIR->opr, REGISTER_OPR_N) << REGISTER_OPR_S;
    instruction |= (Instruction) truncater(registerIR->rm, REGISTER_RM_N) << REGISTER_RM_S;

    // Load [operand].
    switch (registerIR->group) {
        case ARITHMETIC:
        case BIT_LOGIC:
            instruction |= (Instruction) truncater(registerIR->operand.imm6, REGISTER_OPERAND_IMM6_N)
                    << REGISTER_OPERAND_IMM6_S;
            break;
        case MULTIPLY:
            instruction |= (Instruction) registerIR->operand.multiply.x << REGISTER_OPERAND_X_S; // Trust since Boolean.
            instruction |= (Instruction) truncater(registerIR->operand.multiply.ra, REGISTER_OPERAND_RA_N)
                    << REGISTER_OPERAND_RA_S;
            break;
    }

    // Load [rn], [rd].
    instruction |= (Instruction) truncater(registerIR->rn, REGISTER_RN_N) << REGISTER_RN_S;
    return instruction | (Instruction) truncater(registerIR->rd, REGISTER_RD_N);
}
