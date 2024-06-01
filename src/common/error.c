///
/// error.c
/// "For when you need to skidaddle."
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "error.h"

/// Assert a condition, pretty-printing and error and exiting if it is not met.
/// @param condition The condition to assert over.
/// @param message The failure message.
/// @throws -1 Exits the program if [condition] is not met.
void assertFatal(bool condition, char message[]) {
    if (!condition) {
        fprintf(stderr, "%s\n", message);
        fprintf(stderr, "   In file %s, line %d \n\n", __FILE__, __LINE__);
        perror("    Fatal Error");
        exit(-1);
    }
}

/// Pretty-prints the given error and exits the program.
/// \param message The failure message.
/// @throws -1 Exits the program.
noreturn void throwFatal(char message[]) {
    fprintf(stderr, "%s\n", message);
    fprintf(stderr, "   In file %s, line %d", __FILE__, __LINE__);
    perror("    Fatal Error");
    exit(-1);
}
