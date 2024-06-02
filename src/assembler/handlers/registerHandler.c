//
// Created by Jack on 6/1/2024.
//

#include "registerHandler.h"

/// Converts the assembly form of an Data Processing (Register) instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The [Reg_IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Register) assembly instruction.
IR parseRegister(TokenisedLine *line, AssemblerState *state) {
    Register_IR registerIR;

    union RegisterOpCode opc;
    bool M = 0; // At first, assume [M] as if instruction is arithmetic / bit-logic.
    enum RegisterType group = BIT_LOGIC; // At first, assume [group] as if instruction is bit-logic.
    uint8_t opr = REGISTER_BITLOGIC_GM; // At first, assume [opr] as if instruction is bit-logic.
    bool negated;

    switch (line->mnemonic[0]) {

        case 'a':
            // differentiate by string length
            if (line->mnemonic[1] == 'd') {
                opc.arithmetic = (strlen(line->mnemonic) == 3) ? ADD : ADDS;
                group = ARITHMETIC;
            }
            // differentiate by string length
            else {
                opc.logic.standard = (strlen(line->mnemonic) == 3) ? AND : ANDS;
                // both are not negated
                negated = false;
                opr = 0;
            }
            break;

        case 'b':
            // differentiate between bic, bics by string length
            opc.logic.negated = (strlen(line->mnemonic) == 3) ? BIC : BICS;
            // both are negated
            negated = true;
            opr = 0;
            break;

        case 'e':
            // differentiate between eor, eon by 3rd letter.
            if (line->mnemonic[2] == 'r') {
                opc.logic.standard = EOR;
                negated = false;
            }
            else {
                opc.logic.negated = EON;
                negated = true;
            }
            opr = 0;
            break;

        case 'm':
            // differentiate by the 2nd letter
            opc.multiply = (line->mnemonic[1] == 'a') ? MADD : MSUB;
            M = 1;
            group = MULTIPLY;
            break;

        case 'o':
            // differentiate between orr, orn by 3rd letter.
            if (line->mnemonic[2] == 'r') {
                opc.logic.standard = ORR;
                negated = false;
            }
            else {
                opc.logic.negated = ORN;
                negated = false;
            }
            opr = 0;
            break;

        case 's':
            // differentiate by string length
            opc.arithmetic = (strlen(line->mnemonic) == 3) ? SUB : SUBS;
            group = ARITHMETIC;
            break;

        default:
            throwFatal("[parseRegister] Invalid mnemonic!");
    }

    bool sf;
    uint8_t rd = parseRegisterStr(line->operands[0], &sf);
    uint8_t rn = parseRegisterStr(line->operands[1], &sf);
    uint8_t rm = parseRegisterStr(line->operands[2], &sf);

    // handle operand 4 (either Ra (multiply) or shift + immediate)
    union RegisterOperand operand;
    enum ShiftType shift;
    if (group == MULTIPLY) {
        struct Multiply multiply;
        uint8_t ra;

        // set x depending on MADD or MSUB
        bool x = (registerIR.opc.multiply == MSUB);
        sscanf(line->operands[3], "%*c%hhu", &ra);

        // shift is set as LSL since LSL = 0
        shift = LSL;
        multiply = (struct Multiply) {x, ra};
        operand = (union RegisterOperand) {.multiply = multiply};
    } else {
        uint8_t imm6;
        int matched;
        char **shiftAndValue = split(line->operands[3], " ", &matched);

        // fill imm6
        sscanf(shiftAndValue[2], "%*c%hhu", &imm6);

        // switch based on first letter of shift name
        switch (shiftAndValue[0][0]) {
            case 'l':
                // differentiate based on third letter of shift name
                shift = (shiftAndValue[0][2] == 'l') ? LSL : LSR;
                break;
            case 'a':
                shift = ASR;
                break;
            case 'r':
                shift = ROR;
                break;
            default: throwFatal("Shift supplied was not a shift.");
        }

        operand = (union RegisterOperand) {.imm6 = imm6};
    }

    registerIR = (Register_IR) {
            .sf = sf,
            .opc = opc,
            .M = M,
            .opr = opr,
            .group = group,
            .shift = shift,
            .negated = negated,
            .rm = rm,
            .operand = operand,
            .rn = rn,
            .rd = rd
    };

    return (IR) {.type = REGISTER, .ir.registerIR = registerIR};
}

Instruction translateRegister(IR *irObject, unused AssemblerState *state) {
    assertFatal(irObject->type == REGISTER, "[translateRegister] Received non-branch IR!");
    Register_IR *registerIR = &irObject->ir.registerIR;
    Instruction instruction = REGISTER_;

    // Load [sf]
    instruction = (Instruction) registerIR->sf << 31;

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

        case MULTIPLY:
            instruction |= (Instruction) truncater(registerIR->opc.multiply, REGISTER_OPC_N) << REGISTER_OPC_S;
            break;

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
