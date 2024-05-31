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

/// Shorthand to mark an input parameter is unused. This is usually because a type signature
/// has to be followed for a group of functions.
#define unused __attribute__((unused))

/// Shorthand for binary literals. Ignores '_'s.
/// @example \code b(1010_0101) = 0xA5 \endcode
/// @param x The binary as a string.
/// @warning May not be optimised at compile - do not abuse!
#define b(x) toBinary(#x)

// Shorthand for simple bitmasks on the "right hand" / least significant side. One-indexed.
/// Significant inspiration taken from @see https://stackoverflow.com/a/28703383/
/// @example \code maskr(uint8_t, 3) = 0x07 \endcode
/// @warning Produces [uint32_t]!
/// @pre 0 <= __ONE_COUNT__ <= 32
#define maskr(__ONE_COUNT__) \
    ((__ONE_COUNT__) == 0 ? 0 : (((uint32_t)-1) >> (sizeof(uint32_t) * CHAR_BIT - (__ONE_COUNT__))))

/// Shorthand for simple bitmasks on the "left hand" / most significant side. One-indexed.
/// @example \code maskl(4) = 0xF0000000 \endcode
/// @warning Produces [uint32_t]!
/// @pre 0 <= __ONE_COUNT__ <= 32
#define maskl(__ONE_COUNT__) \
    ((__ONE_COUNT__) == 0 ? 0 : (((uint32_t)-1) << (sizeof(uint32_t) * CHAR_BIT - (__ONE_COUNT__))))

/// Shorthand for simple bitmasks in the "center" / delimited by MSB and LSB, inclusive. 0-indexed.
/// Based on original by Billy Highley, \code ((1 << (msb - lsb + 1)) - 1) << lsb; \endcode
/// @example \code mask(3, 1) = 0xE \endcode \code mask(31, 29) = 0xe0000000 \endcode
/// @warning Produces [uint32_t]!
/// @pre 31 >= __MSB__, __LSB__ >= 0
#define mask(__MSB__, __LSB__) maskl(8 * sizeof(uint32_t) - __LSB__) & maskr(__MSB__ + 1)

/// Shorthand for truncating a value to just [__BIT_COUNT__] of its least significant bits.
/// @example \code truncatel(0xF, 3) = 0x7 \endcode
#define truncater(__VALUE__, __BIT_COUNT__) __VALUE__ & maskr(__BIT_COUNT__)

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

#endif // COMMON_CONST_H
