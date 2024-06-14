///
/// line.c
/// Utilities for the line data structure.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 13/06/24.
///

#include "line.h"

/// Half the capacity of a [Line]'s content.
/// @param line The [Line] to resize.
static void decreaseCapacity(Line *line) {
    void *result = realloc(line->content, (line->capacity / 2) * sizeof(char));
    assert(result != NULL);
    line->capacity /= 2;
}

/// Double the capacity of a [Line]'s content.
/// @param line The [Line] to resize.
static void increaseCapacity(Line *line) {
    void *result = realloc(line->content, (line->capacity * 2) * sizeof(char));
    assert(result != NULL);
    line->capacity *= 2;
}

/// Remove a [char] from a [Line]'s contents at a given index.
/// @param line The [Line] from which to remove the [char].
/// @param index The index of the [char] to remove.
/// @pre The index must be a positive [int].
/// @pre The index must be smaller than the [Line] length.
/// @pre The [Line]'s length must be greater than 1.
void removeCharAt(Line *line, int index) {
    assert(index >= 0);
    assert(index < line->length);
    assert(line->length > 1);
    char *content = line->content;
    memmove(content + index, content + index + 1, line->length - index);
    if (line->length - 1 == line->capacity / 2) {
        decreaseCapacity(line);
    }
    line->length--;
}

/// Remove a [char] from the end of a [Line].
/// @param line The [Line] from which to remove the [char].
/// @pre The [Line]'s length must be greater than 1.
void removeChar(Line *line) {
    removeCharAt(line, line->length - 2);
}

/// Insert a [char] into a [Line]'s contents at a given index.
/// @param line The [Line] in which to insert the [char].
/// @param toInsert The [char] to insert.
/// @param index The index at which to insert the [char].
/// @pre The index must be a positive [int].
/// @pre The index must be smaller than the [Line] length.
void insertCharAt(Line *line, char toInsert, int index) {
    assert(index >= 0);
    assert(index < line->length);
    if (line->length == line->capacity) {
        increaseCapacity(line);
    }
    char *content = line->content;
    memmove(content + index + 1, content + index, line->length - index);
    content[index] = toInsert;
    line->length++;
}

/// Insert a [char] into the end of a [Line].
/// @param line The [Line] in which to insert the [char].
/// @param toInsert The [char] to insert.
void insertChar(Line *line, char toInsert) {
    insertCharAt(line, toInsert, line->length - 1);
}

/// Insert a string into a [Line]'s contents at a given index.
/// @param line The [Line] in which to insert the string.
/// @param toInsert The string to insert.
/// @param index The index at which to insert the string.
/// @pre The index must be a positive [int].
/// @pre The index must be smaller than the [Line] length.
void insertStrAt(Line *line, char *toInsert, int index) {
    assert(index >= 0);
    assert(index < line->length);
    size_t length = strlen(toInsert);
    while (line->length + length >= line->capacity) {
        increaseCapacity(line);
    }
    char *content = line->content;
    memmove(content + index + length, content + index, line->length - index);
    strncpy(content + index, toInsert, length);
    line->length += (int) length;
}

/// Insert a string into the end of a [Line].
/// @param line The [Line] in which to insert the string.
/// @param toInsert The string to insert.
void insertStr(Line *line, char *toInsert) {
    insertStrAt(line, toInsert, line->length - 1);
}

/// Free a [Line].
/// @param line The [Line] to free.
void freeLine(Line *line) {
    free(line->content);
    free(line);
}

/// Initialise an empty [Line].
/// @return A pointer to a new empty [Line].
Line *initialiseLine() {
    Line *line = malloc(sizeof(Line));
    assert(line != NULL);
    line->content = malloc(sizeof(char) * 1);
    assert(line->content != NULL);
    line->content[0] = '\0';
    line->length     = 1;
    line->capacity   = 1;
    return line;
}
