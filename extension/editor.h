///
/// editor.h
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#ifndef EXTENSION_EDITOR_H
#define EXTENSION_EDITOR_H

#include <libgen.h>
#include <ncurses.h>
#include <setjmp.h>
#include <stdio.h>

#include "assemblerDelegate.h"
#include "const.h"
#include "emulatorDelegate.h"
#include "error.h"
#include "file.h"
#include "highlight.h"
#include "line.h"
#include "saveOverlay.h"
#include "side.h"
#include "state.h"
#include "termSizeOverlay.h"

/// The key-code for CTRL plus some other key.
#define CTRL(__KEY__) ((__KEY__) & 0x1F)

/// The key code to quit GRIM.
#define QUIT_KEY          CTRL('q')

/// The key code to save the current file.
#define SAVE_KEY          CTRL('s')

/// The key code to run the assembly file.
#define RUN_KEY           CTRL('r')

/// The key code to enter debug mode.
#define DEBUG_KEY         CTRL('d')

/// The key code to view the compiled assembly.
#define BINARY_KEY        CTRL('b')

/// The height (in characters) of the program title.
#define TITLE_HEIGHT      1

/// The height (in characters) of the program help menu.
#define MENU_HEIGHT       1

/// The height (in characters) of the main content.
#define CONTENT_HEIGHT    ((int) rows - TITLE_HEIGHT - MENU_HEIGHT)

static const char *commands[6] = {
    "[^Q] - QUIT",
    "[^S] - SAVE",
    "[^R] - RUN",
    "[^D] - DEBUG",
    "[^B] - BINARY",
    "[ENTER] - STEP",
};

int rows = 0, cols = 0;

WINDOW *title, *lineNumbers, *editor, *help, *separator, *side;

File *file;

/// The current editor mode.
EditorMode mode;

/// The current editor status.
EditorStatus status;

/// Info about the lines after attempt to assemble.
LineInfo *lineInfo;

/// The flag signifying to [error.h] to not exit the program when an error occurs.
bool JUMP_ON_ERROR = true;

/// The jump buffer signifying whether an emulate or assemble operation was correct.
jmp_buf fatalBuffer;

/// The human-readable description of an error, if there was one.
char *fatalError;

int main(int argc, char *argv[]);

#endif // EXTENSION_EDITOR_H
