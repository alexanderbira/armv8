#include "handleAssembly.h"

void handleAssembly(char *line, AssemblerState *state) {
    char *trimmedLine = trim(line, WHITESPACE);

    // Check if line is blank.
    if (strlen(trimmedLine) == 0) return;

    // Check if line is a comment.
    if (strncmp(trimmedLine, "//", 2) == 0) return;

    // Check if line is a label.
    char *colon = strchr(line, ':');
    if (colon != NULL) {
        // Process as label if first character is valid.
        if (isalpha(line[0]) || line[0] == '_' || line[0] == '.') {
            char *label = strdup(line);
            assertFatalNotNull(label, "<Memory> Unable to duplicate [char *]!");
            *strchr(label, ':') = '\0';

            addMapping(state, label, state->address);
            return;
        };

        throwFatal("Found invalid label!");
    }

    // By default, handle either directive or instructions.
    TokenisedLine tokenisedLine = tokenise(line);
    IR ir = getParser(tokenisedLine.mnemonic)(&tokenisedLine, state);

    state->address += 0x4;
    destroyTokenisedLine(&tokenisedLine);
    addIR(state, ir);
}
