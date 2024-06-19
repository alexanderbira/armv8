///
/// termSizeOverlay.c
/// The destructive overlay displayed when the terminal size is too small.
///
/// Created by Lancelot Liu on 18/06/2024.
///

#include "termSizeOverlay.h"

static const char *overlayText = "[ Terminal size too small! ]";

static const int overlayLength = 28;

void showTermSizeOverlay(void) {
    WINDOW *termSizeOverlay = newwin(0, 0, 0, 0);
    wbkgd(termSizeOverlay, COLOR_PAIR(MENU_SCHEME));
    curs_set(false);

    // Intercept all keys, including resize events.
    while (true) {
        getmaxyx(stdscr, rows, cols);
        if (rows >= MINIMUM_HEIGHT && cols >= MINIMUM_WIDTH) break;

        wclear(termSizeOverlay);
        mvwaddstr(termSizeOverlay, rows / 2, cols / 2 - overlayLength / 2, overlayText);
        wrefresh(termSizeOverlay);

        wgetch(termSizeOverlay);
    }

    delwin(termSizeOverlay);
    curs_set(true);
}
