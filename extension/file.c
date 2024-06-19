///
/// file.c
/// Utilities for the [File] data structure.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 13/06/24.
///

#include "file.h"

/// Creates a new [File]. If [path] is not NULL, copies in its contents.
/// @param path The path of the file to read in. Leave [NULL] if not present.
/// @returns A pointer to the new [File].
File *initialiseFile(const char *path) {
    File *file = (File *) malloc(sizeof(File));

    file->size = 0;
    file->maxSize = INITIAL_FILE_SIZE;
    file->lineNumber = 0;
    file->cursor = 0;
    file->windowX = 0;
    file->windowY = 0;

    file->lines = (Line **) malloc(INITIAL_FILE_SIZE * sizeof(Line *));
    file->path = path ? strdup(path) : NULL;

    if (file->path != NULL) {
        FILE *handler = fopen(file->path, "r");
        assert(handler != NULL);

        char *text = NULL;
        size_t textLength = 0;
        ssize_t read;

        while ((read = getline(&text, &textLength, handler)) != -1) {
            if (text[read - 1] == '\n') text[--read] = '\0';
            addLine(file, text, file->size - 1);
        }

        free(text);
        fclose(handler);
    } else {
        // Add a blank line to start.
        addLine(file, NULL, 0);
    }

    return file;
}

/// Free the allocated memory of a [File].
/// @param file A pointer to the [File] to free.
void freeFile(File *file) {
    if (!file) return;
    free(file->path);
    for (int i = 0; i < file->size; i++) {
        freeLine(file->lines[i]);
    }
    free(file->lines);
    free(file);
}

/// Adds a new [Line] containing [content] after line number [afterLine].
/// @param file The [File] to modify.
/// @param content The text to add.
/// @param atLine The line at which to add the new line.
void addLine(File *file, const char *content, int afterLine) {
    assert(afterLine <= file->size);

    Line *newLine = initialiseLine(content);

    if (file->size >= file->maxSize) {
        file->maxSize *= 2; // Exponential scaling policy.
        file->lines = (Line **) realloc(file->lines, file->maxSize * sizeof(Line *));
        assert(file->lines != NULL);
    }

    if (afterLine == file->size) {
        file->lines[file->size++] = newLine;
    } else {
        memmove(&file->lines[afterLine + 2], &file->lines[afterLine + 1],
                (file->size - afterLine - 1) * sizeof(Line *));
        file->lines[afterLine + 1] = newLine;
        file->size++;
    }
}

/// Deletes a [Line] at a given line number from the [File].
/// @param file The [File] to modify.
/// @param lineNumber The line number to delete.
void deleteLine(File *file, int lineNumber) {
    assert(lineNumber < file->size);

    freeLine(file->lines[lineNumber]);
    memmove(&file->lines[lineNumber], &file->lines[lineNumber + 1], (file->size - lineNumber - 1) * sizeof(Line *));

    file->size--;
}

/// Executes [callback] on every line in the [File].
/// @param file The [File] to process.
/// @param callback The [LineCallback] to execute on each line.
void iterateLines(File *file, LineCallback callback) {
    for (int i = 0; i < file->size; ++i) {
        callback(file->lines[i], i);
    }
}

/// Executes [callback] on all lines in the [File] which are in the window.
/// @param file The [File] to process.
/// @param callback The [LineCallback] to execute on the lines.
void iterateLinesInWindow(File *file, LineCallback callback) {
    for (int i = (int) file->windowY; i < (int) file->size; i++) {
        callback(file->lines[i], i);
    }
}

/// Performs some action of [file] given some [key].
/// @param file The [File] to modify.
/// @param key The key code in question.
/// @returns Whether a modification was made to the file.
bool handleFileAction(File *file, int key) {
    switch (key) {
        case KEY_UP:
            if (file->lineNumber == 0) return false;
            file->lineNumber--;
            if (file->cursor >= lineLength(file->lines[file->lineNumber])) {
                file->cursor = lineLength(file->lines[file->lineNumber]);
            }
            break;

        case KEY_DOWN:
            // We allow users to create a new line by pressing
            // the down arrow on the last line of the file.
            if (file->lineNumber + 1 == file->size) {
                // But if they are already on a trailing empty line, return.
                if (!lineLength(file->lines[file->lineNumber])) return false;

                addLine(file, NULL, file->lineNumber++);
                file->cursor = 0;
                return true;
            }

            // Otherwise, go to the next line and coerce cursor x location.
            file->lineNumber++;
            if (file->cursor >= lineLength(file->lines[file->lineNumber])) {
                file->cursor = lineLength(file->lines[file->lineNumber]);
            }

            break;

        case KEY_LEFT:
            if (file->cursor == 0) {
                // Go to end of previous line if it exists.
                if (file->lineNumber == 0) return false;

                file->lineNumber--;
                file->cursor = lineLength(file->lines[file->lineNumber]);
            } else {
                file->cursor--;
            }
            break;

        case KEY_RIGHT:
            if (file->cursor >= lineLength(file->lines[file->lineNumber])) {
                // Go to end of previous line if it exists.
                if (file->lineNumber + 1 == file->size) return false;

                file->lineNumber++;
                file->cursor = 0;
            } else {
                file->cursor++;
            }
            break;

        case '\n': {
            // Save the contents of the current one, and remove after cursor.
            Line *currentLine = file->lines[file->lineNumber];
            char *currentText = getLine(currentLine);
            removeStrAt(currentLine, file->cursor, lineLength(currentLine));

            // Reset cursor to 0, and copy over text from previous line to a new line immediately after.
            addLine(file, currentText + file->cursor, file->lineNumber++);
            file->cursor = 0;
            free(currentText);
            return true;
        }

        case KEY_BACKSPACE:
        case 127: // [DEL] key.
            if (file->cursor > 0) {
                // Remove the character at the cursor position
                removeStrAt(file->lines[file->lineNumber], file->cursor - 1, file->cursor);
                file->cursor--;
                return true;
            } else if (file->cursor == 0 && file->lineNumber > 0) {
                // Current line will be removed, but we copy over text.
                Line *currentLine = file->lines[file->lineNumber];
                Line *previousLine = file->lines[file->lineNumber - 1];
                int previousLineLength = lineLength(previousLine);

                if (lineLength(currentLine) != 0) {
                    // Copy text to previous line if any exist.
                    char *currentText = getLine(currentLine);
                    insertStrAt(previousLine, currentText, previousLine->gapStart);
                    free(currentText);
                }

                deleteLine(file, file->lineNumber);
                file->lineNumber--;
                file->cursor = previousLineLength;
                return true;
            }
            return false;

        case '\t':
            insertStrAt(file->lines[file->lineNumber], "  ", file->cursor);
            file->cursor += 2;
            return true;

        default:
            if (!isprint(key)) return false;
            insertCharAt(file->lines[file->lineNumber], key, file->cursor);
            file->cursor++;
            return true;
    }

    return false;
}

bool saveFile(File *file) {
    FILE *f = fopen(file->path, "w");
    if (f == NULL) return false;

    for (int i = 0; i < file->size; ++i) {
        char *lineText = getLine(file->lines[i]);
        fprintf(f, "%s\n", lineText);
    }

    fclose(f);
    return true;
}

/// Updates the contents of one line, with corresponding line number.
/// @param line The [Line] to be updated on screen.
/// @param index The 0-based index of the line to update.
void rerenderLine(Line *line, int index, bool errored) {
    // Don't render line below screen.
    if (index >= file->windowY + CONTENT_HEIGHT) return;

    // Print the line number, then clear rest of line.
    int padding = getmaxx(lineNumbers) - countDigits(index + 1) - 1;

    // Reset the cursors.
    wmove(editor, index - file->windowY, 0);
    wmove(lineNumbers, index - file->windowY, 0);
    
    wclrtoeol(lineNumbers);

    if (!errored || file->lineNumber == index) {
        // Display editor line with syntax highlighting.
        if (file->lineNumber == index) {
            wattron(lineNumbers, COLOR_PAIR(SELECTED_SCHEME));
            mvwprintw(lineNumbers, index - file->windowY, padding, "%d", index + 1);
            wattroff(lineNumbers, COLOR_PAIR(SELECTED_SCHEME));
        } else {
            mvwprintw(lineNumbers, index - file->windowY, padding, "%d", index + 1);
        }

        wPrintLine(editor, getLine(line));
    } else {
        // Print the line number in red.
        wattron(lineNumbers, COLOR_PAIR(ERROR_SCHEME));
        mvwprintw(lineNumbers, index - file->windowY, padding, "%d", index + 1);
        wattroff(lineNumbers, COLOR_PAIR(ERROR_SCHEME));

        // Display editor line in red.
        wattron(editor, COLOR_PAIR(ERROR_SCHEME));
        wprintw(editor, "%s", getLine(line));
        wattroff(editor, COLOR_PAIR(ERROR_SCHEME));
    }

    wclrtoeol(editor);
    wclrtoeol(lineNumbers);
}
