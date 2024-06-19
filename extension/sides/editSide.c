///
/// editSide.c
/// The side panel for the editing of the assembly code.
///
/// Created by Lancelot Liu on 19/06/2024.
///

#include "editSide.h"

static void updateEditLine(unused Line *line, int index);

/// Updates the edit side panel with the current state of the assembly code.
void updateEdit(void) {
    // Print out all lines in current window.
    iterateLinesInWindow(file, &updateEditLine);
}

/// Updates the edit side panel with the current state of the assembly code.
/// @param line The line to rerender.
/// @param index The index of the line in the window.
static void updateEditLine(Line *line, int index) {
    wmove(side, index - file->windowY, 0);

    AssemblerState state = createState();
    fatalError[0] = '\0';
    bool lineErrored = false;

    setjmp(fatalBuffer);
    if (fatalError[0] != '\0') {
        lineErrored = true;
        wattron(side, (file->lineNumber == index) ? COLOR_PAIR(I_ERROR_SCHEME) : COLOR_PAIR(ERROR_SCHEME));
        mvwaddnstr(side, index - file->windowY, 0,
                   fatalError, (cols - 1) / 2);
        wattroff(side, (file->lineNumber == index) ? COLOR_PAIR(I_ERROR_SCHEME) : COLOR_PAIR(ERROR_SCHEME));
    } else {
        parse(getLine(line), &state);

        // Write the natural language version.
        if (state.irCount == 1) {
            char *lineDescription = adecl(state.irList);
            mvwaddnstr(side, index - file->windowY, 0,
                       lineDescription, (cols - 1) / 2);
        }
    }

    destroyState(state);

    wclrtoeol(side);
    rerenderLine(line, index, lineErrored);
}
