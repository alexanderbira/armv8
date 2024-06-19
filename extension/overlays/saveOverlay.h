///
/// saveOverlay.h
/// The destructive overlay displayed when the user wishes to save but no path is recorded.
///
/// Created by Lancelot Liu on 18/06/2024.
///

#ifndef EXTENSION_SAVE_OVERLAY_H
#define EXTENSION_SAVE_OVERLAY_H

#include <ctype.h>
#include <form.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"
#include "helpers.h"
#include "file.h"

extern int rows, cols;

extern WINDOW *title, *lineNumbers, *editor, *help, *separator, *regView;

extern WINDOW *saveOverlay;

bool showSaveOverlay(File *file);

#endif // #define EXTENSION_SAVE_OVERLAY_H
