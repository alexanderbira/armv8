///
/// termSizeOverlay.c
/// The destructive overlay displayed when the terminal size is too small.
///
/// Created by Lancelot Liu on 18/06/2024.
///

#include "termSizeOverlay.h"

static const char *overlayText = "[ Terminal size too small! ]";

static const int overlayLength = 28;

void updateTermSizeOverlay(bool active) {
    // Do not update if overlay is not active.
    if (!active) {
        if (termSizeOverlay != NULL) {
            delwin(termSizeOverlay);
            termSizeOverlay = NULL;
            curs_set(true);
        }
        return;
    };

    // Create the overlay if it does not exist.
    if (termSizeOverlay == NULL) {
        termSizeOverlay = newwin(0, 0, 0, 0);
        wbkgd(termSizeOverlay, COLOR_PAIR(10));
        curs_set(false);

        wclear(title);
        wclear(help);
        wclear(lineNumbers);
        wclear(editor);
        wclear(separator);
        wclear(regView);
    }

    wclear(termSizeOverlay);
    mvwaddstr(termSizeOverlay, rows / 2, cols / 2 - overlayLength / 2, overlayText);
    wrefresh(termSizeOverlay);
}
