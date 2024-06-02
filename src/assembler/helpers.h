///
/// helpers.h
/// Utility functions for the assembler module.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef ASSEMBLER_HELPERS_H
#define ASSEMBLER_HELPERS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "../common/const.h"
#include "../common/error.h"
#include "../common/ir/ir.h"

/// A tokenised assembly instruction.
typedef struct {
    int operandCount; /// The number of operands parsed.
    char *mnemonic;   /// The instruction mnemonic.
    char **operands;  /// The list of operands.
} TokenisedLine;

char *trim(char *str, const char *except);

char **split(char *str, const char *delim, int *count);

TokenisedLine tokenise(const char *line);

void destroyTokenisedLine(TokenisedLine line);

Literal parseLiteral(const char *literal);

uint8_t parseRegisterStr(const char *name, bool *sf);

#endif // ASSEMBLER_HELPERS_H
