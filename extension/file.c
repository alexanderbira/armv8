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
    }

    return file;
}

/// Free the allocated memory of a [File].
/// @param file A pointer to the [File] to free.
void freeFile(File *file) {
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
/// @param afterLine The line after which to add the new line.
void addLine(File *file, const char *content, size_t afterLine) {
    assert(afterLine < file->size); // Ensure afterLine is within valid range

    Line *line = initialiseLine(content);

    if (file->size >= file->maxSize) {
        file->maxSize *= 2; // Exponential scaling policy.
        file->lines = (Line **) realloc(file->lines, file->maxSize * sizeof(Line *));
    }

    // Move all lines after `afterLine` one position down
    memmove(file->lines + afterLine + 2, file->lines + afterLine + 1, (file->size - afterLine - 1) * sizeof(Line *));

    // Insert the new line
    file->lines[afterLine + 1] = line;
    file->size++;
}

/// Deletes a [Line] at a given line number from the [File].
/// @param file The [File] to modify.
/// @param lineNumber The line number to delete.
void deleteLine(File *file, size_t lineNumber) {
    assert(lineNumber < file->size);

    freeLine(file->lines[lineNumber]);
    memmove(file->lines + lineNumber, file->lines + lineNumber + 1, (file->size - lineNumber - 1) * sizeof(Line *));

    file->size--;
}

/// Performs some action of [file] given a special [ControlKey].
/// @param file The [File] to modify.
/// @param key The [ControlKey] that was pressed.
void handleAction(File *file, ControlKey key) {
    switch (key) {
        case ARROW_UP:
            if (file->lineNumber == 0) return;

        case ARROW_DOWN:
            if (file->lineNumber + 1 == file->size) return;

            file->lineNumber += (key == ARROW_UP) ? -1 : 1;
            if (file->cursor >= file->lines[file->lineNumber]->size) {
                file->cursor = file->lines[file->lineNumber]->size - 1;
            }
            break;

        case ARROW_LEFT:
            if (file->cursor == 0) return;

        case ARROW_RIGHT:
            if (file->cursor >= file->lines[file->lineNumber]->size) return;

            file->cursor += (key == ARROW_LEFT) ? -1 : 1;
            break;

        case RETURN: {
            // Save the contents of the current one, and remove after cursor.
            Line *currentLine = file->lines[file->lineNumber];
            char *currentText = getLine(currentLine);
            removeStrAt(currentLine, file->cursor, lineLength(currentLine));

            // Reset cursor to 0, and copy over text from previous line to a new line immediately after.
            addLine(file, currentText + file->cursor, file->lineNumber++);
            file->cursor = 0;
            free(currentText);
        }

        case DELETE:
            if (file->cursor > 0) {
                // Remove the character at the cursor position
                removeStrAt(file->lines[file->lineNumber], file->cursor - 1, file->cursor);
                file->cursor--;
            } else if (file->cursor == 0 && file->lineNumber > 0) {
                // Current line will be removed, but we copy over text.
                Line *currentLine = file->lines[file->lineNumber];
                Line *previousLine = file->lines[file->lineNumber - 1];
                size_t previousLineLength = lineLength(previousLine);

                if (lineLength(currentLine) != 0) {
                    // Copy text to previous line if any exist.
                    char *currentText = getLine(currentLine);
                    insertStrAt(previousLine, currentText, previousLine->gapStart);
                    free(currentText);
                }

                deleteLine(file, file->lineNumber);
                file->lineNumber--;
                file->cursor = previousLineLength;
            }
            break;
    }
}
