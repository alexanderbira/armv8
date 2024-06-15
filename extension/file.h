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
    size_t size;

    /// The total capacity of [lines].
    size_t maxSize;

    /// The current line of the cursor, zero-indexed.
    size_t lineNumber;

    /// The current cursor position within the line, zero-indexed.
    size_t cursor;

    /// The top-left corner of the window currently being rendered.
    size_t windowX, windowY;
} File;

typedef void (*LineCallback)(Line line);

File *initialiseFile(const char *path);

void freeFile(File *file);

void addLine(File *file, const char *content, size_t afterLine);

void deleteLine(File *file, size_t lineNumber);

void iterateLines(File *file, LineCallback callback);

void handleFileAction(File *file, int key);

#endif // EXTENSION_FILE_H
