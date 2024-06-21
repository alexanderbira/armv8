///
/// state.c
/// The [AssemblerState] and associated functions to model assembler state.
///
/// Created by Lancelot Liu on 31/05/2024.
///

#include "state.h"

/// Creates a fresh [AssemblerState].
/// @returns A fresh [AssemblerState].
AssemblerState createState(void) {
    AssemblerState state;
    state.address = 0x0;
    state.symbolTable = NULL;

    state.irList = calloc(INITIAL_LIST_SIZE, sizeof(IR));
    assertFatalNotNull(state.irList, "<Memory> Unable to contiguously allocate [irList]!");
    state.irCount = 0;
    state.irMaxCount = INITIAL_LIST_SIZE;

    state.symbolTable = calloc(INITIAL_LIST_SIZE, sizeof(struct SymbolPair));
    assertFatalNotNull(state.symbolTable, "<Memory> Unable to contiguously allocate [symbolTable]!");
    state.symbolCount = 0;
    state.symbolMaxCount = INITIAL_LIST_SIZE;
    return state;
}

/// If applicable, frees the string label in a [Literal].
/// @param literal The literal to be freed.
static void freeLiteral(Literal *literal) {
    // By our convention, this simply means that the [Literal]
    // was initialised as a label.
    if (literal->isLabel) free(literal->data.label);
}

/// Frees pointers held in an IR
/// @param ir IR within which to free pointers
static void freeIR(IR *irObject) {
    switch (irObject->type) {
        case BRANCH: {
            Branch_IR *branch = &irObject->ir.branchIR;
            switch (branch->type) {
                case BRANCH_UNCONDITIONAL:
                    freeLiteral(&branch->data.simm26);
                    break;

                case BRANCH_CONDITIONAL:
                    freeLiteral(&branch->data.conditional.simm19);
                    break;

                default:
                    break;
            }
            break;
        }

        case LOAD_STORE:
            if (irObject->ir.loadStoreIR.type == LOAD_LITERAL) {
                if (irObject->ir.loadStoreIR.data.simm19.isLabel) {
                    free(irObject->ir.loadStoreIR.data.simm19.data.label);
                }
            }

        default:
            break;
    }
}

/// Destroys the given [AssemblerState]
/// @param state The [AssemblerState] to be destroyed.
void destroyState(AssemblerState state) {
    for (size_t i = 0; i < state.symbolCount; i++) {
        free(state.symbolTable[i].label);
    }

    free(state.symbolTable);

    for (size_t i = 0; i < state.irCount; i++) {
        freeIR(&state.irList[i]);
    }

    free(state.irList);
}

/// Given an [AssemblerState], add another [LabelAddressPair] mapping to it.
/// @param state The [AssemblerState] to be modified.
/// @param label The name of the label.
/// @param address The address of the label.
void addMapping(AssemblerState *state, char *label, BitData address) {
    struct SymbolPair symbolPair;
    symbolPair.address = address;
    symbolPair.label = label;

    if (state->symbolCount >= state->symbolMaxCount) {
        // Exponential (doubling) scaling policy.
        state->symbolTable = realloc(state->symbolTable, state->symbolMaxCount * 2);
        assertFatalNotNull(state->symbolTable, "<Memory> Unable to expand by re-allocate [symbolTable]!");
    }

    state->symbolTable[state->symbolCount++] = symbolPair;
}

/// Given a [label], searches for its address in the given [AssemblerState].
/// @param state The [AssemblerState] to be modified.
/// @param label The name of the label.
/// @returns Either a pointer to the address, or NULL if not found.
/// @attention We return a pointer simply to be able to express NULL as failure.
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

/// Adds an [IR] to the given [AssemblerState]
/// @param state The [AssemblerState] to modify.
/// @param ir The [IR] to add.
void addIR(AssemblerState *state, IR ir) {
    if (state->irCount >= state->irMaxCount) {
        // Exponential (doubling) scaling policy.
        state->irList = realloc(state->irList, state->irMaxCount * 2);
        assertFatalNotNull(state->irList, "<Memory> Unable to expand by re-allocate [irList]!");
    }

    state->irList[state->irCount++] = ir;
}
