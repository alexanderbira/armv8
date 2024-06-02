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

    state.symbolTable = calloc(INITIAL_LIST_SIZE, sizeof(struct SymbolPair));
    state.symbolCount = 0;
    state.symbolMaxCount = INITIAL_LIST_SIZE;
    return state;
}

/// Destroys the given [AssemblerState]
/// @param state The [AssemblerState] to be destroyed.
void destroyState(AssemblerState state) {
    for (size_t i = 0; i < state.symbolCount; i++) {
        free(state.symbolTable[i].label);
    }

    free(state.symbolTable);
    free(state.irList);
}

/// Given an [AssemblerState], add another [LabelAddressPair] mapping to it.
/// @param state The [AssemblerState] to be modified.
/// @param label The name of the label.
/// @param address The address of the label.
void addMapping(AssemblerState *state, const char *label, BitData address) {
    // Magic Number: 16 == sizeof(BitData) + sizeof(LabelAddressPair *)
    // I.e., the two fixed sized components of a [LabelAddressPair].
    struct SymbolPair symbolPair;
    char *copiedLabel = strdup(label);
    symbolPair.address = address;
    symbolPair.label = copiedLabel;

    if (state->symbolCount >= state->symbolMaxCount) {
        // Exponential (doubling) scaling policy.
        state->symbolTable = realloc(state->symbolTable, state->symbolMaxCount * 2);
    }

    state->symbolTable[state->symbolCount++] = symbolPair;
}

/// Given a [label], searches for its address in the given [AssemblerState].
/// @param state The [AssemblerState] to be modified.
/// @param label The name of the label.
/// @returns Either a pointer to the address, or NULL if not found.
BitData *getMapping(AssemblerState *state, const char *label) {
    for (size_t i = 0; i < state->symbolCount; i++) {
        if (!strcmp(state->symbolTable[i].label, label)) {
            // Returning pointer to [uint32_t] is safe here
            // since the value is not defined in this scope.
            return &state->symbolTable[i].address;
        }
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
