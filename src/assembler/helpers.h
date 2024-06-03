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

/// An address code for a single data transfer instruction.
typedef struct {
    /// The destination register.
    uint8_t xn;

    /// The type of the other operand.
    enum OtherType {
        UNSIGNED, SIGNED, REG
    } type;

    /// The other operand.
    union {
        uint16_t imm;
        int16_t simm;
        uint8_t reg;
    } data;
} AddressCode;

char *trim(char *str, const char *except);

char **split(const char *str, const char *delim, int *count);

TokenisedLine tokenise(const char *line);

void destroyTokenisedLine(TokenisedLine line);

Literal parseLiteral(const char *literal);

uint8_t parseRegisterStr(const char *name, bool *sf);

#endif // ASSEMBLER_HELPERS_H
