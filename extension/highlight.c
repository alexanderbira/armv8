///
/// highlight.c
/// Print a line of code with syntax-highlighting.
///
/// Created by Alexander Biraben-Renard on 15/06/2024.
///

#include "highlight.h"

#define COLOR_TRUE_BLACK 16
#define COLOR_LIGHT_GRAY 248
#define COLOR_PINK 207
#define COLOR_TURTLE_BLUE 73
#define COLOR_LIGHT_BLUE 27
#define COLOR_ORANGE 202
#define COLOR_DARK_YELLOW 220

/// Mnemonics of all instructions.
static const char *mnemonics[] = {
        "add",  "adds", "and",
        "and",  "ands", "b",
        "b.al", "b.eq", "b.ge",
        "b.gt", "b.le", "b.lt",
        "b.ne", "bic",  "bics",
        "br",   "cmn",  "cmp",
        "eon",  "eor",  "ldr",
        "madd", "mneg", "mov",
        "movk", "movn", "movz",
        "msub", "mul",  "mvn",
        "neg",  "negs", "orn",
        "orr",  "str",  "subs",
        "tst"
};

/// Compare pointers to strings by the strings to which they point.
/// @param s1 The pointer to the first string.
/// @param s2 The pointer to the second string.
/// @return The comparison of the strings pointed to by s1 and s2.
static int strPtrCmp(char **ptr1, char **ptr2) {
    return strcmp(*ptr1, *ptr2);
}

/// Initialise the colouring for the syntax highlighting.
void initialiseHighlight(void) {
    start_color();

    init_pair(H_NONE, COLOR_WHITE, COLOR_TRUE_BLACK);
    init_pair(H_MNEMONIC, COLOR_PINK, COLOR_TRUE_BLACK);
    init_pair(H_LABEL, COLOR_TURTLE_BLUE, COLOR_TRUE_BLACK);
    init_pair(H_DIRECTIVE, COLOR_LIGHT_BLUE, COLOR_TRUE_BLACK);
    init_pair(H_LITERAL, COLOR_ORANGE, COLOR_TRUE_BLACK);
    init_pair(H_COMMENT, COLOR_LIGHT_GRAY, COLOR_TRUE_BLACK);
    init_pair(H_REGISTER, COLOR_DARK_YELLOW, COLOR_TRUE_BLACK);
}

/// Print a line with syntax-highlighting at the current cursor position.
/// @param window The window in which to print the line.
/// @param string The string to syntax-highlight and print.
void wPrintLine(WINDOW *window, char *string) {

    // The index of the last char of the line which has been printed.
    int printedIndex = 0;

    // Skip whitespace.
    while (isspace(string[printedIndex])) {
        waddch(window, string[printedIndex]);
        printedIndex++;
    }

    // Whether it's the first token on the line.
    bool firstToken = true;

    // This loop will run once per "token" in the line.
    while (string[printedIndex] != '\0') {

        // The type of token we're dealing with.
        HighlightType highlightType = H_NONE;

        // The index of last scanned char in the line/token.
        int scannedIndex = printedIndex;

        // Check if we're at the start of a comment.
        if (string[printedIndex] == '/' && string[printedIndex+1] == '/') {
            highlightType = H_COMMENT;
            scannedIndex = strlen(string);

        } else {

            // Flags for scanning through the token.

            // If the last char in the token was a colon.
            bool lastWasColon = false;

            // If the whole token was alphanumeric.
            bool wholeAlphaNum = true;

            // If the whole token was hexadecimal.
            bool wholeHex = true;

            // If the whole token was numerical.
            bool wholeNum = true;

            // If the token started with "0x".
            bool hexStart = false;

            // The first char of the token.
            char first = string[scannedIndex];

            // Ensure other flags check only after a special first char.
            if (
                first == '#' || // Decimal literals.
                tolower(first) == 'x' || // Register name.
                tolower(first) == 'w'    // Register name.
            ) {
                scannedIndex++;
            }

            // Detect hexadecimal literal.
            if (first == '0' && string[scannedIndex+1] == 'x') {
                hexStart = true;
                scannedIndex += 2;
            }

            // Scan through to a space, comma, square bracket, or end of
            // string.
            while (
                !isspace(string[scannedIndex]) &&
                string[scannedIndex] != '\0' &&
                string[scannedIndex] != ',' &&
                string[scannedIndex] != '[' &&
                string[scannedIndex] != ']'
            ) {
                char currChar = string[scannedIndex];

                // End parsing of this token if it's followed by a comment.
                if (currChar == '/' && string[scannedIndex + 1] == '/') {
                    break;
                }

                // Ensure the token is classed as alphanumeric, even if it ends
                // in a colon. (For loop labels)
                if (currChar == ':') {
                    // Current char is a colon

                    // It's been 2 colons in a row, so not alphanumeric.
                    if (lastWasColon) {
                        wholeAlphaNum = false;
                    }

                    lastWasColon = true;

                } else if (!lastWasColon) {
                    // Current char isn't a colon and the previous char was a
                    // colon. (General case)

                    lastWasColon = false;
                    wholeAlphaNum = wholeAlphaNum && (
                        isalpha(currChar) ||
                        isdigit(currChar)
                    );

                } else {
                    // Current char isn't a colon and the last char was a
                    // colon
                    wholeAlphaNum = false;
                    lastWasColon = false;
                }

                wholeHex = wholeHex && isxdigit(currChar);
                wholeNum = wholeNum && isdigit(currChar);

                scannedIndex++;
            }

            int tokenLength = scannedIndex - printedIndex;
            char *tokenPtr = &string[printedIndex];

            // Make a copy of the token for searching in the list of mnemonics.
            char *tokenCopy = malloc(sizeof(char) * (tokenLength+1));
            strncpy(tokenCopy, tokenPtr, tokenLength);
            tokenCopy[tokenLength] = '\0';

            if (
                // Token is at most 4 chars.
                tokenLength <= 4 &&
                // Token is the first token on the line.
                firstToken &&
                // The token is in the list of mnemonics.
                bsearch(
                    &tokenCopy,
                    mnemonics,
                    sizeof(mnemonics) / sizeof(char *),
                    sizeof(char *),
                    (int (*)(const void *, const void *)) strPtrCmp
                ) != NULL
            ) {
                highlightType = H_MNEMONIC;

            } else if (
                // Token is alphanumeric (before the ':').
                wholeAlphaNum &&
                // Token ends with ':'.
                lastWasColon
            ) {
                highlightType = H_LABEL;

            } else if (
                // Token is 4 chars long.
                tokenLength == 4 &&
                // Token is the first token on the line.
                firstToken &&
                // Token is ".int".
                strncmp(tokenPtr, ".int", 4) == 0
            ) {
                highlightType = H_DIRECTIVE;

            } else if (
                // Token is not the first token on the line.
                !firstToken &&
                (
                    // Decimal literal.
                    (
                        // Token starts with '#'.
                        first == '#' &&
                        // Token is fully numeric.
                        wholeNum &&
                        // Token is over 1 char long.
                        tokenLength > 1
                    ) ||
                    // Hexadecimal literal.
                    (
                        // Token starts with "0x".
                        hexStart &&
                        // Token contains only hexadecimal digits.
                        wholeHex &&
                        // Token is over 2 chars long.
                        tokenLength > 2
                    )
                )
            ) {
                highlightType = H_LITERAL;

            } else if (
                // Token is not the first token on the line.
                !firstToken &&
                // Token starts with one of 'x', 'X', 'w', or 'W'.
                (tolower(first) == 'w' || tolower(first) == 'x') &&
                // Token is fully numeric (after the first char).
                wholeNum &&
                // Token is longer than 1 char.
                tokenLength > 1 &&
                // Token number is smaller than or equal to 31.
                atoi(&string[printedIndex+1]) <= 31 &&
                // Token number has no preceding 0s.
                (
                    (
                        atoi(&string[printedIndex+1]) >= 10 &&
                        tokenLength == 3
                    ) ||
                    (
                        atoi(&string[printedIndex+1]) <= 9 &&
                        tokenLength == 2
                    )
                )
            ) {
                highlightType = H_REGISTER;
            }

            free(tokenCopy);
        }


        // Print the token.
        wattron(window, COLOR_PAIR(highlightType));
        while (printedIndex < scannedIndex) {
            waddch(window, string[printedIndex]);
            printedIndex++;
        }
        wattroff(window, COLOR_PAIR(highlightType));

        // Skip space, comma, or square bracket.
        while (
            isspace(string[printedIndex]) ||
            string[printedIndex] == ',' ||
            string[printedIndex] == '[' ||
            string[printedIndex] == ']'
        ) {
            waddch(window, string[printedIndex]);
            printedIndex++;
        }

        firstToken = false;
    }
}
