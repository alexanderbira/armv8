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

static void initialise(const char *path) {
    // Pass input immediately, no echo of input, and display cursor.
    initscr(); raw(); noecho(); curs_set(true);
    getmaxyx(stdscr, rows, cols);

    // We initialise line numbers to be 3 chars wide.
    lineNumbers = newwin(CONTENT_HEIGHT, 3, TITLE_HEIGHT, 0);
    editor = newwin(CONTENT_HEIGHT, (int) cols - 3, TITLE_HEIGHT, 2);

    // Set [editor] to be the only window which receives key presses.
    keypad(stdscr, true);

    // Initialise [File] object.
    file = initialiseFile(path);
}

static void updateLineNumbers(void) {
    int currWidth = getmaxx(lineNumbers);
    int maxLineNumber = file->windowY + rows;
    int maxWidth = (maxLineNumber == 0)
        ? 1
        : (int) floor(log10((double) maxLineNumber)) + 1;

    // Resize line number window if widest line number has changed.
    if (maxWidth + 1 != currWidth) {
        wresize(lineNumbers, CONTENT_HEIGHT, maxWidth + 1);
        wresize(editor, CONTENT_HEIGHT, cols - maxWidth - 1);
    }
}

static void printFile() {
    for (int i = (int) file->windowY; i < (int) file->size; i++) {
        mvwprintw(lineNumbers, i - file->windowY, 0, "%d", i + 1);
        mvwaddstr(editor, i - file->windowY, 0, getLine(file->lines[i]));
        wclrtoeol(editor);
    }

    wmove(lineNumbers, file->size - file->windowY, 0);
    wmove(editor, file->size - file->windowY, 0);
    wclrtobot(lineNumbers);
    wclrtobot(editor);
}

static void handleScroll() {
    while (file->lineNumber >= file->windowY + rows) file->windowY++;
    while (file->lineNumber < file->windowY) file->windowY--;
    while (file->cursor - file->windowX >= cols) file->windowX++;
}

int main(int argc, char *argv[]) {
    initialise((argc > 1) ? argv[1] : NULL);

    int key = -1;
    while (key != QUIT) {

        // Display contents of file.
        printFile();
        wmove(editor, file->lineNumber - file->windowY, file->cursor);
        wrefresh(lineNumbers);
        wrefresh(editor);

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

        updateLineNumbers();
        handleScroll();
    }

    // TODO: save 'n stuff, prompt if no path.

    freeFile(file);
    endwin();

    return 0;
}
