///
/// error.c
/// "For when you need to skidaddle."
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "error.h"

void assertFatal_(bool condition, char message[], char *file, int line, const char *func) {
    if (!condition) {
        fprintf(stderr, "[%s] %s\n", func, message);
        fprintf(stderr, "   In file %s, line %d \n\n", file, line);
        perror("    Fatal Error");
        exit(-1);
    }
}

noreturn void throwFatal_(char message[], char *file, int line, const char *func) {
    fprintf(stderr, "[%s] %s\n", func, message);
    fprintf(stderr, "   In file %s, line %d", file, line);
    perror("    Fatal Error");
    exit(-1);
}
