///
/// error.c
/// "For when you need to skidaddle."
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "error.h"

void assertFatal(bool condition, char message[]) {
    if (!condition) {
        fprintf(stderr, "%s\n", message);
        fprintf(stderr, "   In file %s, line %d", __FILE__, __LINE__);
        perror("    Fatal Error");
        exit(-1);
    }
}

void throwFatal(char message[]) {
    fprintf(stderr, "%s\n", message);
    fprintf(stderr, "   In file %s, line %d", __FILE__, __LINE__);
    perror("    Fatal Error");
    exit(-1);
}
