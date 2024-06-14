///
/// editor.c
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#include "editor.h"

int main(void) {
    initscr();            // Start ncurses mode
    printw("Hello, World!"); // Print Hello, World!
    refresh();           // Print it on to the real screen
    getch();             // Wait for user input
    getch();
    endwin();            // End ncurses mode

    return 0;
}
