//
// Created by Lancelot Liu on 30/05/2024.
//

#include "helpers.h"

/// Trims the specified ([except]) characters from the beginning and end of the given string ([str]).
/// @param[in, out] str Pointer to the string to be trimmed.
/// @param[in] except List of character(s) to trim.
/// @return Pointer to the trimmed string.
/// @attention Mutates input pointer [str].
char *trim(char *str, const char *except) {
    // Trim leading space.
    for (; *str && strchr(except, *str) != NULL; str++);

    // Trim trailing space.
    char *end = str + strlen(str) - 1;
    for (; end > str && strchr(except, *end) != NULL; end--);

    // Terminate and return string.
    *++end = '\0';
    return str;
}

/// Splits the given string ([str]) into parts by the given delimiters ([delim]).
/// @param[in, out] str Pointer to the string to be split.
/// @param delim[in] List of delimiter(s).
/// @param count[out] Number of pieces the incoming string was split into.
/// @return Pointer to the array of split strings.
/// @attention Mutates input pointer [str].
char **split(char *str, const char *delim, int *count) {
    // Count how many elements will be extracted
    *count = 0;
    char *tmp = str;
    char *lastComma = NULL;
    while (*tmp++) {
        if (strchr(delim, *tmp) != NULL) {
            (*count)++; // Thanks compiler :)))
            lastComma = tmp;
        }
    }

    // Don't forget to count what's after the trailing comma!
    *count += lastComma < (str + strlen(str) - 1);
    char **result = malloc(*count * sizeof(char *));

    if (result) {
        int idx = 0;
        char *token = strtok(str, delim);

        while (token) {
            *(result + idx++) = strdup(token);
            token = strtok(NULL, delim);
        }
    }

    return result;
}
