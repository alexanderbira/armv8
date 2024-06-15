///
/// editor.c
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#include "editor.h"

// Function prototypes
void displayFile(File *file);

int main(int argc, char *argv[]) {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(TRUE);

    // Initialize the file
    File *file = NULL;
    if (argc > 1) {
        file = initialiseFile(argv[1]);
    } else {
        file = initialiseFile(NULL);
    }

    int ch;
    while ((ch = getch()) != 'q') {
        handleKey(file, ch);

        clear();
        displayFile(file);
        move(file->lineNumber, file->cursor);
        refresh();
    }

    freeFile(file);
    endwin();

    return 0;
}

void displayFile(File *file) {
    for (size_t i = 0; i < file->size; i++) {
        mvprintw(i, 0, "%s", getLine(file->lines[i]));
    }
}
