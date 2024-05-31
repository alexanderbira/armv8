///
/// classify.h
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#include "classify.h"

/// Returns the type of content for this assembly line.
/// @param line The line to classify.
/// @return The type of content.
/// @warning Incoming line is modified - trimmed!
AsmType classify(char *line) {
    trim(line);

    // Check if line is blank.
    if (strlen(line) == 0) return BLANK;

    // Check if line is directive.
    if (line[0] == '.') return DIRECTIVE;

    // Check if line is a label.
    char *colon = strchr(line, ':');
    if (colon != NULL) {
        // Ensure all preceding chars are alphabet.
        for (char *p = line; p < colon; p++) {
            if (!isalpha((unsigned char) *p)) return INSTRUCTION;
        }
        return LABEL;
    }

    // By default, return instruction.
    return INSTRUCTION;
}
