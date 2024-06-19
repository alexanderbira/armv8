///
/// debugSide.h
/// The side panel for the binary representation of the assembly code.
///
/// Created by Lancelot Liu on 19/06/2024.
///

#ifndef EXTENSION_DEBUG_SIDE_H
#define EXTENSION_DEBUG_SIDE_H

#include <inttypes.h>
#include <ncurses.h>
#include <setjmp.h>
#include <stdlib.h>

#include "emulatorDelegate.h"
#include "const.h"
#include "file.h"
#include "ir.h"
#include "line.h"
#include "output.h"
#include "registers.h"
#include "state.h"

extern int rows, cols;

extern WINDOW *side;

extern File *file;

extern EditorMode mode;

extern LineInfo *lineInfo;

extern jmp_buf fatalBuffer;

extern char *fatalError;

void updateDebug(Registers regs);

#endif // EXTENSION_DEBUG_SIDE_H
