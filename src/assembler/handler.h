///
/// handler.h
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_HANDLER_H
#define ASSEMBLER_HANDLER_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

AssemblerState createState(void);

void destroyState(AssemblerState state);

void addMapping(AssemblerState *state, const char *label, BitData address);

IR handleDirective(const char *line, AssemblerState state);

void handleLabel(const char *line, AssemblerState state);

IR handleInstruction(const char *line, AssemblerState state);

#endif //ASSEMBLER_HANDLER_H
