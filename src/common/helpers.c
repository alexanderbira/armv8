///
/// helpers.c
/// Utility functions for the rest of the program.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#include "helpers.h"

void trim(char *str) {
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char) str[begin++]));
    while (end >= begin && isspace((unsigned char) str[end--]));

    int i;
    for (i = begin; i <= end; i++) {
        str[i - begin] = str[i];
    }

    str[i - begin] = '\0';
}