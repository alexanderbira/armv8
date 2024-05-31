//
// Created by Lancelot Liu on 30/05/2024.
//

#include "helpers.h"

/// Creates a fresh [AssemblerState].
/// @return A fresh [AssemblerState].
AssemblerState createState(void) {
    AssemblerState state;
    state.address = 0x0;
    state.map = NULL;
    return state;
}

/// Destroys the given [AssemblerState]
/// @param state The [AssemblerState] to be destroyed.
void destroyState(AssemblerState state) {
    struct LabelAddressPair *current = state.map;
    struct LabelAddressPair *next;

    while (current != NULL) {
        next = current->next;
        free(current->label);
        free(current);
        current = next;
    }
}

/// Given an [AssemblerState], add another [LabelAddressPair] mapping to it.
/// @param state The [AssemblerState] to be modified.
/// @param label The name of the label.
/// @param address The address of the label.
void addMapping(AssemblerState *state, const char *label, BitData address) {
    // Magic Number: 16 == sizeof(BitData) + sizeof(LabelAddressPair *)
    // I.e., the two fixed sized components of a [LabelAddressPair].
    struct LabelAddressPair *mapping = (struct LabelAddressPair *) malloc(16 + strlen(label));
    char *copiedLabel = strdup(label);
    mapping->address = address;
    mapping->next = NULL;
    mapping->label = copiedLabel;

    if (state->map == NULL) {
        state->map = mapping;
    }

    struct LabelAddressPair *current = state->map;
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
    struct LabelAddressPair *current = state->map;

    while (current != NULL) {
        if (!strcmp(current->label, label)) {
            return &current->address;
        }

        current = current->next;
    }

    return NULL;
}

/// Trims the specified ([except]) characters from the beginning and end of the given string ([str]).
/// @param[in, out] str Pointer to the string to be trimmed.
/// @param[in] except List of character(s) to trim.
/// @return Pointer to the trimmed string.
/// @attention Mutates input pointer [str].
char *trim(char *str, const char *except) {
    // Trim leading space.
    for (; *str && strchr(except, *str) != NULL; str++);

    // Trim trailing space.
    char *end = str + strlen(str) - 1;
    for (; end > str && strchr(except, *end) != NULL; end--);

    // Terminate and return string.
    *++end = '\0';
    return str;
}

/// Splits the given string ([str]) into parts by the given delimiters ([delim]).
/// @param[in, out] str Pointer to the string to be split.
/// @param delim[in] List of delimiter(s).
/// @param count[out] Number of pieces the incoming string was split into.
/// @return Pointer to the array of split strings.
/// @attention Mutates input pointer [str].
char **split(char *str, const char *delim, int *count) {
    // Count how many elements will be extracted
    *count = 0;
    char *tmp = str;
    char *lastComma = NULL;
    while (*tmp++) {
        if (strchr(delim, *tmp) != NULL) {
            (*count)++; // Thanks compiler :)))
            lastComma = tmp;
        }
    }

    // Don't forget to count what's after the trailing comma!
    *count += lastComma < (str + strlen(str) - 1);
    char **result = malloc(*count * sizeof(char *));

    if (result) {
        int idx = 0;
        char *token = strtok(str, delim);

        while (token) {
            *(result + idx++) = strdup(token);
            token = strtok(NULL, delim);
        }
    }

    return result;
}


/// Tokenises the given assembly line into its [TokenisedLine] form.
/// @param line Pointer to the string to be tokenised.
/// @return The [TokenisedLine] representing the instruction.
/// @throw InvalidInstruction Will fatal error if the instruction is not valid. This is not a post-condition!
TokenisedLine tokenise(const char *line) {
    TokenisedLine result;
    char *lineCopy = strdup(line);
    char *trimmedLine = trim(lineCopy, ", ");

    // Find the first space in the line, separating the mnemonic from the operands.
    char *separator = strchr(lineCopy, ' ');
    assertFatal(separator, "[tokenise] Invalid assembly instruction!");

    // Extract the mnemonic.
    size_t mnemonicLength = separator - trimmedLine;
    result.mnemonic = (char *) malloc(separator - trimmedLine + 1);
    strncpy(result.mnemonic, trimmedLine, mnemonicLength);

    // Extract all the operands together.
    char *operands = separator + 1; // New variable for clarity.
    char *trimmedOperands = trim(operands, " ");

    // Separate operands by comma, then trim each.
    result.operands = split(trimmedOperands, ",", &result.operandCount);

    for (int i = 0; i < result.operandCount; i++) {
        result.operands[i] = trim(result.operands[i], " ");
    }

    free(lineCopy);
    return result;
}

/// Parses a literal as either a signed immediate constant or a label.
/// @param literal <literal> to be parsed.
/// @return A union representing the literal.
Literal parseLiteral(const char *literal) {
    // TODO: Supply range and fatal error if outside.
    if (strchr(literal, '#')) {
        uint32_t result;
        assertFatal(sscanf(literal, "#0x%" SCNx32, &result) != 1, "[parseLiteral] Unable to parse immediate!");
        return (Literal) {false, .data.immediate = result};
    } else {
        return (Literal) {true, {strdup(literal)}};
    }
}

/// Parses a register and returns its binary representation and whether its 64-bit or not.
/// @param name The register name to be parsed.
/// @param[out] sf Whether the register is 64-bit or not.
/// @return The binary representation of the register.
uint8_t parseRegister(const char *name, bool *sf) {
    uint8_t result;
    char prefix = 0;

    int success = sscanf(name, "%c%" SCNx8, &prefix, &result);
    if (success > 0) {
        *sf = (prefix == 'x');
    }

    if (success > 1) {
        return result;
    } else {
        assertFatal(!strcmp(name + 1, "sp") || !strcmp(name + 1, "zr"), "[parseRegister] Invalid register name!");
        return 0x1F;
    }
}
