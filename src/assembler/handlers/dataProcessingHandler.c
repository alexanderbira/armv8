///
/// dataProcessingHandler.c
/// Functions to parse from assembly a Data Processing instruction.
///
/// Created by Jack Wong on 03/06/2024.
///

#include "dataProcessingHandler.h"

static const char *aliasMnemonics[] = {
        "cmp", "cmn", "neg",
        "negs", "tst", "mvn",
        "mov", "mul", "mneg"
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

/// Converts the assembly form of an Data Processing instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @return The [IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing assembly instruction.
IR parseDataProcessing(TokenisedLine *line, AssemblerState *state) {
    assertFatal(line->operandCount >= 2 && line->operandCount <= 4,
                "[parseDataProcessing] Incorrect number of operands!");

    // If [line] is an aliased instruction, convert it first.
    bool isAlias = bsearch(line->mnemonic, *aliasMnemonics, numAliasMnemonics,
                           sizeof(char *), strcmpVoid) != NULL;
    if (isAlias) {
        // Aliased instructions always have zero register as destination.
        char *zeroRegister = (char *) malloc(3);
        strcpy(zeroRegister, (line->operands[0][0] == 'x') ? "x31" : "w31");

        // Number of operands in translated aliases is always one more.
        line->operands = realloc(line->operands, line->operandCount++ + 1);
        char *oldMnemonic = line->mnemonic;
        line->mnemonic = (char *) malloc(5);

        switch (oldMnemonic[0]) {
            case 'c':
                // cmp -> subs, cmn -> adds
                strcpy(line->mnemonic, (*(line->mnemonic + 2) == 'p') ? "subs" : "adds");
                line->operands[2] = line->operands[1];
                line->operands[1] = line->operands[0];
                line->operands[0] = zeroRegister;
                break;
            case 'n':
                // neg -> sub, negs -> subs
                strcpy(line->mnemonic, (strlen(line->mnemonic) == 3) ? "sub" : "subs");
                line->operands[2] = line->operands[1];
                line->operands[1] = zeroRegister;
                break;
            case 't':
                // tst -> ands
                strcpy(line->mnemonic, "ands");
                line->operands[2] = line->operands[1];
                line->operands[1] = line->operands[0];
                line->operands[0] = zeroRegister;
                break;
            case 'm':
                switch (line->mnemonic[1]) {
                    case 'v':
                        // mvn -> orn
                        strcpy(line->mnemonic, "orn");
                        line->operands[2] = line->operands[1];
                        line->operands[1] = zeroRegister;
                        break;
                    case 'o':
                        // mov -> orr
                        strcpy(line->mnemonic, "orr");
                        line->operands[2] = line->operands[1];
                        line->operands[1] = zeroRegister;
                        break;
                    case 'u':
                        // mul -> madd
                        strcpy(line->mnemonic, "madd");
                        line->operands[3] = zeroRegister;
                        break;
                    case 'n':
                        // mneg -> msub
                        strcpy(line->mnemonic, "msub");
                        line->operands[3] = zeroRegister;
                        break;
                }
                break;
            default:
                throwFatal("Instruction mnemonic is invalid!");
        }

        free(oldMnemonic);
    }

    // If instruction is wide-move, or arithmetic with immediate, it is an Immediate instruction.
    bool isImmediate = bsearch(line->mnemonic, *wideMoveMnemonics, numWideMoveMnemonics,
                               sizeof(char *), strcmpVoid) != NULL;
    if (!isImmediate) {
        isImmediate = bsearch(line->mnemonic, *arithmeticMnemonics, numArithmeticMnemonics,
                              sizeof(char *), strcmpVoid) != NULL;
        assertFatal(line->operandCount >= 3,
                    "[parseDataProcessing] Incorrect number of operands when calculating [isImmediate]!");
        // Immediate in instructions of [arithmeticMnemonics] always positioned in 3rd operand.
        isImmediate &= strchr(line->operands[2], '#') != NULL;
    }

    return isImmediate ? parseImmediate(line, state) : parseRegister(line, state);
}
