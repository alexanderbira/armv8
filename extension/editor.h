///
/// editor.h
/// Instantiates an editor (terminal-based) interface.
///
/// Created by Billy Highley on 12/06/24.
///

#ifndef EXTENSION_EDITOR_H
#define EXTENSION_EDITOR_H

#include <math.h>
#include <ncurses.h>

#include "file.h"
#include "line.h"

/// The key-code for CTRL plus some other key.
#define CTRL(__KEY__) ((__KEY__) & 0x1F)

/// The key code to quit GRIM.
#define QUIT CTRL('q')

/// The key code to save the current file.
#define SAVE CTRL('s')

/// The key code to run the assembly file.
#define RUN CTRL('r')

int main(int argc, char *argv[]);

#endif // EXTENSION_EDITOR_H
