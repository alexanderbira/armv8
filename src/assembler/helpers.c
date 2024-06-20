///
/// helpers.h
/// Utility functions for the assembler module.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "helpers.h"

/// Trims the specified [except] characters from the beginning and end of the given string [str].
/// @param[in, out] str Pointer to the string to be trimmed.
/// @param except List of character(s) to trim.
/// @returns Pointer to the trimmed string.
/// @attention Mutates input pointer [str].
char *trim(char *str, const char *except) {
    // Trim leading space.
    char *start = str;
    for (; *start && strchr(except, *start) != NULL; start++);

    // Trim trailing space.
    char *end = str + strlen(str) - 1;
    for (; end > start && strchr(except, *end) != NULL; end--);

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
/// @returns Pointer to the array of split strings.
char **split(const char *str, const char *delim, int *count) {
    *count = 0;
    char *trimmedLine = strdup(str);
    assertFatalNotNull(trimmedLine, "<Memory> Unable to duplicate [char *]!");
    trimmedLine = trim(trimmedLine, WHITESPACE);

    // These present the start and end of the current segment.
    char *start = trimmedLine;
    char *end = trimmedLine;
    size_t length = strlen(trimmedLine);

    char **result = NULL;

    for (size_t i = 0; i < length + 1; i++) {
        if (!trimmedLine[i] || strchr(delim, trimmedLine[i]) != NULL) {
            size_t operandLength = end - start;
            result = (char **) realloc(result, (*count + 1) * sizeof(char *));
            assertFatalNotNull(result, "<Memory> Unable to expand by re-allocate [result]!");
            result[*count] = strndup(start, operandLength);
            assertFatalNotNull(result[*count], "<Memory> Unable to duplicate [char *]!");
            *(result[*count] + operandLength) = '\0';
            start = ++end;
            (*count)++;
        } else {
            end++;
        }
    }

    free(trimmedLine);
    return result;
}

/// Tokenises the given assembly line into its [TokenisedLine] form.
/// @param line Pointer to the string to be tokenised.
/// @returns The [TokenisedLine] representing the instruction.
/// @throw InvalidInstruction Will fatal error if the instruction is not valid. This is not a post-condition!
TokenisedLine tokenise(const char *line) {
    char *lineCopy = strdup(line);
    assertFatalNotNull(lineCopy, "<Memory> Unable to duplicate [char *]!");
    TokenisedLine result;
    result.subMnemonic = NULL;

    char *trimmedLine = trim(lineCopy, ", \n");
    // Find the first space in the line, separating the mnemonic from the operands.
    char *separator = strchr(lineCopy, ' ');
    assertFatal(separator, "Invalid assembly instruction!");

    // Extract the mnemonic.
    size_t mnemonicLength = separator - trimmedLine;
    *separator = '\0';

    // Extract sub-mnemonic if present.
    char *mnemonicSeparator = strchr(trimmedLine, '.');
    if (mnemonicSeparator != NULL) {
        mnemonicLength = mnemonicSeparator - trimmedLine;

        // Throw away leading '.' when copying.
        size_t subMnemonicLength = separator - mnemonicSeparator - 1;
        result.subMnemonic = strndup(mnemonicSeparator + 1, subMnemonicLength);
        assertFatalNotNull(result.subMnemonic, "<Memory> Unable to duplicate [char *]!");

        assertFatal(strcmp(result.subMnemonic, ""),
                    "Sub-mnemonic was present but is empty!");
        // Note, we do not [assertFatal] on the mnemonic, since that signifies
        // we have found a directive!
    }

    // Copy in mnemonic. Null terminate supplied previously.
    result.mnemonic = strndup(trimmedLine, mnemonicLength);
    assertFatalNotNull(result.mnemonic, "<Memory> Unable to duplicate [char *]!");

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
void destroyTokenisedLine(TokenisedLine *line) {
    free(line->mnemonic);
    free(line->subMnemonic);
    for (int i = 0; i < line->operandCount; i++) {
        free(line->operands[i]);
    }
    free(line->operands);
}

/// Parses a literal as either a signed immediate constant or a label.
/// @param literal <literal> to be parsed.
/// @returns A union representing the literal.
Literal parseLiteral(const char *literal) {
    if (strchr(literal, '#')) {
        uint32_t result;
        bool matched = sscanf(literal, "#0x%" SCNx32, &result) == 1;
        if (!matched) matched = sscanf(literal, "#%" SCNu32, &result) == 1;
        assertFatalWithArgs(matched,
                            "Unable to parse immediate <%s>!", literal);
        return (Literal) { .isLabel = false, .data.immediate = result };
    } else {
        char *label = strdup(literal);
        assertFatalNotNull(label, "<Memory> Unable to duplicate [char *]!");
        return (Literal) { .isLabel = true, .data.label = label };
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
    assertFatalWithArgs(success > 0, "Unable to parse register named <%s>!", name);

    if (sf != NULL) {
        *sf = (prefix == 'x');
    }

    if (success > 1) {
        assertFatalWithArgs(result < 32, "Register <%d> out of bounds!", result);
        return result;
    } else {
        assertFatalWithArgs(!strcmp(name + 1, "sp") || !strcmp(name + 1, "zr"),
                            "Invalid register named <%s>!", name);
        return 0x1F;
    }
}

/// Parses an immediate value from a string (0X... for hex, #... for dec) to a uint64_t
/// @param operand The string to parse the immediate value from.
/// @param width The maximum bit-width of the resulting value.
/// @returns The literal extracted from the string.
uint64_t parseImmediateStr(const char *operand, size_t width) {
    int64_t value;

    // Scan for hex immediate; if failure, scan for decimal.
    bool matched = sscanf(operand, "#0x%" SCNx64, &value) == 1;
    if (!matched) matched = sscanf(operand, "#%" SCNu64, &value) == 1;

    assertFatalWithArgs(matched, "Invalid immediate value <%s>!", operand);

    int64_t maxValue = width == 64 ? UINT64_MAX : (1LL << width) - 1;
    assertFatalWithArgs(value <= maxValue,
                        "Immediate value <%s> exceeds width %zu bits!", operand, width);

    return value;
}

/// Calculates the offset of a label and stores it in [Literal.data]
/// @param data The [literalData] of the [Literal] to alter
/// @param state The current state of the assembler.
void parseOffset(union LiteralData *data, AssemblerState *state) {
    // Calculate offset, then divide by 4 to encode.
    BitData *immediate = getMapping(state, data->label);
    assertFatalNotNullWithArgs(immediate, "No mapping for label named <%s>!", data->label);

    data->immediate = *immediate;
    data->immediate -= state->address;
    data->immediate /= 4;
}

/// The same as [strcmp], but takes in [void *]s.
/// @param v1 The first item.
/// @param v2 The second item.
/// @return The result of \code strcmp((const char *) v1, (const char *) v2) \endcode
/// @attention Use only when you are sure your pointers are [char *]!
int strcmpVoid(const void *v1, const void *v2) {
    const char *s1 = *(const char **) v1;
    const char *s2 = *(const char **) v2;
    return strcmp(s1, s2);
}
