///
/// error.c
/// "For when you need to skidaddle."
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "error.h"

static noreturn void generateFatal(char format[], const char *file, int line, const char *func, va_list args) {
    char message[1024];
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);

    if (JUMP_ON_ERROR) {
        fatalError = strdup(message);
        longjmp(fatalBuffer, 1);
    } else {
        fprintf(stderr, "[%s] %s\n", func, message);
        fprintf(stderr, "    In file %s, line %d\n", file, line);
        if (errno) {
            fprintf(stderr, "    With description: %s\n", strerror(errno));
        }
        exit(1);
    }
}

void assertFatal_(bool condition, char format[], char *file, int line, const char *func, ...) {
    va_list args;
    va_start(args, func);
    if (!condition) generateFatal(format, file, line, func, args);
}

noreturn void throwFatal_(char format[], char *file, int line, const char *func, ...) {
    va_list args;
    va_start(args, func);
    generateFatal(format, file, line, func, args);
}
