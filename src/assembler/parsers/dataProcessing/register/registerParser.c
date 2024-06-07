///
/// registerParser.c
/// Transform a [TokenisedLine] to an [IR] of a Data Processing (Register) instruction.
///
/// Created by Jack Wong on 01/06/2024.
///

#include "registerParser.h"

/// Transform a [TokenisedLine] to an [IR] of a data processing (register) instruction.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @returns The [IR] form of the data processing (register) instruction.
/// @pre The [line]'s mnemonic is that of a data processing (register) instruction.
IR parseRegister(TokenisedLine *line, unused AssemblerState *state) {
    Register_IR registerIR;

    // Populate all fields except [opc], [M], [opr].
    // Where [M] and derivations of [opr] is given assumptions.
    registerIR.M = false;        // At first, assume [M] == false.
    registerIR.negated = false;  // At first, assume [N] == false.
    registerIR.shift = LSL;      // At first, assume [shift] == LSL since it's ordinal value is 0x0.
    registerIR.operand.imm6 = 0; // At first, assume the shift amount is 0x0.

    // Parse all registers, and ensure their bit-width is identical.
    bool sfRD, sfRN, sfRM;
    registerIR.rd = parseRegisterStr(line->operands[0], &sfRD);
    registerIR.rn = parseRegisterStr(line->operands[1], &sfRN);
    registerIR.rm = parseRegisterStr(line->operands[2], &sfRM);
    assertFatal(sfRD == sfRN && sfRN == sfRM, "Register bit-widths not identical!");
    registerIR.sf = sfRD;

    switch (*line->mnemonic) {
        case 'a':
            if (line->mnemonic[1] == 'd') {
                // add
                registerIR.group = ARITHMETIC;
                registerIR.opc.arithmetic = (strlen(line->mnemonic) == 3) ? ADD : ADDS;
                registerIR.opr = REGISTER_ARITHMETIC_C;
            } else {
                // and
                registerIR.group = BIT_LOGIC;
                registerIR.opc.logic.standard = (strlen(line->mnemonic) == 3) ? AND : ANDS;
                registerIR.opr = REGISTER_BIT_LOGIC_C;
            }
            break;

        case 'b':
            // bic, bics
            registerIR.group = BIT_LOGIC;
            registerIR.opc.logic.negated = (strlen(line->mnemonic) == 3) ? BIC : BICS;
            registerIR.opr = REGISTER_BIT_LOGIC_C;
            registerIR.negated = true;
            break;

        case 'e':
            // eor, eon
            registerIR.group = BIT_LOGIC;
            registerIR.opr = REGISTER_BIT_LOGIC_C;
            if (line->mnemonic[2] == 'r') {
                registerIR.opc.logic.standard = EOR;
            } else {
                registerIR.opc.logic.negated = EON;
                registerIR.negated = true;
            }
            break;

        case 'm':
            // madd, msub
            registerIR.group = MULTIPLY;
            registerIR.opc.multiply = (line->mnemonic[1] == 'a') ? MADD : MSUB;
            registerIR.M = true;
            registerIR.opr = REGISTER_MULTIPLY_C;

            uint8_t ra = parseRegisterStr(line->operands[3], NULL);
            bool x = (registerIR.opc.multiply == MSUB);
            registerIR.operand = (union RegisterOperand) { .multiply = (struct Multiply) { x, ra }};
            break;

        case 'o':
            // orr, orn
            registerIR.group = BIT_LOGIC;
            registerIR.opr = REGISTER_BIT_LOGIC_C;
            if (line->mnemonic[2] == 'r') {
                registerIR.opc.logic.standard = ORR;
            } else {
                registerIR.opc.logic.negated = ORN;
                registerIR.negated = true;
            }
            break;

        case 's':
            // sub, subs
            registerIR.group = ARITHMETIC;
            registerIR.opr = REGISTER_ARITHMETIC_C;
            registerIR.opc.arithmetic = (strlen(line->mnemonic) == 3) ? SUB : SUBS;
            break;

        default:
            throwFatal("Invalid mnemonic!");
    }

    // Deal with shift, if present.
    // We know that if the last argument exists, it must be a shift.
    if (line->operandCount == 4 && registerIR.group != MULTIPLY) {
        int numMatched;
        char **shiftAndValue = split(line->operands[3], " ", &numMatched);
        assertFatal(numMatched == 2, "Incomplete shift parameter!");

        enum ShiftType shift;
        switch (shiftAndValue[0][0]) {
            case 'l':
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

        uint8_t imm6 = parseImmediateStr(shiftAndValue[1]);
        registerIR.operand.imm6 = imm6;
        registerIR.shift = shift;
    }

    registerIR.opr |= registerIR.shift << 1;
    registerIR.opr |= registerIR.negated;

    return (IR) { .type = REGISTER, .ir.registerIR = registerIR };
}
