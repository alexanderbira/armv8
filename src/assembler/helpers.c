///
/// helpers.h
/// Utility functions for the assembler module.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "helpers.h"

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

/// Splits the given string [str] into parts by the given delimiters [delim].
/// @param[in] str Pointer to the string to be split.
/// @param[in] delim List of delimiter(s).
/// @param[out] count Number of pieces the incoming string was split into.
/// @return Pointer to the array of split strings.
/// @attention Will ignore delimeters inside of brackets (incl. square, curly, round)! Cannot handle nested brackets.
/// @example \code split("hello [world haha]", " ", &count) = ["hello", "[world haha]"] \endcode
char **split(const char *str, const char *delim, int *count) {
    *count = 0;
    char *trimmedLine = strdup(str);
    trimmedLine = trim(trimmedLine, " ");

    // These present the start and end of the current segment.
    char *start = trimmedLine;
    char *end = trimmedLine;
    size_t length = strlen(trimmedLine);
    bool inBrackets = false;

    char **result = NULL;

    for (size_t i = 0; i < length; i++) {
        // Cannot handle nested brackets!
        if (strchr("[{()}]", trimmedLine[i]) != NULL) inBrackets = !inBrackets;

        if (strchr(delim, trimmedLine[i]) != NULL && !inBrackets) {
            result = realloc(result, *count);
            result[*count] = malloc(end - start);
            strncpy(result[*count], start, end - start);
            start = ++end;
            (*count)++;
        } else {
            end++;
        }
    }

    assertFatal(!inBrackets, "[split] Malformed brackets!");

    // Put in last element if present.
    if (start != end) {
        result = realloc(result, *count);
        result[*count] = malloc(end - start);
        strncpy(result[*count], start, end - start);
        (*count)++;
    }

    free(trimmedLine);
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
    *separator = '\0';

    // Extract sub-mnemonic if present.
    char *mnemonicSeparator = strchr(trimmedLine, '.');
    if (mnemonicSeparator != NULL) {
        mnemonicLength = mnemonicSeparator - trimmedLine;

        // Throw away leading '.' when copying.
        size_t subMnemonicLength = separator - mnemonicSeparator - 1;
        result.subMnemonic = (char *) malloc(subMnemonicLength + 1);
        strncpy(result.subMnemonic, mnemonicSeparator + 1, subMnemonicLength);
        *(result.subMnemonic + subMnemonicLength) = '\0';

        assertFatal(strcmp(result.subMnemonic, ""),
                    "[tokenise] Sub-mnemonic was present but is empty!");
        // Note, we do not [assertFatal] on the mnemonic, since that signifies
        // we have found a directive!
    }

    // Copy in mnemonic.
    result.mnemonic = (char *) malloc(mnemonicLength + 1);
    strncpy(result.mnemonic, trimmedLine, mnemonicLength);
    *(result.mnemonic + mnemonicLength) = '\0';

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

/// Frees all memory associated with a [TokenisedLine].
/// @param line [TokenisedLine] to be freed.
void destroyTokenisedLine(TokenisedLine line) {
    free(line.mnemonic);
    free(line.subMnemonic);

    for (int i = 0; i < line.operandCount; i++) {
        free(line.operands[i]);
    }
    free(line.operands);
}

/// Parses a literal as either a signed immediate constant or a label.
/// @param literal <literal> to be parsed.
/// @return A union representing the literal.
Literal parseLiteral(const char *literal) {
    // TODO: Supply range and fatal error if outside.
    if (strchr(literal, '#')) {
        uint32_t result;
        assertFatal(sscanf(literal, "#0x%" SCNx32, &result) == 1,
                    "[parseLiteral] Unable to parse immediate!");
        return (Literal) {false, .data.immediate = result};
    } else {
        return (Literal) {true, .data.label = strdup(literal) };
    }
}

/// Parses a register and returns its binary representation and whether its 64-bit or not.
/// @param name The register name to be parsed.
/// @param[out] sf Whether the register is 64-bit or not.
/// @return The binary representation of the register.
/// @attention Set [bool *sf] to NULL if information is not desired.
uint8_t parseRegisterStr(const char *name, bool *sf) {
    uint8_t result;
    char prefix = 0;

    int success = sscanf(name, "%c%" SCNu8, &prefix, &result);
    if (success > 0 && sf != NULL) {
        *sf = (prefix == 'x');
    }

    if (success > 1) {
        return result;
    } else {
        assertFatal(!strcmp(name + 1, "sp") || !strcmp(name + 1, "zr"),
                    "[parseRegister] Invalid register name!");
        return 0x1F;
    }
}

/// Parses an address code for a single data transfer instruction.
/// @param operand The operand containing the address code.
/// @return An [AddressCode].
/// @pre [operand] is trimmed. This should be the case if [operand] is gotten from [tokenise].
AddressCode parseAddressCode(const char *operand) {
    AddressCode code;
    char *registerName = malloc(3);
    char *otherName = malloc(strlen(operand));

    assertFatal(*operand == '[', "[parseAddressCode] Not an address code! (Missing opening brackets.)");

    // Register names are at most three characters.
    // White space around / between comma will be trimmed away.
    // Trailing exclamation mark can be absent - sscanf supports partial success.
    // Example Operand: "[x13, #838383]!"
    assertFatal(sscanf(operand, "[%3s,%[^]!]s]!", registerName, otherName) == 2,
                "[parseAddressCode] Failed to parse code!");
    code.xn = parseRegisterStr(registerName, NULL);
    char *otherTrimmed = trim(otherName, " ");

    if (*(operand + strlen(operand) - 1) == '!') {
        code.type = SIGNED;
        assertFatal(sscanf(otherTrimmed, "#%" SCNi16, &code.data.simm) == 1,
                    "[parseAddressCode] Failed to parse signed immediate!");
    } else if (strchr(operand, '#') != NULL) {
        code.type = UNSIGNED;
        assertFatal(sscanf(otherTrimmed, "#%" SCNx16, &code.data.simm) == 1,
                    "[parseAddressCode] Failed to parse unsigned immediate!");
    } else {
        code.type = REG;
        code.data.reg = parseRegisterStr(otherTrimmed, NULL);
    }

    free(registerName);
    free(otherName);
    return code;
}
