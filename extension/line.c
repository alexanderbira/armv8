///
/// line.c
/// Utilities for the line data structure.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 13/06/24.
///

#include "line.h"

/// Initialise an empty [Line].
/// @param content If not [NULL], initialise this [Line] with [content].
/// @return A pointer to a new empty [Line].
Line *initialiseLine(const char *content) {
    Line *line = (Line *) malloc(sizeof(Line));

    line->size = INITIAL_LINE_SIZE;
    if (content != NULL) {
        line->size = (strlen(content) > line->size)
                     ? strlen(content)
                     : line->size;
    }

    line->buffer = (char *) malloc(line->size);
    if (content != NULL) {
        memcpy(line->buffer, content, line->size);
    }

    line->gapStart = (content == NULL) ? 0 : line->size;
    line->gapEnd = line->size;

    return line;
}

/// Free the allocated memory of a [Line].
/// @param line A pointer to the [Line] to free.
void freeLine(Line *line) {
    free(line->buffer);
    free(line);
}

/// Resizes [Line], the gap buffer, to [newSize].
/// @param line The [Line] to be resized.
/// @param newSize The new size desired.
static void resizeLine(Line *line, size_t newSize) {
    char *newBuffer = (char *) malloc(newSize);

    memcpy(newBuffer, line->buffer, line->gapStart);
    memcpy(
        newBuffer + newSize - (line->size - line->gapEnd),
        line->buffer + line->gapEnd,
        line->size - line->gapEnd
    );

    free(line->buffer);
    line->buffer = newBuffer;
    line->gapEnd = newSize - (line->size - line->gapEnd);
    line->size = newSize;
}

/// Moves the gap in a [Line], the gap buffer, to [index].
/// @param line The [Line] for which the gap should be moved.
/// @param index The position the gap should be moved to.
static void moveGap(Line *line, size_t index) {
    if (index == line->gapStart) return;

    if (index < line->gapStart) {
        size_t moveSize = line->gapStart - index;
        memmove(
            line->buffer + line->gapEnd - moveSize,
            line->buffer + index, moveSize
        );
    } else {
        size_t moveSize = index - line->gapStart;
        memmove(
            line->buffer + line->gapStart,
            line->buffer + line->gapEnd, moveSize
        );
    }

    line->gapEnd -= line->gapStart - index;
    line->gapStart = index;
}

/// Insert a [char] into a [Line]'s contents at a given index.
/// @param line The [Line] in which to insert the [char].
/// @param toInsert The [char] to insert.
/// @param index The index at which to insert the [char].
/// @pre The index must be smaller than the [Line] length.
void insertCharAt(Line *line, char toInsert, size_t index) {
    assert(index <= line->size);

    // If we have run out of space, double size.
    if (line->gapStart == line->gapEnd) {
        resizeLine(line, line->size * 2);
    }

    moveGap(line, index);
    line->buffer[line->gapStart++] = toInsert;
}

/// Remove a [char] from a [Line]'s contents at a given index.
/// @param line The [Line] from which to remove the [char].
/// @param index The index of the [char] to remove.
/// @pre The index must be smaller than the [Line] length.
void removeCharAt(Line *line, size_t index) {
    assert(index < line->size);

    if (index < line->gapStart) {
        memmove(line->buffer + index, line->buffer + index + 1, line->gapStart - index - 1);
        line->gapStart--;
    } else if (index >= line->gapEnd) {
        memmove(line->buffer + index, line->buffer + index + 1, line->size - index - 1);
        line->gapEnd++;
    }
}

/// Insert a string into a [Line]'s contents at a given index.
/// @param line The [Line] in which to insert the string.
/// @param toInsert The string to insert.
/// @param index The index at which to insert the string.
/// @pre The index must be a positive [int].
/// @pre The index must be smaller than the [Line] length.
void insertStrAt(Line *line, const char *toInsert, size_t index) {
    assert(index >= 0);
    assert(index <= (line->size - (line->gapEnd - line->gapStart)));

    size_t length = strlen(toInsert);

    // Resize buffer if needed
    while (line->gapEnd - line->gapStart < length) {
        resizeLine(line, line->size * 2);
    }

    // Move gap to the insert position
    moveGap(line, index);

    // Copy the string into the gap
    strncpy(line->buffer + line->gapStart, toInsert, length);
    line->gapStart += length;
}

void printLine(Line *line) {
    for (size_t i = 0; i < line->gapStart; ++i) {
        putchar(line->buffer[i]);
    }
    for (size_t i = line->gapEnd; i < line->size; ++i) {
        putchar(line->buffer[i]);
    }
    putchar('\n');
}
