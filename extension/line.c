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

/// Remove a substring from a [Line]'s contents between [start] and [end].
/// @param line The [Line] from which to remove the string.
/// @param start The starting index of the substring to remove.
/// @param end The ending index of the substring to remove.
void removeStrAt(Line *line, size_t start, size_t end) {
    assert(start <= end);
    assert(end <= (line->size - (line->gapEnd - line->gapStart)));

    // Move gap to the start position
    moveGap(line, start);

    // Increase the gap size by moving the gap end backward
    line->gapEnd -= (end - start);
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
    char *result = malloc(line->size + 1);
    strncpy(result, line->buffer, line->gapStart);
    strncpy(result + line->gapStart, line->buffer + line->gapEnd, line->size - line->gapEnd);
    result[line->size] = '\0';
    return result;
}
