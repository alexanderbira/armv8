//
// Created by Lancelot Liu on 30/05/2024.
//

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

/// Struct representing the current state of the assembler.
typedef struct {
    /// The address of the current instruction being handled.
    BitData address;

    /// The mapping of string labels to addresses as a linked list.
    struct LabelAddressPair {
        /// The address the label points to.
        BitData address;

        /// Pointer to the next mapping in the linked list.
        struct LabelAddressPair *next;

        /// The string title of the label.
        char *label;
    } *map;
} AssemblerState;

/// A tokenised assembly instruction.
typedef struct {
    int operandCount; /// The number of operands parsed.
    char *mnemonic;   /// The instruction mnemonic.
    char **operands;  /// The list of operands.
} TokenisedLine;

/// A function which processes a tokenised assembly instruction.
typedef IR (*Handler)(TokenisedLine line, AssemblerState *state);

AssemblerState createState(void);

void destroyState(AssemblerState state);

void addMapping(AssemblerState *state, const char *label, BitData address);

void getMapping(AssemblerState *state, const char *label, BitData *address);

char *trim(char *str, const char *except);

char **split(char *str, const char *delim, int *count);

TokenisedLine tokenise(const char *line);

Literal parseLiteral(const char *literal);

uint8_t parseRegister(const char *name);

#endif //ASSEMBLER_HELPERS_H
