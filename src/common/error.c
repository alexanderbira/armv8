///
/// error.c
/// "For when you need to skidaddle."
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "error.h"

void assertFatal_(bool condition, char message[], char *file, int line, const char *func) {
    if (!condition) throwFatal_(message, file, line, func);
}

noreturn void throwFatal_(char message[], char *file, int line, const char *func) {
    #ifdef JUMP_ON_ERROR
    fatalError = strdup(message);
    longjmp(fatalBuffer, 1);
    #else
    fprintf(stderr, "[%s] %s\n", func, message);
    fprintf(stderr, "    In file %s, line %d\n", file, line);
    if (errno) {
        fprintf(stderr, "    With description: %s\n", strerror(errno));
    }
    #endif
    exit(1);
}
