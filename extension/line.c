///
/// line.c
/// Utilities for the [Line] data structure.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 13/06/24.
///

#include "line.h"

/// Initialise an empty [Line].
/// @param content If not [NULL], initialise this [Line] with [content].
/// @returns A pointer to a new empty [Line].
Line *initialiseLine(const char *content) {
    Line *line = (Line *) malloc(sizeof(Line));
    assert(line != NULL);

    size_t contentLength = content ? strlen(content) : 0;
    size_t bufferSize = (contentLength > INITIAL_LINE_SIZE) ? contentLength * 2 : INITIAL_LINE_SIZE;

    line->buffer = (char *) malloc(bufferSize);
    assert(line->buffer != NULL);

    line->size = bufferSize;
    line->gapStart = contentLength;
    line->gapEnd = bufferSize;

    if (contentLength > 0) {
        memcpy(line->buffer, content, contentLength);
    }

    return line;
}

/// Free the allocated memory of a [Line].
/// @param line A pointer to the [Line] to free.
void freeLine(Line *line) {
    if (!line) return;
    free(line->buffer);
    free(line);
}

/// Resizes [Line], the gap buffer, to [newSize].
/// @param line The [Line] to be resized.
/// @param newSize The new size desired.
static void resizeLine(Line *line, size_t newSize) {
    char *newBuffer = (char *) malloc(newSize);
    assert(newBuffer != NULL);

    size_t newGapEnd = newSize - (line->size - line->gapEnd);

    memcpy(newBuffer, line->buffer, line->gapStart);
    memcpy(newBuffer + newGapEnd, line->buffer + line->gapEnd, line->size - line->gapEnd);

    free(line->buffer);
    line->buffer = newBuffer;
    line->gapEnd = newGapEnd;
    line->size = newSize;
}

/// Moves the gap in a [Line], the gap buffer, to [index].
/// @param line The [Line] for which the gap should be moved.
/// @param index The position the gap should be moved to.
static void moveGap(Line *line, size_t index) {
    if (index < line->gapStart) {
        memmove(line->buffer + line->gapEnd - (line->gapStart - index), line->buffer + index, line->gapStart - index);
        line->gapEnd -= (line->gapStart - index);
        line->gapStart = index;
    } else if (index > line->gapStart) {
        memmove(line->buffer + line->gapStart, line->buffer + line->gapEnd, index - line->gapStart);
        line->gapEnd += (index - line->gapStart);
        line->gapStart = index;
    }
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

    moveGap(line, index);
    line->gapEnd++;
}

/// Insert a string into a [Line]'s contents at a given index.
/// @param line The [Line] in which to insert the string.
/// @param toInsert The string to insert.
/// @param index The index at which to insert the string.
/// @pre The index must be smaller than the [Line] length.
void insertStrAt(Line *line, const char *toInsert, size_t index) {
    assert(index >= 0);
    assert(index <= (line->size - (line->gapEnd - line->gapStart)));

    size_t insertLength = strlen(toInsert);
    while (line->gapEnd - line->gapStart < insertLength) {
        resizeLine(line, line->size * 2);
    }

    moveGap(line, index);
    memcpy(line->buffer + line->gapStart, toInsert, insertLength);
    line->gapStart += insertLength;
}

/// Remove a substring from a [Line]'s contents between [start] and [end].
/// @param line The [Line] from which to remove the string.
/// @param start The starting index of the substring to remove.
/// @param end The ending index of the substring to remove.
void removeStrAt(Line *line, size_t start, size_t end) {
    assert(start <= end);
    assert(end <= (line->size - (line->gapEnd - line->gapStart)));

    moveGap(line, start);
    line->gapEnd += (end - start);
}

/// Calculates the length of the given [Line].
/// @param line The [Line] to calculate over.
/// @returns The length of the text represented by the [Line].
size_t lineLength(Line *line) {
    return line->size - (line->gapEnd - line->gapStart);
}

/// Converts a [Line] object to text. (I.e., remove the gap.)
/// @param line The [Line] to prettify.
/// @returns The text represented by the [Line].
char *getLine(Line *line) {
    size_t len = lineLength(line);
    char *result = (char *) malloc(len + 1);
    assert(result != NULL);

    memcpy(result, line->buffer, line->gapStart);
    memcpy(result + line->gapStart, line->buffer + line->gapEnd, line->size - line->gapEnd);
    result[len] = '\0';

    return result;
}
