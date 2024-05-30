///
/// handler.c
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#include "handler.h"

/// Creates a fresh [AssemblerState].
/// @return A fresh [AssemblerState].
AssemblerState createState(void) {
    AssemblerState state;
    state.address = 0x0;
}

/// Destroys the given [AssemblerState]
/// @param state The [AssemblerState] to be destroyed.
void destroyState(AssemblerState state) {
    LabelAddressPair *current = state.map;
    LabelAddressPair *next;

    while (current != NULL) {
        next = current->next;
        free(current->label);
        free(current);
        current = next;
    }
}

/// Given an [AssemblerState], add another [LabelAddressPair] mapping to it.
/// @param state The [AssemblerState] to be modified.
/// @param mapping The [LabelAddressPair] to be added.
void addMapping(AssemblerState *state, const char *label, BitData address) {
    // Magic Number: 16 == sizeof(BitData) + sizeof(LabelAddressPair *)
    // I.e., the two fixed sized components of a [LabelAddressPair].
    LabelAddressPair *mapping = (LabelAddressPair *) malloc(16 + strlen(label));
    char *copiedLabel = strdup(label);
    mapping->address = address;
    mapping->next = NULL;
    mapping->label = copiedLabel;

    if (state->map == NULL) {
        state->map = mapping;
    }

    LabelAddressPair *current = state->map;
    LabelAddressPair *prev;

    while (current != NULL) {
        prev = current;
        current = current->next;
    }

    prev->next = mapping;
}

/// Function to process a directive, i.e. a line that begins with '.'.
/// @param line The line to "process".
/// @return The resulting binary word.
IR handleDirective(const char *line, AssemblerState state) {
    // TODO: Implement
    return 0;
}

/// Function to process a label, i.e. a line of alphabet characters ending in ':'.
/// @param line The line to "process".
/// @return The resulting binary word.
/// @pre The incoming [line] is a label,  i.e. a line of alphabet characters ending in ':', and is trimmed.
void handleLabel(const char *line, AssemblerState state) {
    // Remove trailing colon. See precondition.
    char *label = strdup(line);
    *(label + strlen(label) - 1) = '\0';

    addMapping(&state, label, state.address);
}

/// Function to process an instruction.
/// @param line The line to "process".
/// @return The resulting binary word.
IR handleInstruction(const char *line, AssemblerState state) {
    // TODO: Implement
    return 0;
}
