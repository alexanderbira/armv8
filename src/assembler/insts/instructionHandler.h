///
/// instructionHandler.h
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef ASSEMBLER_INSTRUCTION_HANDLER_H
#define ASSEMBLER_INSTRUCTION_HANDLER_H

#include <string.h>

#include "../../common/ir/ir.h"
#include "../handler.h"
#include "../helpers.h"

typedef struct {
    int parameterCount;
    char *mnemonic;
    char **operands;
} TokenisedLine;

typedef IR (*Handler)(const char *line, AssemblerState state);

typedef struct {
    char *mnemonic;
    Handler handler;
} HandlerEntry;

HandlerEntry instructionHandlers[] = {

};

TokenisedLine tokenise(const char *line);

Handler getParseFunction(const char *mnemonic);

#endif //ASSEMBLER_INSTRUCTION_HANDLER_H
