///
/// saveOverlay.c
/// The destructive overlay displayed when the user wishes to save but no path is recorded.
///
/// Created by Lancelot Liu on 18/06/2024.
///

#include "saveOverlay.h"

static const char *overlayText = "[ Could not save file! ]";

static const int overlayLength = 24;

static const char *promptFormat = "[ Save To: %s ]";

static const int promptLength = 13;

bool showSaveOverlay(File *file) {
    // Create a window for the overlay with dimensions covering the entire screen
    WINDOW *saveOverlay = newwin(0, 0, 0, 0);
    wbkgd(saveOverlay, COLOR_PAIR(10));
    keypad(saveOverlay, TRUE);

    char userPath[1024] = "";
    int length = 0;

    mvwprintw(saveOverlay, rows / 2, (cols - promptLength) / 2, promptFormat, "");
    wmove(saveOverlay, rows / 2, (cols + promptLength) / 2 - 2);
    wrefresh(saveOverlay);

    // Process user input
    int key;
    while ((key = wgetch(saveOverlay)) != KEY_F(1)) {
        switch (key) {
            case KEY_BACKSPACE:
            case 127:
                if (length > 0) userPath[--length] = '\0';
                break;

            case KEY_ENTER:
            case 10: {
                char *trimmedPath = trim(userPath, " ");
                file->path = strdup(trimmedPath);
                if (saveFile(file)) {
                    delwin(saveOverlay);
                    endwin();
                    return true;
                } else {
                    mvwprintw(saveOverlay, rows / 2 + 2, (cols - overlayLength) / 2, overlayText);
                    wrefresh(saveOverlay);
                }
                break;
            }

            case 27: // ESC key
                delwin(saveOverlay);
                endwin();
                return false;

            default:
                if (isprint(key)) {
                    userPath[length++] = key;
                    userPath[length] = '\0';
                }
                break;
        }

        // Clear the line and render the updated input
        wmove(saveOverlay, rows / 2, 0);
        wclrtoeol(saveOverlay);
        mvwprintw(saveOverlay, rows / 2, (cols - promptLength - length) / 2, promptFormat, userPath);
        wmove(saveOverlay, rows / 2, (cols + promptLength + length) / 2 - 2);
        wrefresh(saveOverlay);
    }

    // Clean up
    delwin(saveOverlay);
    endwin();

    return false;
}
