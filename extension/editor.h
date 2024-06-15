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

#include "file.h"
#include "line.h"

#define JUMP_ON_ERROR

jmp_buf fatalBuffer;
char *fatalError;

int main(int argc, char *argv[]);

#endif // EXTENSION_EDITOR_H
