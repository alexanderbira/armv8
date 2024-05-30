///
/// const.h
/// Constants for the rest of the program.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#ifndef COMMON_CONST_H
#define COMMON_CONST_H

#include "stdint.h"

/// The virtual memory size of the emulated machine.
#define MEMORY_SIZE (2 << 20) // 2MB

/// The number of general purpose registers in the virtual machine.
#define NUM_GPRS 31

/// Alias for a chunk of data passed to and from the virtual registers or memory.
typedef uint64_t BitData;

/// Alias for an instruction as a binary word.
typedef uint32_t BitInst;

/// Shorthand for binary literals. Ignores '_'s.
/// @example \code b(1010_0101) = 0xA5 \endcode
/// @author Alexander Biraben-Renard
/// @param x The binary as a string.
/// @warning May not be optimised at compile - do not abuse!
#define b(x) toBinary(#x)

static inline uint64_t toBinary(const char *str) {
    uint64_t result = 0;
    while (*str) {
        if (*str == '-') {
            str++;
            continue;
        }
        result <<= 1;
        result += *str++ - '0';
    }
    return result;
}

#endif //COMMON_CONST_H
