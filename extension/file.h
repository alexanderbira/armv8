///
/// file.h
/// Utilities for the [File] data structure.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 13/06/24.
///

#ifndef EXTENSION_FILE_H
#define EXTENSION_FILE_H

#include <ctype.h>
#include <ncurses.h>

#include "line.h"

#define INITIAL_FILE_SIZE 8

/// Overall representation of a text file.
typedef struct {
    /// Path to the file.
    char *path;

    /// The individual lines of text.
    Line **lines;

    /// The number of lines.
    int size;

    /// The total capacity of [lines].
    int maxSize;

    /// The current line of the cursor, zero-indexed.
    int lineNumber;

    /// The current cursor position within the line, zero-indexed.
    int cursor;

    /// The top-left corner of the window currently being rendered.
    int windowX, windowY;
} File;

typedef void (*LineCallback)(Line *line, int index);

File *initialiseFile(const char *path);

void freeFile(File *file);

void addLine(File *file, const char *content, int afterLine);

void deleteLine(File *file, int lineNumber);

void iterateLines(File *file, LineCallback callback);

void iterateLinesInWindow(File *file, LineCallback callback);

bool handleFileAction(File *file, int key);

bool saveFile(File *file);

#endif // EXTENSION_FILE_H
