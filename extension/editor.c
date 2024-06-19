///
/// editor.c
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#include "editor.h"

int rows = 0, cols = 0;

WINDOW *title, *lineNumbers, *editor, *help, *separator, *regView;

static File *file;

/// The current editor mode.
static EditorMode mode;

/// The current editor status.
static EditorStatus status;

/// Info about the lines after attempt to assemble.
static LineInfo *lineInfo;

static void initialise(const char *path);

static void updateUI(void);

static void updateLine(Line *line, int index);

static void printSpaced(WINDOW *window, int row, int count, char **content);

int main(int argc, char *argv[]) {
    initialise((argc > 1) ? argv[1] : NULL);

    int key = -1;
    while (key != QUIT_KEY) {
        updateUI();

        // Get and handle input.
        key = wgetch(editor);

        switch (key) {
            case SAVE_KEY:
                status = (file->path)
                         ? (saveFile(file) ? SAVED : status)
                         : (showSaveOverlay(file) ? SAVED : status);
                updateUI();
                break;

            case RUN_KEY: {
                AssemblerState state = createState();
                if (!setjmp(fatalBuffer)) {
                    for (int currentLine = 0; currentLine < file->size; currentLine++) {
                        parse(getLine(file->lines[currentLine]), &state);
                    }
                } else {
                    // TODO: Send the error message where needed.
                }
                break;
            }

            case DEBUG_KEY:
                // TODO: Enter debug mode.
                break;

            case BINARY_KEY:
                // Toggle binary mode.
                mode = (mode == BINARY) ? EDIT : BINARY;
                break;

            default:
                if (status == READ_ONLY) break;
                status = handleFileAction(file, key) ? UNSAVED : status;
                break;
        }
    }

    // TODO: save 'n stuff, prompt if no path.

    // Cleanup
    freeFile(file);
    free(fatalError);
    endwin();

    return 0;
}

/// Convert an instruction to a binary string representation.
/// @param str A pointer to the char array to overwrite with the binary repr.
/// @param instruction The instruction to convert.
static void strBinRep(char *str, Instruction instruction) {
    int i = sizeof(instruction) * 8 + 7;

    str[i] = '\0';

    while (--i >= 0) {
        str[i] = '0' + (instruction & 1);
        if (i % 5 == 0) {
            str[--i] = ' ';
        }
        instruction >>= 1;
    }
}

static void initialise(const char *path) {
    // fatalError is "realloc"ed elsewhere.
    fatalError = malloc(0);

    // Initialise <ncurses>.
    initscr();       // ncurses setup.
    raw();           // Pass raw input to our code.
    noecho();        // Do not echo back when getting input.
    curs_set(true);  // Display cursor.
    getmaxyx(stdscr, rows, cols);

    start_color();
    init_pair(10, 15, 127);
    init_pair(11, 15, 16);
    init_pair(12, 15, 16);
    init_pair(13, 1, 16);

    title = newwin(TITLE_HEIGHT, cols, 0, 0);
    help = newwin(MENU_HEIGHT, cols, rows - MENU_HEIGHT, 0);
    wbkgd(help, COLOR_PAIR(10));
    wbkgd(title, COLOR_PAIR(10));

    lineNumbers = newwin(CONTENT_HEIGHT, 2, TITLE_HEIGHT, 0);
    wbkgd(lineNumbers, COLOR_PAIR(11));

    editor = newwin(CONTENT_HEIGHT, cols / 2 - 2, TITLE_HEIGHT, 2);
    wbkgd(editor, COLOR_PAIR(11));

    separator = newwin(CONTENT_HEIGHT, 1, TITLE_HEIGHT, cols / 2);
    wbkgd(separator, COLOR_PAIR(12));

    regView = newwin(CONTENT_HEIGHT, (cols - 1) / 2, TITLE_HEIGHT, cols / 2 + 1);
    wbkgd(regView, COLOR_PAIR(13));

    // Set [editor] to be the only window which receives key presses.
    keypad(editor, true);

    // Initialise [File] object.
    file = initialiseFile(path);

    // Set mode and state.
    mode = EDIT;
    status = file->path ? SAVED : UNSAVED; // TODO: Dynamically change to read only if loaded file is read-only.

    // Initialise syntax highlighting.
    initialiseHighlight();
    updateUI();
}

/// Updates UI, including line numbers window, scrolling.
static void updateUI(void) {
    // Check if the terminal size has changed.
    int oldRows = rows, oldCols = cols;
    getmaxyx(stdscr, rows, cols);
    if (rows < MINIMUM_HEIGHT || cols < MINIMUM_WIDTH) {
        showTermSizeOverlay();
        return;
    }

    // Resize all windows if size has changed.
    if (oldRows != rows || oldCols != cols) {
        // Resize content.
        werase(lineNumbers);
        wresize(lineNumbers, CONTENT_HEIGHT, getmaxx(lineNumbers));
        mvwin(lineNumbers, TITLE_HEIGHT, 0);

        werase(editor);
        wresize(editor, CONTENT_HEIGHT, cols / 2 - 2);
        mvwin(editor, TITLE_HEIGHT, getmaxx(lineNumbers));

        werase(regView);
        wresize(regView, CONTENT_HEIGHT, (cols - 1) / 2);
        mvwin(regView, TITLE_HEIGHT, cols / 2 + 1);

        // Rerender menu bars and separator.
        werase(title);
        wresize(title, TITLE_HEIGHT, cols);
        mvwin(title, 0, 0);

        werase(help);
        wresize(help, MENU_HEIGHT, cols);
        mvwin(help, rows - MENU_HEIGHT, 0);

        werase(separator);
        wresize(separator, CONTENT_HEIGHT, 1);
        mvwin(separator, TITLE_HEIGHT, cols / 2);
    }

    // Update top title bar.
    char **buffer = (char **) malloc(5 * sizeof(char *));
    wattron(title, A_BOLD);

    asprintf(&buffer[0], "[GRIM]");
    asprintf(&buffer[1], "MODE: %s", modes[mode]);
    asprintf(&buffer[2], "%s", file->path ? file->path : "unknown.c");
    // TODO: Change after status is properly defined.
    asprintf(&buffer[3], "STATUS: %s", statuses[status]);
    asprintf(&buffer[4], "[%d, %d]", file->lineNumber + 1, file->cursor + 1);
    werase(title);
    printSpaced(title, 0, 5, buffer);

    wattroff(title, A_BOLD);
    for (int i = 1; i < 5; i++) free(buffer[i]);
    wrefresh(title);

    // Update bottom help bar.
    wattron(help, A_BOLD);
    werase(help);
    printSpaced(help, 0, 5, (char **) commands);
    wattroff(help, A_BOLD);
    wrefresh(help);

    // Update separator.
    mvwvline(separator, TITLE_HEIGHT - 1, 0, ACS_VLINE, CONTENT_HEIGHT);
    wrefresh(separator);

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

    // The largest line number is either the bottom of the screen, or
    // the last line of the file.
    int currWidth = getmaxx(lineNumbers);
    int maxLineNumber = file->windowY + CONTENT_HEIGHT + 1;
    maxLineNumber = (file->size < maxLineNumber) ? file->size : maxLineNumber;
    int maxWidth = countDigits(maxLineNumber);

    // Resize line number window if widest line number has changed.
    if (maxWidth + 1 != currWidth) {
        wresize(lineNumbers, CONTENT_HEIGHT, maxWidth + 1);
        mvwin(lineNumbers, TITLE_HEIGHT, 0);
        wresize(editor, CONTENT_HEIGHT, cols / 2 - maxWidth - 1);
        mvwin(editor, TITLE_HEIGHT, maxWidth + 1);
    }

    switch (mode) {
        case BINARY:
            // Allocate memory for the lineInfo array.
            lineInfo = malloc(file->size * sizeof(LineInfo));

            AssemblerState state = createState();

            // Perform the first pass of the assembler.
            for (int i = 0; i < file->size; i++) {
                size_t irCount = state.irCount;

                LineInfo currLineInfo;
                fatalError[0] = '\0';

                setjmp(fatalBuffer);
                if (fatalError[0] != '\0') {
                    // An error has occured.
                    currLineInfo.lineStatus = ERRORED;
                    currLineInfo.data.error = strdup(fatalError);
                } else {
                    parse(getLine(file->lines[i]), &state);

                    // At this point, the parsing was successful.
                    currLineInfo.lineStatus = (state.irCount == irCount) ? NONE : ASSEMBLED;
                    currLineInfo.data.error = NULL;
                }

                lineInfo[i] = currLineInfo;
            }

            // Keep track of which line we're doing a second pass on, and reset counter.
            int currLineNum = 0;
            state.address = 0x0;

            // Perform the second pass of the assembler.
            for (size_t i = 0; i < state.irCount; i++) {
                // Skip lines which do not require a second pass.
                while (lineInfo[currLineNum].lineStatus != ASSEMBLED) currLineNum++;

                IR ir = state.irList[i];
                fatalError[0] = '\0';

                setjmp(fatalBuffer);
                if (fatalError[0] != '\0') {
                    // Fatal error was encountered during second assembly pass.
                    free(lineInfo[currLineNum].data.error);
                    lineInfo[currLineNum].lineStatus = ERRORED;
                    lineInfo[currLineNum].data.error = strdup(fatalError);
                } else {
                    Instruction instruction = getTranslator(&ir.type)(&ir, &state);

                    // If this code is reached, the line was assembled.
                    lineInfo[currLineNum].lineStatus = ASSEMBLED;
                    lineInfo[currLineNum].data.instruction = instruction;
                }

                state.address += 0x4;
                currLineNum++;
            }

            // Print out all lines in current window.
            iterateLinesInWindow(file, &updateLine);

            // Free the line info
            destroyState(state);
            for (int i = 0; i < file->size; i++) {
                // Free all error strings
                if (lineInfo[i].lineStatus == ERRORED) {
                    free(lineInfo[i].data.error);
                }
            }
            free(lineInfo);
            break;

        default:
            // Print out all lines in current window.
            iterateLinesInWindow(file, &updateLine);
            break;
    }

    // Clear all unoccupied space in line number and editor windows.
    wclrtobot(regView);
    wrefresh(regView);
    wclrtobot(lineNumbers);
    wrefresh(lineNumbers);
    wclrtobot(editor);

    // Move cursor to new position.
    wmove(editor, file->lineNumber - file->windowY, file->cursor);
    wrefresh(editor);
}

/// Updates the contents of one line, with corresponding line number.
/// @param line The [Line] to be updated on screen.
/// @param index The 0-based index of the line to update.
static void updateLine(Line *line, int index) {
    // Don't render line below screen.
    if (index >= file->windowY + CONTENT_HEIGHT) return;

    // Print the line number, then clear rest of line.
    int padding = getmaxx(lineNumbers) - countDigits(index + 1) - 1;

    // Reset the cursors.
    wmove(editor, index - file->windowY, 0);
    wmove(lineNumbers, index - file->windowY, 0);
    wmove(regView, index - file->windowY, 0);

    // Whether a fatal error was encountered when parsing the line.
    bool lineError = false;

    // Print different things in the regView window depending on the mode.
    switch (mode) {
        case BINARY:
            switch (lineInfo[index].lineStatus) {
                case ERRORED:
                    lineError = true;

                    // Display the error
                    wattron(regView, COLOR_PAIR(13));
                    mvwaddnstr(regView, index - file->windowY, 0,
                               lineInfo[index].data.error, (cols - 1) / 2);
                    wattroff(regView, COLOR_PAIR(13));
                    break;

                case ASSEMBLED: {
                    // Convert the instruction to a string.
                    char instrStr[8 * sizeof(Instruction) + 8];
                    strBinRep(instrStr, lineInfo[index].data.instruction);

                    // Display the binary string.
                    wattron(regView, COLOR_PAIR(11));
                    mvwaddnstr(regView, index - file->windowY, 0,
                               instrStr, (cols - 1) / 2);
                    wattroff(regView, COLOR_PAIR(11));
                    break;
                }

                default:
                    break;
            }

            break;

        default: {
            AssemblerState state = createState();
            fatalError[0] = '\0';

            setjmp(fatalBuffer);
            if (fatalError[0] != '\0') {
                lineError = true;
                mvwaddnstr(regView, index - file->windowY, 0,
                           fatalError, (cols - 1) / 2);
            } else {
                parse(getLine(line), &state);
            }

            destroyState(state);
            break;
        }
    }

    wclrtoeol(regView);
    wclrtoeol(lineNumbers);

    if (!lineError || file->lineNumber == index) {
        // Display editor line with syntax highlighting.
        mvwprintw(lineNumbers, index - file->windowY, padding, "%d", index + 1);
        wPrintLine(editor, getLine(line));
    } else {
        // Print the line number in red.
        wattron(lineNumbers, COLOR_PAIR(13));
        mvwprintw(lineNumbers, index - file->windowY, padding, "%d", index + 1);
        wattroff(lineNumbers, COLOR_PAIR(13));

        // Display editor line in red.
        wattron(editor, COLOR_PAIR(13));
        wprintw(editor, "%s", getLine(line));
        wattroff(editor, COLOR_PAIR(13));
    }

    wclrtoeol(editor);
    wclrtoeol(lineNumbers);
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
