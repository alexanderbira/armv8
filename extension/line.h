///
/// line.h
/// Utilities for the [Line] data structure.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 13/06/24.
///

#ifndef EXTENSION_LINE_H
#define EXTENSION_LINE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_LINE_SIZE 8

/// Implementation of a gap buffer, representing a line of text.
typedef struct {
    /// The buffer contents.
    char *buffer;

    /// The total size of the buffer.
    int size;

    /// The start of the gap, zero-indexed.
    int gapStart;

    /// The end of the gap, zero-indexed.
    int gapEnd;
} Line;

Line *initialiseLine(const char *content);

void freeLine(Line *line);

void insertCharAt(Line *line, char toInsert, int index);

void removeCharAt(Line *line, int index);

void insertStrAt(Line *line, const char *toInsert, int index);

void removeStrAt(Line *line, int start, int end);

int lineLength(Line *line);

char *getLine(Line *line);

#endif //EXTENSION_LINE_H
