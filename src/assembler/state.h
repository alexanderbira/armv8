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
#include "../common/ir/ir.h"

#define INITIAL_LIST_SIZE 64

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
    } *symbolTable;

    /// The list of all parsed intermediate representations.
    IR *irList;

    /// The number of [IR]s in [irList].
    size_t irCount;

    /// The maximum number of [IR]s that [irList] is currently allocated for.
    size_t irMaxCount;
} AssemblerState;

AssemblerState createState(void);

void destroyState(AssemblerState state);

void addMapping(AssemblerState *state, const char *label, BitData address);

BitData *getMapping(AssemblerState *state, const char *label);

void addIR(AssemblerState *state, IR ir);

#endif // ASSEMBLER_STATE_H
