///
/// editor.c
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#include "editor.h"

static void initialise(const char *path);

static void updateUI(void);

static void printSpaced(WINDOW *window, int row, int count, char **content);

int main(int argc, char *argv[]) {
    initialise((argc > 1) ? argv[1] : NULL);

    // Was the last keypress ^R?
    bool justRan = false;

    int key = -1;
    while (key != QUIT_KEY) {
        // Don't update the UI if we just ran the code.
        if (!justRan) {
            updateUI();
        }
        justRan = false;

        // Get and handle input.
        key = wgetch(editor);

        switch (key) {
            case SAVE_KEY:
                status = (file->path)
                         ? (saveFile(file) ? SAVED : status)
                         : (showSaveOverlay(file) ? SAVED : status);
                break;

            case RUN_KEY: {
                // Run the code.

                // Initialise registers, memory, and assembler state.
                Registers_s registersStruct = createRegs();
                Registers registers = &registersStruct;
                Memory memory = allocMem();
                AssemblerState state = createState();

                // Initialise error string.
                fatalError[0] = '\0';

                if (!setjmp(fatalBuffer)) {

                    // Perform the first assembly pass.
                    for (int currentLine = 0; currentLine < file->size; currentLine++) {
                        parse(getLine(file->lines[currentLine]), &state);
                    }

                    state.address = 0x0;

                    // Perform the second pass and write the instruction to memory.
                    for (size_t i = 0; i < state.irCount; i++) {
                        IR ir = state.irList[i];
                        Instruction instruction = getTranslator(&ir.type)(&ir, &state);
                        writeMem(memory, false, state.address, instruction);
                        state.address += 0x4;
                    }

                    // Fetch first instruction.
                    Instruction instruction = readMem(memory, false, getRegPC(registers));

                    // Fetch, decode, execute cycle while the program has not terminated.
                    while (instruction != HALT) {
                        execute(&instruction, registers, memory);
                    }

                }

                // Display the register states.
                updateDebug(registers);

                // Free the emulator and assembler states.
                freeMem(memory);
                destroyState(state);

                justRan = true;
            }

                break;

            case DEBUG_KEY:
                mode = (mode == DEBUG) ? EDIT : DEBUG;
                status = (mode == DEBUG) ? READ_ONLY : UNSAVED;
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

/// Initialises the editor.
/// @param path The path to the file to open, or NULL if no file is to be opened.
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
    init_pair(MENU_SCHEME, 15, 127);
    init_pair(DEFAULT_SCHEME, 15, 16);
    init_pair(I_DEFAULT_SCHEME, 16, 15);
    init_pair(SELECTED_SCHEME, 2, 16);
    init_pair(ERROR_SCHEME, 196, 16);
    init_pair(I_ERROR_SCHEME, 16, 196);

    title = newwin(TITLE_HEIGHT, cols, 0, 0);
    wbkgd(title, COLOR_PAIR(MENU_SCHEME));

    lineNumbers = newwin(CONTENT_HEIGHT, 2, TITLE_HEIGHT, 0);
    wbkgd(lineNumbers, COLOR_PAIR(DEFAULT_SCHEME));

    editor = newwin(CONTENT_HEIGHT, cols / 2 - 2, TITLE_HEIGHT, 2);
    wbkgd(editor, COLOR_PAIR(DEFAULT_SCHEME));

    separator = newwin(CONTENT_HEIGHT, 1, TITLE_HEIGHT, cols / 2);
    wbkgd(separator, COLOR_PAIR(DEFAULT_SCHEME));

    side = newwin(CONTENT_HEIGHT, (cols - 1) / 2, TITLE_HEIGHT, cols / 2 + 1);
    wbkgd(side, COLOR_PAIR(DEFAULT_SCHEME));

    help = newwin(MENU_HEIGHT, cols, rows - MENU_HEIGHT, 0);
    wbkgd(help, COLOR_PAIR(MENU_SCHEME));

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
        wresize(editor, CONTENT_HEIGHT, cols / 2 - getmaxx(lineNumbers));
        mvwin(editor, TITLE_HEIGHT, getmaxx(lineNumbers));

        werase(side);
        wresize(side, CONTENT_HEIGHT, (cols - 1) / 2);
        mvwin(side, TITLE_HEIGHT, cols / 2 + 1);

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
    asprintf(&buffer[2], "%s", file->path ? basename(file->path) : "untitled.s");
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
        case EDIT:
            updateEdit();
            break;

        case BINARY:
            updateBinary();
            break;

        case DEBUG:
//            Registers_s regs = createRegs();
//            updateDebug(&regs);
            break;
    }

    wclrtobot(side);
    wrefresh(side);

    // Clear all unoccupied space in line number and editor windows.
    wclrtobot(lineNumbers);
    wrefresh(lineNumbers);
    wclrtobot(editor);

    // Move cursor to new position.
    wmove(editor, file->lineNumber - file->windowY, file->cursor);
    wrefresh(editor);
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
