///
/// state.c
/// The [AssemblerState] and associated functions to model assembler state.
///
/// Created by Lancelot Liu on 31/05/2024.
///

#include "state.h"

/// Creates a fresh [AssemblerState].
/// @return A fresh [AssemblerState].
AssemblerState createState(void) {
    AssemblerState state;
    state.address = 0x0;
    state.symbolTable = NULL;

    state.irList = calloc(INITIAL_LIST_SIZE, sizeof(IR));
    state.irCount = 0;
    state.irMaxCount = INITIAL_LIST_SIZE;
    return state;
}

/// Destroys the given [AssemblerState]
/// @param state The [AssemblerState] to be destroyed.
void destroyState(AssemblerState state) {
    struct LabelAddressPair *currentSymbol = state.symbolTable;
    struct LabelAddressPair *nextSymbol;

    while (currentSymbol != NULL) {
        nextSymbol = currentSymbol->next;
        free(currentSymbol->label);
        free(currentSymbol);
        currentSymbol = nextSymbol;
    }

    free(state.irList);
}

/// Given an [AssemblerState], add another [LabelAddressPair] mapping to it.
/// @param state The [AssemblerState] to be modified.
/// @param label The name of the label.
/// @param address The address of the label.
void addMapping(AssemblerState *state, const char *label, BitData address) {
    // Magic Number: 16 == sizeof(BitData) + sizeof(LabelAddressPair *)
    // I.e., the two fixed sized components of a [LabelAddressPair].
    struct LabelAddressPair *mapping = (struct LabelAddressPair *) malloc(16 + strlen(label) + 1);
    char *copiedLabel = strdup(label);
    mapping->address = address;
    mapping->next = NULL;
    mapping->label = copiedLabel;

    if (state->symbolTable == NULL) {
        state->symbolTable = mapping;
    }

    struct LabelAddressPair *current = state->symbolTable;
    struct LabelAddressPair *previous;

    while (current != NULL) {
        previous = current;
        current = current->next;
    }

    previous->next = mapping;
}

/// Given a [label], searches for its address in the given [AssemblerState].
/// @param state The [AssemblerState] to be modified.
/// @param label The name of the label.
/// @returns Either a pointer to the address, or NULL if not found.
BitData *getMapping(AssemblerState *state, const char *label) {
    struct LabelAddressPair *current = state->symbolTable;

    while (current != NULL) {
        if (!strcmp(current->label, label)) {
            return &current->address;
        }

        current = current->next;
    }

    return NULL;
}

void addIR(AssemblerState *state, IR ir) {
    if (state->irCount >= state->irMaxCount) {
        // Exponential (doubling) scaling policy.
        state->irList = realloc(state->irList, state->irMaxCount * 2);
    }

    state->irList[state->irCount++] = ir;
}
