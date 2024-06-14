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

typedef struct {
    char *content;
    int   length;
    int   capacity;
} Line;

void  removeCharAt(Line *line, int index);
void  removeChar(Line *line);
void  insertCharAt(Line *line, char toInsert, int index);
void  insertChar(Line *line, char toInsert);
void  insertStrAt(Line *line, char *toInsert, int index);
void  insertStr(Line *line, char *toInsert);
void  freeLine(Line *line);
Line *initialiseLine();

#endif //EXTENSION_LINE_H
