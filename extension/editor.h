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
#include "error.h"
#include "file.h"
#include "highlight.h"
#include "line.h"
#include "saveOverlay.h"
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

/// The mode that GRIM is in.
typedef enum {
    EDIT,   ///< Standard editing mode.
    DEBUG,  ///< Read-only, debugging.
    BINARY, ///< Read-only view of compiled binary.
} EditorMode;

/// The status of the editor.
typedef enum {
    READ_ONLY, ///< File is read-only.
    UNSAVED,   ///< File has unsaved changes.
    SAVED      ///< File has no pending changes.
} EditorStatus;

/// The human-readable titles of [EditorMode].
static const char *modes[] = { "EDIT", "DEBUG", "BINARY" };

/// The human-readable titles of [EditorStatus].
static const char *statuses[] = { "READ ONLY", "UNSAVED", "SAVED" };

/// The flag signifying to [error.h] to not exit the program when an error occurs.
bool JUMP_ON_ERROR = true;

/// The jump buffer signifying whether an emulate or assemble operation was correct.
jmp_buf fatalBuffer;

/// The human-readable description of an error, if there was one.
char *fatalError;

int main(int argc, char *argv[]);

/// The status of the line after going through the assembler.
typedef enum {
    /// If the line was successfully assembled.
    ASSEMBLED,

    /// If an error was encountered during the line assembly.
    ERRORED,

    /// If the line didn't need to be assembled (e.g. comments, labels, ...).
    NONE,
} LineStatus;

/// Contains data about the line after it goes through the assembler.
typedef struct {
    /// The status of the line after going through the assembler.
    LineStatus lineStatus;

    /// Data about the line after it goes through the assembler.
    union {
        /// The assembled instruction.
        Instruction instruction;

        /// A string containing the error message.
        char *error;
    } data;
} LineInfo;

#endif // EXTENSION_EDITOR_H
