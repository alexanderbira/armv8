//
// Created by Lancelot Liu on 30/05/2024.
//

#ifndef ASSEMBLER_HELPERS_H
#define ASSEMBLER_HELPERS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#include "../common/const.h"
#include "../common/ir/ir.h"

/// Linked list representing a mapping from a string label to an address.
typedef struct LabelAddressPair {
    /// The address the label points to.
    BitData address;

    /// Pointer to the next mapping in the linked list.
    struct LabelAddressPair *next;

    /// The string title of the label.
    char *label;
} LabelAddressPair;

/// Struct representing the current state of the assembler.
typedef struct {
    /// The address of the current instruction being handled.
    BitData address;

    /// The mapping of string labels to addresses as a linked list.
    LabelAddressPair *map;
} AssemblerState;

typedef struct {
    int parameterCount;
    char *mnemonic;
    char **operands;
} TokenisedLine;

typedef IR (*Handler)(TokenisedLine line, AssemblerState state);

char *trim(char *str, const char *except);

char **split(char *str, const char *delim, int *count);

TokenisedLine tokenise(const char *line);

Literal parseLiteral(const char *literal);

uint8_t parseRegister(const char *name);

#endif //ASSEMBLER_HELPERS_H
