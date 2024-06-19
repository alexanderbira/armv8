///
/// termSizeOverlay.h
/// The destructive overlay displayed when the terminal size is too small.
///
/// Created by Lancelot Liu on 18/06/2024.
///

#ifndef EXTENSION_TERM_SIZE_OVERLAY_H
#define EXTENSION_TERM_SIZE_OVERLAY_H

#include <ncurses.h>
#include <string.h>
#include <stdio.h>

#include "const.h"

/// The minimum width (in characters) of the terminal required for GRIM.
#define MINIMUM_WIDTH  60

/// The minimum height (in characters) of the terminal required for GRIM.
#define MINIMUM_HEIGHT 25

extern int rows, cols;

void showTermSizeOverlay(void);

#endif // EXTENSION_TERM_SIZE_OVERLAY_H
