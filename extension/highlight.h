///
/// highlight.h
/// Print a line of code with syntax-highlighting.
///
/// Created by Alexander Biraben-Renard on 15/06/2024.
///

#ifndef HIGHLIGHT_H
#define HIGHLIGHT_H

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define COLOR_TRUE_BLACK 16
#define COLOR_LIGHT_GRAY 248
#define COLOR_PINK 207
#define COLOR_TURTLE_BLUE 73
#define COLOR_LIGHT_BLUE 27
#define COLOR_ORANGE 202
#define COLOR_DARK_YELLOW 220

/// The numeric code for the highlighting colour pair.
typedef enum {
    /// No syntax highlighting.
    H_NONE,

    /// Mnemonic syntax highlighting.
    /// @example ldr
    H_MNEMONIC,

    /// Label syntax highlighting.
    /// @example loop:
    H_LABEL,

    /// Directive syntax highlighting.
    /// @example .int
    H_DIRECTIVE,

    /// Literal syntax highlighting.
    /// @example #1234
    /// @example 0x12BEEF
    H_LITERAL,

    /// Comment syntax highlighting.
    /// @example // Store the result
    H_COMMENT,

    /// Register syntax highlighting.
    /// @example w30
    H_REGISTER
} HighlightType;


void initialiseHighlight(void);
void wPrintLine(WINDOW *window, char *string);

#endif //HIGHLIGHT_H
