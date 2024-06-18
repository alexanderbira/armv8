///
/// termSizeOverlay.h
/// The destructive overlay displayed when the terminal size is too small.
///
/// Created by Lancelot Liu on 18/06/2024.
///

#include <ncurses.h>
#include <string.h>
#include <stdio.h>

/// The minimum width (in characters) of the terminal required for GRIM.
#define MINIMUM_WIDTH 80

/// The minimum height (in characters) of the terminal required for GRIM.
#define MINIMUM_HEIGHT 20

static const char *overlayText = "[ Terminal size too small! ]";

static const int overlayLength = 28;

extern int rows, cols;

extern WINDOW *title, *lineNumbers, *editor, *help, *separator, *regView;

extern WINDOW* termSizeOverlay;

void updateTermSizeOverlay(bool active);
