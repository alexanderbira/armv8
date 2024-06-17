///
/// editor.h
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#ifndef EXTENSION_EDITOR_H
#define EXTENSION_EDITOR_H

#include <ncurses.h>

#include "const.h"
#include "file.h"
#include "highlight.h"
#include "line.h"

/// The key-code for CTRL plus some other key.
#define CTRL(__KEY__) ((__KEY__) & 0x1F)

/// The key code to quit GRIM.
#define QUIT         CTRL('q')

/// The key code to save the current file.
#define SAVE         CTRL('s')

/// The key code to run the assembly file.
#define RUN          CTRL('r')

/// The height (in characters) of the program title.
#define TITLE_HEIGHT   1

/// The height (in characters) of the program help menu.
#define MENU_HEIGHT 1

/// The height (in characters) of the main content.
#define CONTENT_HEIGHT ((int) rows - TITLE_HEIGHT - MENU_HEIGHT)

/// The mode that GRIM is in.
typedef enum {
    EDIT,   ///< Standard editing mode.
    DEBUG,  ///< Read-only, debugging.
    BINARY, ///< Read-only view of compiled binary.
} EditorMode;

/// The human-readable titles of [Mode].
static const char *modes[] = { "EDIT", "DEBUG", "BINARY" };

int main(int argc, char *argv[]);

#endif // EXTENSION_EDITOR_H
