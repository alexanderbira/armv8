///
/// file.c
/// Utilities for the file data structure.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 13/06/24.
///

#include "file.h"

/// Half the capacity of a [File]'s lines.
/// @param file The [File] to resize.
static void decreaseCapacity(File *file) {
    void *result = realloc(file->lines, (file->capacity / 2) * sizeof(Line *));
    assert(result != NULL);
    file->capacity /= 2;
}

/// Double the capacity of a [File]'s lines.
/// @param file The [File] to resize.
static void increaseCapacity(File *file) {
    void *result = realloc(file->lines, (file->capacity * 2) * sizeof(Line *));
    assert(result != NULL);
    file->capacity *= 2;
}

/// Remove a [Line] from a [File]'s lines at a given index.
/// @param file The [File] from which to remove the [Line].
/// @param index The index of the [Line] to remove.
/// @pre The index must be a positive [int].
/// @pre The index must be smaller than the [File] length.
/// @pre The [File]'s length must be greater than 1.
void removeLineAt(File *file, int index) {
    assert(index >= 0);
    assert(index < file->length);
    assert(file->length > 1);
    Line **lines = file->lines;
    memmove(lines + index, lines + index + 1, (file->length - index) * sizeof(Line *));
    if (file->length - 1 == file->capacity / 2) {
        decreaseCapacity(file);
    }
    file->length--;
}

/// Remove a [Line] from the end of a [File].
/// @param file The [File] from which to remove the [Line].
/// @pre The [File]'s length must be greater than 1.
void removeLine(File *file) {
    removeLineAt(file, file->length - 1);
}

/// Insert a [Line] into a [File]'s lines at a given index.
/// @param file The [File] in which to insert the [Line].
/// @param toInsert The [Line] to insert.
/// @param index The index at which to insert the [Line].
/// @pre The index must be a positive [int].
/// @pre The index must be smaller than the [File] length.
void insertLineAt(File *file, Line *toInsert, int index) {
    assert(index >= 0);
    assert(index < file->length);
    if (file->length == file->capacity) {
        increaseCapacity(file);
    }
    Line **lines = file->lines;
    memmove(lines + index + 1, lines + index, (file->length - index) * sizeof(Line *));
    lines[index] = toInsert;
    file->length++;
}

/// Insert a [Line] into the end of a [File].
/// @param file The [File] in which to insert the [Line].
/// @param toInsert The [Line] to insert.
void insertLine(File *file, Line *toInsert) {
    insertLineAt(file, toInsert, file->length);
}

/// Free a [File].
/// @param file The [File] to free.
void freeFile(File *file) {
    for (int i = 0; i < file->length; i++) {
        freeLine(file->lines[i]);
    }
    free(file);
}

/// Initialise an empty [File].
/// @return A pointer to a new empty [File].
File *initialiseFile() {
    File *file = malloc(sizeof(File));
    assert(file != NULL);

    file->lines = malloc(sizeof(File *) * 1);
    assert(file->lines != NULL);

    file->lines[0] = initialiseLine();
    file->length   = 1;
    file->capacity = 1;

    return file;
}
