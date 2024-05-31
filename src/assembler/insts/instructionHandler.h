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
    char *mnemonic;
    Handler handler;
} HandlerEntry;

Handler getParseFunction(const char *mnemonic);

#endif // ASSEMBLER_INSTRUCTION_HANDLER_H
