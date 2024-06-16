///
/// editor.c
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#include "editor.h"

static int rows, cols;

static WINDOW *lineNumbers, *editor;

static File *file;

static void initialise(const char *path);

static void updateUI(void);

static void updateLine(Line *line, int index);

static void updateFile(void);

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
                // TODO: Mark everything to be re-rendered.
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

    // We initialise line numbers to be 2 chars wide.
    lineNumbers = newwin(CONTENT_HEIGHT, 2, TITLE_HEIGHT, 0);
    editor = newwin(CONTENT_HEIGHT, (int) cols - 2, TITLE_HEIGHT, 2);

    // Set [editor] to be the only window which receives key presses.
    keypad(editor, true);

    // Initialise [File] object.
    file = initialiseFile(path);

    // Initialise syntax highlighting.
    initialiseHighlight();
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
