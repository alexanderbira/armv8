///
/// editor.c
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#include "editor.h"

static int rows, cols;

static WINDOW *title, *lineNumbers, *editor, *menu;

static File *file;

static const char *name     = "[GRIM]";
static const char *mode     = "MODE : EDIT";
static const char *fileName = "[filename.c]";
static const char *status   = "STATUS : SAVED";
static const char *position = "[1,1]";
static const char *command  = "[^Q] - QUIT | [^R] - RUN";

static void initialise(const char *path);

static void initialiseTitle(void);

static void initialiseMenu(void);

static void updateUI(void);

static void updateLine(Line *line, int index);

static void updateFile(void);

static void resizeUI(void);

int main(int argc, char *argv[]) {
    initialise((argc > 1) ? argv[1] : NULL);

    int key = -1;
    while (key != QUIT) {
        // Update what is displayed.
        updateUI();
        updateFile();

        // Get and handle input.
        key = wgetch(editor);
        switch (key) {
            case SAVE:
                // TODO: save file, prompt if no path.
                break;

            case RUN:
                // TODO: Run the assembly.
                break;

            case KEY_RESIZE:
                getmaxyx(stdscr, rows, cols);
                resizeUI();
                break;

            default:
                handleFileAction(file, key);
                break;
        }
    }

    // TODO: save 'n stuff, prompt if no path.

    // Cleanup
    freeFile(file);
    endwin();

    return 0;
}

static void initialise(const char *path) {
    // Initialise <ncurses>.
    initscr();       // ncurses setup.
    raw();           // Pass raw input to our code.
    noecho();        // Do not echo back when getting input.
    curs_set(true);  // Display cursor.
    getmaxyx(stdscr, rows, cols);

    start_color();
    init_pair(10, 15, 127);
    init_pair(11, 15, 16);

    title = newwin(TITLE_HEIGHT, cols, 0, 0);
    wbkgd(title, COLOR_PAIR(10));
    initialiseTitle();

    lineNumbers = newwin(CONTENT_HEIGHT, 2, TITLE_HEIGHT, 0);
    wbkgd(lineNumbers, COLOR_PAIR(11));

    editor = newwin(CONTENT_HEIGHT, cols - 2, TITLE_HEIGHT, 2);
    wbkgd(editor, COLOR_PAIR(11));

    menu = newwin(MENU_HEIGHT, cols, rows - MENU_HEIGHT, 0);
    wbkgd(menu, COLOR_PAIR(10));
    initialiseMenu();

    // Set [editor] to be the only window which receives key presses.
    keypad(editor, true);

    // Initialise [File] object.
    file = initialiseFile(path);

    // Initialise syntax highlighting.
    initialiseHighlight();
}

static void initialiseTitle(void) {
    wattron(title, A_BOLD);
    mvwprintw(title, 0, 0, "%s", name);
    mvwprintw(title, 0, (int) (cols / 4 - strlen(mode) / 2),       "%s", mode);
    mvwprintw(title, 0, (int) (cols / 2 - strlen(fileName) / 2),   "%s", fileName);
    mvwprintw(title, 0, (int) (3 * cols / 4 - strlen(status) / 2), "%s", status);
    mvwprintw(title, 0, (int) (cols - strlen(position)),           "%s", position);
    wrefresh(title);
    wattroff(title, A_BOLD);
}

static void initialiseMenu(void) {
    wattron(menu, A_BOLD);
    mvwprintw(menu, 0, (int) (cols - strlen(command)) / 2, "%s", command);
    wrefresh(menu);
    wattroff(menu, A_BOLD);
}

/// Updates UI, including line numbers window, scrolling.
static void updateUI(void) {
    // Calculate the maximum width of a line number currently possible.
    int currWidth = getmaxx(lineNumbers);

    // The largest line number is either the bottom of the screen, or
    // the last line of the file.
    int maxLineNumber = file->windowY + CONTENT_HEIGHT + 1;
    maxLineNumber = (file->size < maxLineNumber) ? file->size : maxLineNumber;
    int maxWidth = countDigits(maxLineNumber);

    // Resize line number window if widest line number has changed.
    if (maxWidth + 1 != currWidth) {
        wresize(lineNumbers, CONTENT_HEIGHT, maxWidth + 1);
        mvwin(lineNumbers, TITLE_HEIGHT, 0);
        wresize(editor, CONTENT_HEIGHT, cols - maxWidth - 1);
        mvwin(editor, TITLE_HEIGHT, maxWidth + 1);
    }

    // Scroll if out of bounds in any direction.
    if (file->lineNumber >= file->windowY + CONTENT_HEIGHT) {
        file->windowY = file->lineNumber - CONTENT_HEIGHT + 1;
    }
    if (file->lineNumber < file->windowY) file->windowY = file->lineNumber;
    if (file->cursor >= file->windowX + cols) {
        file->windowX = file->cursor - cols + 1;
    }
    if (file->cursor < file->windowX) file->windowX = file->cursor;

    // Scroll if last line could be at bottom of screen but is not.
    if (CONTENT_HEIGHT - file->lineNumber < 2) {
        file->windowY = file->lineNumber - CONTENT_HEIGHT + 1;
    }
}

/// Updates the contents of one line, with corresponding line number.
/// @param line The [Line] to be updated on screen.
static void updateLine(Line *line, int index) {
    // Print the line number, then clear rest of line.
    int padding = getmaxx(lineNumbers) - countDigits(index + 1) - 1;
    wmove(lineNumbers, index - file->windowY, 0);
    wclrtoeol(lineNumbers);
    mvwprintw(lineNumbers, index - file->windowY, padding, "%d", index + 1);

    // Print the line contents, then clear rest of line.
    wmove(editor, index - file->windowY, 0);
    wPrintLine(editor, getLine(line));
    wclrtoeol(editor);
}

/// Updates the contents of the file, with corresponding line numbers.
static void updateFile(void) {
    // Print out all lines in current window.
    iterateLinesInWindow(file, &updateLine);

    // Clear all unoccupied space in line number and editor windows.
    wmove(lineNumbers, file->size - file->windowY, 0);
    wclrtobot(lineNumbers);

    wmove(editor, file->size - file->windowY, 0);
    wclrtobot(editor);

    // Move cursor to new position.
    wmove(editor, file->lineNumber - file->windowY, file->cursor);

    // Refresh relevant windows.
    wrefresh(lineNumbers);
    wrefresh(editor);
}

static void resizeUI(void) {
    wclear(title);
    wresize(title, TITLE_HEIGHT, cols);
    initialiseTitle();

    wresize(lineNumbers, CONTENT_HEIGHT, 2);
    wrefresh(lineNumbers);

    wresize(editor, CONTENT_HEIGHT, cols - 2);
    wrefresh(editor);

    wclear(menu);
    wresize(menu, MENU_HEIGHT, cols);
    mvwin(menu, rows - MENU_HEIGHT, 0);
    initialiseMenu();
}
