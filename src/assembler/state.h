///
/// state.h
/// The [AssemblerState] and associated functions to model assembler state.
///
/// Created by Lancelot Liu on 31/05/2024.
///

#ifndef ASSEMBLER_STATE_H
#define ASSEMBLER_STATE_H

#include <stdlib.h>
#include <string.h>

#include "../common/const.h"

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

AssemblerState createState(void);

void destroyState(AssemblerState state);

void addMapping(AssemblerState *state, const char *label, BitData address);

BitData *getMapping(AssemblerState *state, const char *label);

#endif // ASSEMBLER_STATE_H
