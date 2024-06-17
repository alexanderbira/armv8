///
/// editor.c
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#include "editor.h"

static int rows, cols;

static WINDOW *title, *lineNumbers, *editor, *help, *separator, *regView;

static File *file;

static EditorMode mode;

static void initialise(const char *path);

static void updateUI(void);

static void updateLine(Line *line, int index);

static void updateFile(void);

static void resizeUI(void);

int main(int argc, char *argv[]) {
    initialise((argc > 1) ? argv[1] : NULL);

    int key = -1;
    while (key != QUIT_KEY) {
        // Update what is displayed.
        updateUI();
        updateFile();

        // Get and handle input.
        key = wgetch(editor);
        switch (key) {
            case SAVE_KEY:
                // TODO: save file, prompt if no path.
                break;

            case RUN_KEY:
                // TODO: Run the assembly.
                break;

            case DEBUG_KEY:
                // TODO: Enter debug mode.

            case BINARY_KEY:
                // TODO: Enter binary mode.

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
    init_pair(12, 127, 16);

    title = newwin(TITLE_HEIGHT, cols, 0, 0);
    help = newwin(MENU_HEIGHT, cols, rows - MENU_HEIGHT, 0);
    wbkgd(help, COLOR_PAIR(10));
    wbkgd(title, COLOR_PAIR(10));

    lineNumbers = newwin(CONTENT_HEIGHT, 2, TITLE_HEIGHT, 0);
    wbkgd(lineNumbers, COLOR_PAIR(11));

    editor = newwin(CONTENT_HEIGHT, cols/2 - 2, TITLE_HEIGHT, 2);
    wbkgd(editor, COLOR_PAIR(11));

    separator = newwin(CONTENT_HEIGHT, 1, TITLE_HEIGHT, cols/2);
    wbkgd(separator, COLOR_PAIR(12));
    mvwvline(separator, TITLE_HEIGHT-1, 0, ACS_VLINE, CONTENT_HEIGHT);
    wrefresh(separator);

    regView = newwin(CONTENT_HEIGHT, cols/2 - 1, TITLE_HEIGHT, cols/2 + 1);
    wrefresh(regView);

    // Set [editor] to be the only window which receives key presses.
    keypad(editor, true);

    // Initialise [File] object.
    file = initialiseFile(path);

    // Initialise syntax highlighting.
    initialiseHighlight();
}

/// Prints the given [...] strings equally spaced.
/// @param window The window to print to.
/// @param row The row in the window to print to.
/// @param content The pieces of text to print.
static void printSpaced(WINDOW *window, int row, int count, char **content) {
    if (count == 0) return;
    mvwaddstr(window, row, 0, content[0]);

    for (int i = 1; i < count - 1; i++) {
        mvwaddstr(window, row, (int) (i * cols / (count - 1) - strlen(content[i]) / 2), content[i]);
    }

    if (count == 1) return;
    mvwaddstr(window, row, cols - (int) strlen(content[count - 1]), content[count - 1]);
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
        wresize(editor, CONTENT_HEIGHT, cols/2 - maxWidth - 1);
        mvwin(editor, TITLE_HEIGHT, maxWidth + 1);
    }

    // Update top title bar.
    char **buffer = (char **) malloc(5 * sizeof(char *));
    wattron(title, A_BOLD);

    asprintf(&buffer[0], "[GRIM]");
    asprintf(&buffer[1], "MODE: %s", modes[mode]);
    asprintf(&buffer[2], "PATH: [%s]", file->path ? file->path : "<UNKNOWN>");
    // TODO: Change after status is properly defined.
    asprintf(&buffer[3], "STATUS: %s", "UNSAVED");
    asprintf(&buffer[4], "line %d, col %d", file->lineNumber + 1, file->cursor + 1);
    printSpaced(title, 0, 5, buffer);

    wrefresh(title);
    wattroff(title, A_BOLD);
    for (int i = 1; i < 5; i++) free(buffer[i]);

    // Update bottom help bar.
    wattron(help, A_BOLD);
    printSpaced(help, 0, 5, (char **) commands);
    wrefresh(help);
    wattroff(help, A_BOLD);

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

    wresize(lineNumbers, CONTENT_HEIGHT, 2);
    wrefresh(lineNumbers);

    wresize(editor, CONTENT_HEIGHT, cols/2 - 2);
    wrefresh(editor);

    wresize(regView, CONTENT_HEIGHT, cols/2 - 1);
    mvwin(regView, TITLE_HEIGHT, cols/2 + 1);
    wrefresh(regView);

    wresize(separator, CONTENT_HEIGHT, 1);
    mvwin(separator, TITLE_HEIGHT, cols/2);
    mvwvline(separator, TITLE_HEIGHT-1, 0, ACS_VLINE, CONTENT_HEIGHT);
    wrefresh(separator);

    wclear(help);
    wresize(help, MENU_HEIGHT, cols);
    mvwin(help, rows - MENU_HEIGHT, 0);
}
