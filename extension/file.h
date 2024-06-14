///
/// file.h
/// Utilities for the file data structure.
///
/// Created by Billy Highley and Alexander Biraben-Renard on 13/06/24.
///

#ifndef EXTENSION_FILE_H
#define EXTENSION_FILE_H

#include "line.h"

typedef struct {
    Line **lines;
    int    length;
    int    capacity;
} File;

void removeLineAt(File *file, int index);
void removeLine(File *file);
void insertLineAt(File *file, Line *toInsert, int index);
void insertLine(File *file, Line *toInsert);
void freeFile(File *file);
File *initialiseFile();

#endif //EXTENSION_FILE_H
