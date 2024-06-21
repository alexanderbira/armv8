///
/// binarySide.h
/// The side panel for the binary representation of the assembly code.
///
/// Created by Lancelot Liu on 19/06/2024.
///

#ifndef EXTENSION_BINARY_SIDE_H
#define EXTENSION_BINARY_SIDE_H

#include <ncurses.h>
#include <setjmp.h>
#include <stdlib.h>

#include "assemblerDelegate.h"
#include "const.h"
#include "file.h"
#include "ir.h"
#include "line.h"
#include "state.h"

extern int rows, cols;

extern WINDOW *side;

extern File *file;

extern EditorMode mode;

extern LineInfo *lineInfo;

extern jmp_buf fatalBuffer;

extern char *fatalError;

void updateBinary(void);

#endif // EXTENSION_BINARY_SIDE_H
