///
/// error.c
/// "For when you need to skidaddle."
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "error.h"

void _assertFatal(bool condition, char message[], char *file, int line) {
    if (!condition) {
        fprintf(stderr, "%s\n", message);
        fprintf(stderr, "   In file %s, line %d \n\n", file, line);
        perror("    Fatal Error");
        exit(-1);
    }
}

noreturn void _throwFatal(char message[], char *file, int line) {
    fprintf(stderr, "%s\n", message);
    fprintf(stderr, "   In file %s, line %d", file, line);
    perror("    Fatal Error");
    exit(-1);
}
