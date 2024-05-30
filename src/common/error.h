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

void assertFatal(bool condition, char message[]);

void throwFatal(char message[]);

#endif //COMMON_ERROR_H
