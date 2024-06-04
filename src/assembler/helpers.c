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
    char *start = str;
    for (; *start && strchr(except, *start) != NULL; start++);

    // Trim trailing space.
    char *end = str + strlen(str) - 1;
    for (; end > str && strchr(except, *end) != NULL; end--);

    // (Possibly) shift, terminate and return string.
    // Calculate the new length of the trimmed string.
    size_t newLength = end - start + 1;
    if (start != str) memmove(str, start, newLength);
    str[newLength] = '\0';
    return str;
}

/// Splits the given string [str] into parts by the given delimiters [delim].
/// @param[in] str Pointer to the string to be split.
/// @param[in] delim List of delimiter(s).
/// @param[out] count Number of pieces the incoming string was split into.
/// @return Pointer to the array of split strings.
// /// @attention Will ignore delimeters inside of brackets (incl. square, curly, round)! Cannot handle nested brackets.
// /// @example \code split("hello [world haha]", " ", &count) = ["hello", "[world haha]"] \endcode
char **split(const char *str, const char *delim, int *count) {
    *count = 0;
    char *trimmedLine = strdup(str);
    trimmedLine = trim(trimmedLine, WHITESPACE);

    // These present the start and end of the current segment.
    char *start = trimmedLine;
    char *end = trimmedLine;
    size_t length = strlen(trimmedLine);
    bool inBrackets = false;

    char **result = NULL;

    for (size_t i = 0; i < length + 1; i++) {
        // Cannot handle nested brackets!
        // if (strchr("[{()}]", trimmedLine[i]) != NULL) inBrackets = !inBrackets;

        if ((!trimmedLine[i] || strchr(delim, trimmedLine[i]) != NULL) && !inBrackets) {
            size_t operandLength = end - start;
            result = (char **) realloc(result, (*count + 1) * sizeof(char *));
            result[*count] = (char *) malloc(operandLength + 1);
            strncpy(result[*count], start, operandLength);
            *(result[*count] + operandLength) = '\0';
            start = ++end;
            (*count)++;
        } else {
            end++;
        }
    }

    assertFatal(!inBrackets, "[split] Malformed brackets!");
    free(trimmedLine);
    return result;
}

/// Tokenises the given assembly line into its [TokenisedLine] form.
/// @param line Pointer to the string to be tokenised.
/// @return The [TokenisedLine] representing the instruction.
/// @throw InvalidInstruction Will fatal error if the instruction is not valid. This is not a post-condition!
TokenisedLine tokenise(const char *line) {
    char *lineCopy = strdup(line);
    TokenisedLine result;
    result.subMnemonic = NULL;

    char *trimmedLine = trim(lineCopy, ", \n");
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

    // Copy in mnemonic. Null terminate supplied previously.
    result.mnemonic = (char *) malloc(mnemonicLength + 1);
    strncpy(result.mnemonic, trimmedLine, mnemonicLength);

    // Extract all the operands together.
    char *operands = separator + 1; // New variable for clarity.

    // Separate operands by comma, then trim each.
    result.operands = split(operands, ",", &result.operandCount);
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
//    for (int i = 0; i < line.operandCount; i++) {
//        free(line.operands[i]);
//    }
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
                    "[parseRegisterStr] Invalid register name!");
        return 0x1F;
    }
}

/// The same as [strcmp], but takes in [void *]s.
/// @param v1 The first item.
/// @param v2 The second item.
/// @return The result of \code strcmp((const char *) v1, (const char *) v2) \endcode
/// @attention Use only when you are sure your pointers are [char *]!
int strcmpVoid(const void *v1, const void *v2) {
    const char *s1 = (const char *) v1;
    const char *s2 = (const char *) v2;
    return strcmp(s1, s2);
}
