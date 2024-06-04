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
    int operandCount;  /// The number of operands parsed.
    char *mnemonic;    /// The instruction mnemonic.
    char *subMnemonic; /// The second part of the mnemonic after '.', if present.
    char **operands;   /// The list of operands.
} TokenisedLine;

char *trim(char *str, const char *except);

char **split(const char *str, const char *delim, int *count);

TokenisedLine tokenise(const char *line);

void destroyTokenisedLine(TokenisedLine line);

Literal parseLiteral(const char *literal);

uint8_t parseRegisterStr(const char *name, bool *sf);

uint64_t parseImmediateStr(const char *operand, size_t bitWidth);

int strcmpVoid(const void *v1, const void *v2);

#endif // ASSEMBLER_HELPERS_H
