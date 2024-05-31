///
/// const.h
/// Constants for the rest of the program.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#ifndef COMMON_CONST_H
#define COMMON_CONST_H

#include <limits.h>
#include <stdint.h>

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

/// Shorthand for simple bitmasks. Many thanks to SO for this ingenious trick.
/// @example \code mask(uint8_t, 3) = 0x07 \endcode
/// @author https://stackoverflow.com/a/28703383/
#define mask(__TYPE__, __ONE_COUNT__) \
    (((__TYPE__) (-((__ONE_COUNT__) != 0))) \
    & (((__TYPE__) -1) >> ((sizeof(__TYPE__) * CHAR_BIT) - (__ONE_COUNT__))))

static inline uint64_t toBinary(const char *str) {
    uint64_t result = 0;
    while (*str) {
        if (*str == '_') {
            str++;
            continue;
        }
        result <<= 1;
        result += *str++ - '0';
    }
    return result;
}

/// Truncates the least significant [numBits] bits of [value].
/// @param value The value to truncate.
/// @param numBits How many bits to preserve.
/// @return The truncated value.
inline uint64_t truncate(uint64_t value, size_t numBits) {
    return value & mask(uint64_t, numBits);
}

#endif //COMMON_CONST_H
