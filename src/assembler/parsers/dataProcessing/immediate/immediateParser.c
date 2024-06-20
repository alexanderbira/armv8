///
/// immediateParser.c
/// Transform a [TokenisedLine] to an [IR] of a Data Processing (Immediate) instruction.
///
/// Created by Jack Wong on 29/05/2024.
///

#include "immediateParser.h"

/// Transform a [TokenisedLine] to an [IR] of a data processing (immediate) instruction.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @returns The [IR] form of the data processing (immediate) instruction.
/// @pre The [line]'s mnemonic is that of a data processing (immediate) instruction.
IR parseImmediate(TokenisedLine *line, unused AssemblerState *state) {
    assertFatal(line->operandCount >= 2 && line->operandCount <= 4,
                "Incorrect number of operands; data processing (immediate) instructions need 2, 3, or 4!");
    Immediate_IR immediateIR;

    bool sf;
    const uint8_t reg = parseRegisterStr(line->operands[0], &sf);

    if (*(line->mnemonic) == 'm') {
        enum WideMoveType type;
        switch (line->mnemonic[3]) {
            case 'n':
                type = MOVN;
                break;

            case 'z':
                type = MOVZ;
                break;

            case 'k':
                type = MOVK;
                break;

            default:
                throwFatal("Wide move suffix does not exist!");
        }

        struct WideMove wideMove;
        wideMove.imm16 = parseImmediateStr(line->operands[1], IMMEDIATE_WIDE_MOVE_IMM16_N);

        // Get shift is <lsl> is present.
        wideMove.hw = 0x0;
        if (line->operandCount == 3) {
            int matched;
            char **shiftAndValue = split(line->operands[2], " ", &matched);
            assertFatal(matched == 2, "Incorrect shift parameter!");
            assertFatal(!strcmp(shiftAndValue[0], "lsl"), "Wide move received shift other than logical left!");

            // The "hw" in assembly is actually 16 times the value in the binary instruction.
            uint8_t hwTemp = parseImmediateStr(shiftAndValue[1], IMMEDIATE_WIDE_MOVE_HW_N + 4);
            assertFatalWithArgs(hwTemp % 16 == 0, "Wide move shift <%s> is not a multiple of 16!", shiftAndValue[1]);

            // The maximum value [hw] can be is 0b11, i.e., 3. (3 * 16 = 48)
            assertFatal(hwTemp <= 48, "Wide move shift value is too high!");
            wideMove.hw = hwTemp / 16;

            free(shiftAndValue[0]);
            free(shiftAndValue[1]);
            free(shiftAndValue);
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
        *(line->mnemonic) == 'a'
        ? (type = (strlen(line->mnemonic) == 4) ? ADDS : ADD)
        : (type = (strlen(line->mnemonic) == 4) ? SUBS : SUB);

        struct Arithmetic arithmetic;

        arithmetic.rn = parseRegisterStr(line->operands[1], &sf);
        arithmetic.imm12 = parseImmediateStr(line->operands[2], IMMEDIATE_ARITHMETIC_IMM12_N);

        // Get shift is <lsl> is present.
        arithmetic.sh = false;
        if (line->operandCount == 4) {
            int matched;
            char **shiftAndValue = split(line->operands[3], " ", &matched);
            assertFatal(matched == 2, "Incorrect shift parameter!");
            assertFatal(!strcmp(shiftAndValue[0], "lsl"), "Wide move received shift other than logical left!");

            // The "hw" in assembly is actually 16 times the value in the binary instruction.
            uint8_t shiftAmount = parseImmediateStr(shiftAndValue[1], IMMEDIATE_WIDE_MOVE_HW_N + 4);
            assertFatal(shiftAmount == 0 || shiftAmount == 0xC, "Arithmetic shift is not 0x0 or 0xC!");
            arithmetic.sh = shiftAmount == 0xC;

            free(shiftAndValue[0]);
            free(shiftAndValue[1]);
            free(shiftAndValue);
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
