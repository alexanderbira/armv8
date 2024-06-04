///
/// error.h
/// "For when you need to skidaddle."
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef COMMON_ERROR_H
#define COMMON_ERROR_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

/// Assert a condition, pretty-printing and error and exiting if it is not met.
/// @param condition The condition to assert over. Type: bool
/// @param message The failure message. Type: char message[]
/// @throws -1 Exits the program if [condition] is not met.
#define assertFatal(condition, message) (assertFatal_(condition, message, __FILE__, __LINE__))

/// Pretty-prints the given error and exits the program.
/// \param message The failure message. Type: char message[]
/// @throws -1 Exits the program.
#define throwFatal(message) (throwFatal_(message, __FILE__, __LINE__))

void assertFatal_(bool condition, char message[], char *file, int line);

noreturn void throwFatal_(char message[], char *file, int line);

#endif // COMMON_ERROR_H
