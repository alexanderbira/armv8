///
/// editor.c
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#include "editor.h"

static size_t countDigits(size_t number);

int main(int argc, char *argv[]) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(TRUE);

    File *file = initialiseFile((argc > 1) ? argv[1] : NULL);

    // Note that [rows] and [cols] represent available space for TEXT.
    size_t rows, cols;
    getmaxyx(stdscr, rows, cols);
    size_t prefixPadding = 0;
    char formatString[9];

    int key;
    while (true) {
        // If necessary, refresh format string.
        size_t newPadding = countDigits(file->lineNumber + rows + 1) + 1;
        if (newPadding != prefixPadding) {
            prefixPadding = newPadding;
            snprintf(formatString, sizeof(formatString), "%%%zuzu %%s", prefixPadding);
        }

        // Display contents of file.
        clear();
        for (size_t i = file->windowY; i < file->size; i++) {
            mvprintw(i - file->windowY, 0, formatString, i + 1, getLine(file->lines[i]));
        }

        move(file->lineNumber - file->windowY, file->cursor + prefixPadding + 1);
        refresh();

        // Get and handle input.
        key = getch();

        switch (key) {
            case QUIT:
                // TODO: save 'n stuff, prompt if no path.
                exit(0);

            case SAVE:
                // TODO: save file, prompt if no path.

            case RUN:
                // TODO: Run the assembly.

            case KEY_RESIZE:
                getmaxyx(stdscr, rows, cols);
                // TODO: Mark everything to be re-rendered.

            default: handleFileAction(file, key);
        }

        // Scroll if necessary.
        while (file->lineNumber >= file->windowY + rows) file->windowY++;
        while (file->lineNumber < file->windowY) file->windowY--;
        while (file->cursor - file->windowX >= cols) file->windowX++;
    }

    freeFile(file);
    endwin();

    return 0;
}

static size_t countDigits(size_t number) {
    if (number == 0) return 1;
    return (size_t) floor(log10((double) number)) + 1;
}
