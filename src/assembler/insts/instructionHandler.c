///
/// instructionHandler.c
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "instructionHandler.h"

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
    if (!separator) {
        // TODO: Error since not valid instruction.
    }

    // Extract the mnemonic.
    size_t mnemonicLength = separator - trimmedLine;
    result.mnemonic = (char *) malloc(separator - trimmedLine + 1);
    strncpy(result.mnemonic, trimmedLine, mnemonicLength);

    // Extract all the operands together.
    char *operands = separator + 1; // New variable for clarity.
    char *trimmedOperands = trim(operands, " ");

    // Separate operands by comma, then trim each.
    result.operands = split(operands, ",", &result.parameterCount);

    for (int i = 0; i < result.parameterCount; i++) {
        result.operands[i] = trim(result.operands[i], " ");
    }

    free(lineCopy);
    return result;
}

/// Parses a literal as either a signed immediate constant or a label.
/// @param literal <literal> to be parsed.
/// @return A union representing the literal.
Literal parseLiteral(const char *literal) {
    Literal result;

    if (!strchr(literal, '#')) {
        sscanf(literal, "#0x%" SCNx32, &result.immediate);
    } else {
        result.label = strdup(literal);
        result.label++;
    }

    return result;
}

uint8_t parseRegister(const char *name) {
    uint8_t result;
    if (sscanf(name + 1, "*" SCNx8, &result) == 1) {
        char *number = strdup(name);
        number++;
        return sscanf(number, "x" SCNx8, &result);
    } else if (!strcmp(name + 1, "sp") || !strcmp(name + 1, "zr")) {
        return 0x1F;
    } else {
        // TODO: Throw error since invalid register name.
    }
}

Handler getParseFunction(const char *mnemonic) {
    for (int i = 0; i < sizeof(instructionHandlers) / sizeof(HandlerEntry); i++) {
        if (!strcmp(instructionHandlers[i].mnemonic, mnemonic)) {
            return instructionHandlers[i].handler;
        }
    }

    return NULL;
}
