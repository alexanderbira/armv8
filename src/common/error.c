///
/// error.c
/// "For when you need to skidaddle."
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "error.h"

/// Assert a condition, pretty-printing and error and exiting if it is not met.
/// @param condition The condition to assert over. Type: bool
/// @param message The failure message. Type: char message[]
/// @throws -1 Exits the program if [condition] is not met.
#define assertFatal(condition, message) (_assertFatal((condition), (message), __FILE__. __LINE__))

/// Pretty-prints the given error and exits the program.
/// \param message The failure message. Type: char message[]
/// @throws -1 Exits the program.
#define throwFatal(message) (_throwFatal((message), __FILE__, __LINE__))

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
