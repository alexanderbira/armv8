///
/// editor.c
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#include "editor.h"

static size_t rows, cols;

static size_t prefixPadding = 0;

static char formatString[9];

static size_t countDigits(size_t number);

static void initialiseEditor() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(TRUE);
}

static void printFile(File *file) {
    for (int i = (int) file->windowY; i < (int) file->size; i++) {
        mvprintw(i - (int) file->windowY, 0, formatString, i + 1, getLine(file->lines[i]));
    }
}

static void handleScroll(File *file) {
    while (file->lineNumber >= file->windowY + rows) file->windowY++;
    while (file->lineNumber < file->windowY) file->windowY--;
    while (file->cursor - file->windowX >= cols) file->windowX++;
}

int main(int argc, char *argv[]) {
    initialiseEditor();
    File *file = initialiseFile((argc > 1) ? argv[1] : NULL);
    getmaxyx(stdscr, rows, cols);

    int key = -1;
    while (key != QUIT) {
        // If necessary, refresh format string.
        size_t newPadding = countDigits(file->lineNumber + rows + 1) + 1;
        if (newPadding != prefixPadding) {
            prefixPadding = newPadding;
            snprintf(formatString, sizeof(formatString), "%%%zuzu %%s", prefixPadding);
        }

        // Display contents of file.
        clear();
        printFile(file);
        move(file->lineNumber - file->windowY, file->cursor + prefixPadding + 1);
        refresh();

        // Get and handle input.
        key = getch();
        switch (key) {
            case SAVE:
                // TODO: save file, prompt if no path.
                break;
            case RUN:
                // TODO: Run the assembly.
                break;
            case KEY_RESIZE:
                getmaxyx(stdscr, rows, cols);
                // TODO: Mark everything to be re-rendered.
                break;
            default: handleFileAction(file, key);
        }

        handleScroll(file);
    }

    // TODO: save 'n stuff, prompt if no path.

    freeFile(file);
    endwin();

    return 0;
}

static size_t countDigits(size_t number) {
    if (number == 0) return 1;
    return (size_t) floor(log10((double) number)) + 1;
}
