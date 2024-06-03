//
// Created by Jack on 6/3/2024.
//

#include "dataProcessingHandler.h"

/// Converts the assembly form of an Data Processing instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @return The [IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Immediate) assembly instruction.
IR dataProcessingHandler(TokenisedLine *line, AssemblerState *state) {

}

/// Converts an assembly form instruction to its alias (if applicable).
/// @param line The [TokenisedLine] of the instruction.
/// @return The [TokenisedLine] of the input's alias (or itself, if no alias).
static TokenisedLine convertAlias(TokenisedLine *line) {
    char *mnemonic = line->mnemonic;
    char *newMnemonic;
    int operandCount = 3;
    char *zr = (line->operands[0][0] == 'x') ? "x31" : "w31";
    char **operands;

    switch (mnemonic[0]) {
        case 'c':
        {
            if (strcmp(mnemonic, "cmp") == 0) newMnemonic = "subs";
            else if (strcmp(mnemonic, "cmn") == 0) newMnemonic = "adds";
            else throwFatal("Instruction mnemonic is invalid!");
            operands = (char **[][]) {zr, line->operands[0], line->operands[1]};
            break;
        }
        case 'n':
        {
            if (strcmp(mnemonic, "neg") == 0) newMnemonic = "sub";
            else if (strcmp(mnemonic, "negs") == 0) newMnemonic = "subs";
            else throwFatal("Instruction mnemonic is invalid!");
            operands = (char **[][]) {line->operands[0], zr, line->operands[1]};
            break;
        }
        case 't':
        {
            if (strcmp(mnemonic, "tst") == 0) newMnemonic = "ands";
            else throwFatal("Instruction mnemonic is invalid!");
            operands = (char **[][]) {zr, line->operands[0], line->operands[1]};
            break;
        }
        case 'm':
        {
            if (strcmp(mnemonic, "mvn") == 0) {
                newMnemonic = "orn";
                operands = (char **[][]) {line->operands[0], zr, line->operands[1]};
            }
            else if (strcmp(mnemonic, "mov") == 0) {
                newMnemonic = "orr";
                operands = (char **[][]) {line->operands[0], zr, line->operands[1]};
            }
            else if (strcmp(mnemonic, "mul") == 0) {
                newMnemonic = "madd";
                operandCount = 4;
                operands = (char **[][]) {line->operands[0], line->operands[1], line->operands[2], zr};
            }
            else if (strcmp(mnemonic, "mneg") == 0) {
                newMnemonic = "msub";
                operandCount = 4;
                operands = (char **[][]) {line->operands[0], line->operands[1], line->operands[2], zr};
            }
            else throwFatal("Instruction mnemonic is invalid!");
            break;
        }
        case default: throwFatal("Instruction mnemonic is invalid!");
    }
    return (TokenisedLine) {operandCount, newMnemonic, .operands = operands };
}