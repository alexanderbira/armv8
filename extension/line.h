///
/// line.h
/// Utilities for the line data structure.
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

/// Implementation of a gap buffer. In GRim, this is used as a line buffer.
typedef struct {
    /// The buffer contents.
    char *buffer;

    /// The total size of the buffer.
    size_t size;

    /// The start of the gap, zero-indexed.
    size_t gapStart;

    /// The end of the gap, zero-indexed.
    size_t gapEnd;
} Line;

Line *initialiseLine(const char *content);

void freeLine(Line *line);

void insertCharAt(Line *line, char toInsert, size_t index);

void removeCharAt(Line *line, size_t index);

void insertStrAt(Line *line, const char *toInsert, size_t index);

void printLine(Line *line);

#endif //EXTENSION_LINE_H
