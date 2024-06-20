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

    // Has the debug mode terminated execution?
    bool finishedExecuting = false;

    // The memory for debug mode.
    Memory debugMemory;

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

                wmove(editor, file->lineNumber, file->cursor);

                // Free the emulator and assembler states.
                freeMem(memory);
                destroyState(state);

                justRan = true;
            }

                break;

            case DEBUG_KEY:
                finishedExecuting = false;

                // Toggle debug mode.
                if (mode == DEBUG) {
                    // If manually exiting debug, terminate the execution.
                    mode = EDIT;
                    status = UNSAVED;
                    freeMem(debugMemory);
                    free(addrLines);
                    clearLastRegs();
                    break;
                } else {
                    mode = DEBUG;
                    status = READ_ONLY;
                }

                // Initialise the addrLines array.
                addrLines = malloc(file->size * sizeof(AddrLine));

                // The index of the current instruction
                int instructionIndex = 0;

                // Initialise registers, memory, and assembler state.
                debugRegistersStruct = createRegs();
                debugMemory = allocMem();
                AssemblerState state = createState();

                // Initialise error string.
                fatalError[0] = '\0';

                if (!setjmp(fatalBuffer)) {

                    // Perform the first assembly pass.
                    for (int currentLine = 0; currentLine < file->size; currentLine++) {
                        size_t irCount = state.irCount;

                        parse(getLine(file->lines[currentLine]), &state);

                        if (irCount != state.irCount) {
                            // Add a new addrLine if the line compiles.
                            // This will remember the association between the line number
                            // and the memory address of its compiled instruction.
                            AddrLine addrLine;
                            addrLine.line = currentLine;
                            addrLine.address = instructionIndex * 4;
                            addrLines[instructionIndex] = addrLine;

                            instructionIndex++;
                        }
                    }

                    state.address = 0x0;

                    // Perform the second pass and write the instruction to memory.
                    for (size_t i = 0; i < state.irCount; i++) {
                        IR ir = state.irList[i];
                        Instruction instruction = getTranslator(&ir.type)(&ir, &state);
                        writeMem(debugMemory, false, state.address, instruction);
                        state.address += 0x4;
                    }

                    destroyState(state);

                    pcValue = 0x0;
                } else {
                    // Fatal error encountered during execution

                    // Update the side window.
                    updateDebug(&debugRegistersStruct);

                    // Free the memory
                    freeMem(debugMemory);
                    free(addrLines);

                    finishedExecuting = true;
                }

                break;

            case BINARY_KEY:
                // Toggle binary mode.
                mode = (mode == BINARY) ? EDIT : BINARY;
                break;

            default:
                if (mode == DEBUG && key == '\n') {
                    if (finishedExecuting) {
                        // If the program finished execution because of a fatal error.
                        mode = EDIT;
                        status = UNSAVED;
                        clearLastRegs();
                        break;
                    }
                    // Run the current instruction.

                    // Fetch instruction.
                    Instruction instruction = readMem(debugMemory, false, getRegPC(&debugRegistersStruct));

                    // Go back to edit mode if the instruction was a halt.
                    if (instruction == HALT) {
                        mode = EDIT;
                        status = UNSAVED;
                        freeMem(debugMemory);
                        free(addrLines);
                        clearLastRegs();
                        break;
                    }

                    // Execute the instruction.
                    execute(&instruction, &debugRegistersStruct, debugMemory);
                    finishedExecuting = false;

                    pcValue = getRegPC(&debugRegistersStruct);

                    // Scroll to the line now being executed.
                    for (int addrLineIndex = 0; addrLineIndex < file->size; addrLineIndex++) {
                        AddrLine currAddrLine = addrLines[addrLineIndex];
                        if (currAddrLine.address == pcValue) {
                            file->lineNumber = currAddrLine.line;
                            break;
                        }
                    }

                    break;
                }

                if (status == READ_ONLY) break;

                status = handleFileAction(file, key) ? UNSAVED : status;
                break;
        }
    }

    // Automatically save the file if it is unsaved.
    status = (file->path)
        ? (saveFile(file) ? SAVED : status)
        : (showSaveOverlay(file) ? SAVED : status);

    // Cleanup
    freeFile(file);
    free(fatalError);
    endwin();

    return 0;
}

/// Wrapper around [rerenderLine] where the line is always presumed to be correct.
/// @param line The line to rerender.
/// @param index The index of the line in the window.
static void rerenderLineWrapper(Line *line, int index) {
    bool currentDebugLine = false;
    // Determine if the current line is the one being debugged.
    for (int addrLineIndex = 0; addrLineIndex < file->size; addrLineIndex++) {
        AddrLine currAddrLine = addrLines[addrLineIndex];
        if (currAddrLine.line == (size_t) index && currAddrLine.address == pcValue) {
            currentDebugLine = true;
            break;
        }
        if (currAddrLine.line > (size_t) index) break;
    }
    rerenderLine(line, index, false, currentDebugLine);
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
    status = file->path ? SAVED : UNSAVED;

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
            // Print out all lines in current window.
            iterateLinesInWindow(file, &rerenderLineWrapper);

            // Update the side window.
            updateDebug(&debugRegistersStruct);
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
