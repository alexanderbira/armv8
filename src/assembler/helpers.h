//
// Created by Lancelot Liu on 30/05/2024.
//

#ifndef ASSEMBLER_HELPERS_H
#define ASSEMBLER_HELPERS_H

#include <stdlib.h>
#include <string.h>

char *trim(char *str, const char *except);

char **split(char *str, const char *delim, int *count);

#endif //ASSEMBLER_HELPERS_H
