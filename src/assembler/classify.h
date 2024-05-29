///
/// classify.h
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_CLASSIFY_H
#define ASSEMBLER_CLASSIFY_H

#include "../common/helpers.h"

typedef enum {
    /// Represents a blank line.
    BLANK,

    /// Represents a directive, i.e. a line that begins with '.'.
    DIRECTIVE,

    /// Represents a label, i.e. a line of alphabet characters ending in ':'.
    LABEL,

    /// Represents an instruction.
    INSTRUCTION,
} AsmType;

AsmType classify(char *line);

#endif //ASSEMBLER_CLASSIFY_H
