///
/// dataProcessingHandler.c
/// Functions to parse from assembly a Data Processing instruction.
///
/// Created by Jack Wong on 03/06/2024.
///

#include "dataProcessingHandler.h"

static TokenisedLine convertAlias(TokenisedLine *line);
static bool isImmediateInstruction(TokenisedLine *line);

/// Converts the assembly form of an Data Processing instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @return The [IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing assembly instruction.
IR parseDataProcessing(TokenisedLine *line, AssemblerState *state) {
    IR ir;
    char *mnemonic = line->mnemonic;

    bool isAlias = false;
    char *aliases[9] = {"cmp", "cmn", "neg", "negs", "tst", "mvn", "mov", "mul", "mneg"};
    for (int i = 0; i < 9; i++) {
        if (strcmp(mnemonic, aliases[i]) == 0) {
            isAlias = true;
            break;
        }
    }

    TokenisedLine converted = convertAlias(line);
    TokenisedLine *workingLine = isAlias ? &converted : line;

    if (isImmediateInstruction(workingLine)) {
        ir = parseImmediate(workingLine, state);
    } else {
        // assuming it can't be any other instruction type
        ir = parseRegister(workingLine, state);
    }

    if (isAlias) {
        // only destroy if a new TokenisedLine was made (since the one passed in to parseDataProcessing is handled elsewhere)
        destroyTokenisedLine(*workingLine);
    }

    return ir;
}

/// Converts a [TokenisedLine] of an alias instruction to a normal instruction.
/// @param line The [TokenisedLine] to be converted.
/// @return The new [TokenisedLine].
/// @pre Incoming [line->mnemonic] is a alias mnemonic.
/// @attention Remember to destroy the original [TokenisedLine]!
static TokenisedLine convertAlias(TokenisedLine *line) {
    // Overallocating by 1 seems reasonable for [char *newMnemonic] and [char **operands].
    // Max number of chars / pointers is 4 for both.
    // Using #define for this seems... hard to name :)
    char *newMnemonic = (char *) malloc(4);
    char **operands = (char **) malloc(4 * sizeof(char *));
    int operandCount = 3; // operandCount is either 3 or 4. Default to 3.

    // Aliased instructions always have zero register as destination.
    char *zeroRegister = (char *) malloc(3);
    strcpy(zeroRegister, (line->operands[0][0] == 'x') ? "x31" : "w31");

    switch (line->mnemonic[0]) {

        case 'c':
            // cmp -> subs, cmn -> adds
            strcpy(newMnemonic, (*(line->mnemonic + 2) == 'p') ? "subs" : "adds");
            operands[0] = zeroRegister;
            operands[1] = strdup(line->operands[0]);
            operands[2] = strdup(line->operands[1]);
            break;

        case 'n':
            // neg -> sub, negs -> subs
            strcpy(newMnemonic, (strlen(line->mnemonic) == 3) ? "sub" : "subs");
            operands[0] = strdup(line->operands[0]);
            operands[1] = zeroRegister;
            operands[2] = strdup(line->operands[1]);
            break;

        case 't':
            // tst -> ands
            strcpy(newMnemonic, "ands");
            operands[0] = zeroRegister;
            operands[1] = strdup(line->operands[0]);
            operands[2] = strdup(line->operands[1]);
            break;

        case 'm':
            switch (line->mnemonic[1]) {

                case 'v':
                    // mvn -> orn
                    strcpy(newMnemonic, "orn");
                    operands[0] = strdup(line->operands[0]);
                    operands[1] = zeroRegister;
                    operands[2] = strdup(line->operands[1]);
                    break;

                case 'o':
                    // mov -> orr
                    strcpy(newMnemonic, "orr");
                    operands[0] = strdup(line->operands[0]);
                    operands[1] = zeroRegister;
                    operands[2] = strdup(line->operands[1]);
                    break;

                case 'u':
                    // mul -> madd
                    strcpy(newMnemonic, "madd");
                    operandCount = 4;
                    operands[0] = strdup(line->operands[0]);
                    operands[1] = strdup(line->operands[1]);
                    operands[2] = strdup(line->operands[2]);
                    operands[3] = zeroRegister;
                    break;

                case 'n':
                    // mneg -> msub
                    strcpy(newMnemonic, "msub");
                    operandCount = 4;
                    operands[0] = strdup(line->operands[0]);
                    operands[1] = strdup(line->operands[1]);
                    operands[2] = strdup(line->operands[2]);
                    operands[3] = zeroRegister;
                    break;
            }
            break;
        default:
            throwFatal("Instruction mnemonic is invalid!");
    }

    return (TokenisedLine) {operandCount, newMnemonic, NULL, operands};
}

/// Returns whether a given instruction is a Data Processing (Immediate) instruction.
/// @param line The [TokenisedLine] of the instruction.
/// @return true if instruction is Data Processing (Immediate), false if not.
static bool isImmediateInstruction(TokenisedLine *line) {
    char *mnemonic = line->mnemonic;

    bool isWideMove = (
            strcmp(mnemonic, "movk") == 0 ||
            strcmp(mnemonic, "movn") == 0 ||
            strcmp(mnemonic, "movz") == 0
    );
    if (isWideMove) return true;

    bool isArithmetic = (
        strcmp(mnemonic, "add") == 0 ||
        strcmp(mnemonic, "adds") == 0 ||
        strcmp(mnemonic, "sub") == 0 ||
        strcmp(mnemonic, "subs") == 0
        );

    // Only return true if it's an arithmetic operation with an immediate operand
    return isArithmetic && line->operands[2][0] == '#';
}