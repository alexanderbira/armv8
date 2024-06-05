///
/// dataProcessingHandler.c
/// Functions to parse from assembly a Data Processing instruction.
///
/// Created by Jack Wong on 03/06/2024.
///

#include "dataProcessingHandler.h"

static const char *aliasMnemonics[] = {
        "cmn", "cmp", "mneg",
        "mov", "mul", "mvn",
        "neg", "negs", "tst",
};

static const size_t numAliasMnemonics = sizeof(aliasMnemonics) / sizeof(char *);

static const char *wideMoveMnemonics[] = {
        "movk", "movn", "movz"
};

static const size_t numWideMoveMnemonics = sizeof(wideMoveMnemonics) / sizeof(char *);

static const char *arithmeticMnemonics[] = {
        "add", "adds", "sub", "subs"
};

static const size_t numArithmeticMnemonics = sizeof(arithmeticMnemonics) / sizeof(char *);

static void setLine(TokenisedLine *line, const char *newMnemonic, int newOperandCount, ...);

/// Converts the assembly form of an Data Processing instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @return The [IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing assembly instruction.
IR parseDataProcessing(TokenisedLine *line, AssemblerState *state) {
    assertFatal(line->operandCount >= 2 && line->operandCount <= 4,
                "[parseDataProcessing] Incorrect number of operands!");

    // If [line] is an aliased instruction, convert it first.
    bool isAlias = bsearch(&line->mnemonic, aliasMnemonics, numAliasMnemonics,
                           sizeof(char *), strcmpVoid) != NULL;
    if (isAlias) {
        // Aliased instructions always have zero register as destination.
        char *zeroRegister = strdup((line->operands[0][0] == 'x') ? "x31" : "w31");
        char *oldMnemonic = line->mnemonic;
        int oldOperandCount = line->operandCount;

        switch (oldMnemonic[0]) {
            case 'c':
                // cmp -> subs, cmn -> adds
                if (oldOperandCount == 2) {
                    setLine(line, (*(line->mnemonic + 2) == 'p') ? "subs" : "adds", 3,
                            zeroRegister, line->operands[0], line->operands[1]);
                } else {
                    setLine(line, (*(line->mnemonic + 2) == 'p') ? "subs" : "adds", 4,
                            zeroRegister, line->operands[0], line->operands[1], line->operands[2]);
                }
                break;
            case 'n':
                // neg -> sub, negs -> subs
                if (oldOperandCount == 2) {
                    setLine(line, (strlen(line->mnemonic) == 3) ? "sub" : "subs", 3,
                            line->operands[0], zeroRegister, line->operands[1]);
                } else {
                    setLine(line, (strlen(line->mnemonic) == 3) ? "sub" : "subs", 4,
                            line->operands[0], zeroRegister, line->operands[1], line->operands[2]);
                }
                break;
            case 't':
                // tst -> ands
                if (oldOperandCount == 2) {
                    setLine(line, "ands", 3,
                            zeroRegister, line->operands[0], line->operands[1]);
                } else {
                    setLine(line, "ands", 4,
                            zeroRegister, line->operands[0], line->operands[1], line->operands[2]);
                }
                break;
            case 'm':
                switch (line->mnemonic[1]) {
                    case 'v':
                        // mvn -> orn
                        if (oldOperandCount == 2) {
                            setLine(line, "orn", 3,
                                    line->operands[0], zeroRegister, line->operands[1]);
                        } else {
                            setLine(line, "orn", 4,
                                    line->operands[0], zeroRegister, line->operands[1], line->operands[2]);
                        }
                        break;
                    case 'o':
                        // mov -> orr
                        setLine(line, "orr", 3,
                                line->operands[0], zeroRegister, line->operands[1]);
                        break;
                    case 'u':
                        // mul -> madd
                        setLine(line, "madd", 4,
                                line->operands[0], line->operands[1], line->operands[2], zeroRegister);
                        break;
                    case 'n':
                        // mneg -> msub
                        setLine(line, "msub", 4,
                                line->operands[0], line->operands[1], line->operands[2], zeroRegister);
                        break;
                }
                break;
            default:
                throwFatal("Instruction mnemonic is invalid!");
        }

        // Zero register was not freed by [setLine].
        free(zeroRegister);
    }

    // If instruction is wide-move, or arithmetic with immediate, it is an Immediate instruction.
    bool isImmediate = bsearch(&line->mnemonic, wideMoveMnemonics, numWideMoveMnemonics,
                               sizeof(char *), strcmpVoid) != NULL;
    if (!isImmediate) {
        isImmediate = bsearch(&line->mnemonic, arithmeticMnemonics, numArithmeticMnemonics,
                              sizeof(char *), strcmpVoid) != NULL;
        assertFatal(line->operandCount >= 3,
                    "[parseDataProcessing] Incorrect number of operands when calculating [isImmediate]!");
        // Immediate in instructions of [arithmeticMnemonics] always positioned in 3rd operand.
        isImmediate &= (strchr(line->operands[2], '#') != NULL);
    }

    return isImmediate ? parseImmediate(line, state) : parseRegister(line, state);
}

/// Replaces the content of a [TokenisedLine], taking into account that the
/// new operands may be pointers to the old. Frees old pointers.
/// @param line The [TokenisedLine] to be altered.
/// @param newMnemonic The new mnemonic.
/// @param newOperandCount The new number of operands.
/// @param ... The new operands, [char *]s.
/// @pre len(...) == newOperandCount
/// @pre Only [...] may reference the old [TokenisedLine].
static void setLine(TokenisedLine *line, const char *newMnemonic, int newOperandCount, ...) {
    va_list args;
    va_start(args, newOperandCount);

    free(line->mnemonic);
    line->mnemonic = strdup(newMnemonic);

    // Duplicate [...] to get new operands.
    char **newOperands = malloc(newOperandCount * sizeof(char *));
    for (int i = 0; i < newOperandCount; i++) {
        char *newOperand = va_arg(args, char *);
        newOperands[i] = strdup(newOperand);
    }

    // Dispose of old operands, and replace with new.
    for (int i = 0; i < line->operandCount; i++) {
        free(line->operands[i]);
    }

    line->operands = newOperands;
    line->operandCount = newOperandCount;
}
