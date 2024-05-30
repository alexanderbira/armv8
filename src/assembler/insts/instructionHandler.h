///
/// instructionHandler.h
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef ASSEMBLER_INSTRUCTION_HANDLER_H
#define ASSEMBLER_INSTRUCTION_HANDLER_H

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "../../common/ir/ir.h"
#include "../handler.h"
#include "../helpers.h"
#include "branchHandler.h"

typedef struct {
    int parameterCount;
    char *mnemonic;
    char **operands;
} TokenisedLine;

typedef IR (*Handler)(TokenisedLine line, AssemblerState state);

typedef struct {
    char *mnemonic;
    Handler handler;
} HandlerEntry;

typedef union {
    char *label;
    int32_t immediate;
} Literal;

HandlerEntry instructionHandlers[] = {
        {"b", handleBranch}
};

TokenisedLine tokenise(const char *line);

Literal parseLiteral(const char *literal);

Handler getParseFunction(const char *mnemonic);

#endif //ASSEMBLER_INSTRUCTION_HANDLER_H
