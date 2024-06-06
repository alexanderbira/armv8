///
/// registerParser.c
/// Transform a [TokenisedLine] to an [IR] of a Data Processing (Register) instruction.
///
/// Created by Jack Wong on 01/06/2024.
///

#include "registerParser.h"

/// Converts the assembly form of an Data Processing (Register) instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @return The [Reg_IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Register) assembly instruction.
IR parseRegister(TokenisedLine *line, unused AssemblerState *state) {
    Register_IR registerIR;

    union RegisterOpCode opc;
    bool M = 0; // At first, assume [M] as if instruction is arithmetic / bit-logic.
    enum RegisterType group; // At first, assume [group] as if instruction is bit-logic.
    uint8_t opr;
    bool negated;

    registerIR.opc.multiply = false;

    switch (line->mnemonic[0]) {

        case 'a':
            // differentiate by string length
            if (line->mnemonic[1] == 'd') {
                opc.arithmetic = (strlen(line->mnemonic) == 3) ? ADD : ADDS;
                group = ARITHMETIC;
                opr = REGISTER_ARITHMETIC_C;
            }
                // differentiate by string length
            else {
                opc.logic.standard = (strlen(line->mnemonic) == 3) ? AND : ANDS;
                // both are not negated
                negated = false;
                group = BIT_LOGIC;
                opr = REGISTER_BITLOGIC_C;
            }
            break;

        case 'b':
            // differentiate between bic, bics by string length
            opc.logic.negated = (strlen(line->mnemonic) == 3) ? BIC : BICS;
            // both are negated
            negated = true;
            group = BIT_LOGIC;
            opr = REGISTER_BITLOGIC_C;
            break;

        case 'e':
            // differentiate between eor, eon by 3rd letter.
            if (line->mnemonic[2] == 'r') {
                opc.logic.standard = EOR;
                negated = false;
            } else {
                opc.logic.negated = EON;
                negated = true;
            }
            group = BIT_LOGIC;
            opr = REGISTER_BITLOGIC_C;
            break;

        case 'm':
            // differentiate by the 2nd letter
            opc.multiply = (line->mnemonic[1] == 'a') ? MADD : MSUB;
            M = 1;
            negated = opc.multiply == MSUB;
            group = MULTIPLY;
            opr = REGISTER_MULTIPLY_C;
            break;

        case 'o':
            // differentiate between orr, orn by 3rd letter.
            if (line->mnemonic[2] == 'r') {
                opc.logic.standard = ORR;
                negated = false;
            } else {
                opc.logic.negated = ORN;
                negated = true;
            }
            group = BIT_LOGIC;
            opr = REGISTER_BITLOGIC_C;
            break;

        case 's':
            // differentiate by string length
            opc.arithmetic = (strlen(line->mnemonic) == 3) ? SUB : SUBS;
            group = ARITHMETIC;
            opr = REGISTER_ARITHMETIC_C;
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
        uint8_t ra = parseRegisterStr(line->operands[3], NULL);
        bool x = (opc.multiply == MSUB);

        // shift is set as LSL since LSL = 0
        shift = LSL;
        struct Multiply multiply = (struct Multiply) {x, ra};

        operand = (union RegisterOperand) {.multiply = multiply};
    } else {
        uint8_t imm6 = 0;
        shift = 0;

        int matched;

        if (line->operandCount == 4 && strchr(line->operands[3], ' ') != NULL) {
            char **shiftAndValue = split(line->operands[3], " ", &matched);
            // fill imm6
            imm6 = parseImmediateStr(shiftAndValue[1]);
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
                default:
                    throwFatal("Shift supplied was not a shift.");
            }
        }


        operand = (union RegisterOperand) {.imm6 = imm6};

        opr |= shift << 1;
        opr |= negated;
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
