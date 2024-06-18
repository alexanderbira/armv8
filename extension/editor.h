///
/// editor.h
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#ifndef EXTENSION_EDITOR_H
#define EXTENSION_EDITOR_H

#include <ncurses.h>
#include <setjmp.h>
#include <stdio.h>

#include "const.h"
#include "file.h"
#include "highlight.h"
#include "line.h"
#include "error.h"
#include "handleAssembly.h"
#include "state.h"
// #include "emulate.h"

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

static const char *commands[5] = {
    "[^Q] - QUIT_KEY",
    "[^S] - SAVE_KEY",
    "[^R] - RUN_KEY",
    "[^D] - DEBUG_KEY",
    "[^B] - BINARY_KEY",
};

//static const char *debugCommands[] = {
//    "[ENTER] - STEP",
//    "[^D] - EDIT"
//    "[^B] - BREAKPOINT"
//};
//
//static const char *binaryCommands[] = {
//    "[^B] - EDIT"
//};

/// The mode that GRIM is in.
typedef enum {
    EDIT,   ///< Standard editing mode.
    DEBUG,  ///< Read-only, debugging.
    BINARY, ///< Read-only view of compiled binary.
} EditorMode;

/// The human-readable titles of [Mode].
static const char *modes[] = { "EDIT", "DEBUG_KEY", "BINARY_KEY" };

bool JUMP_ON_ERROR = true;
jmp_buf fatalBuffer;
char *fatalError;

int main(int argc, char *argv[]);

#endif // EXTENSION_EDITOR_H
