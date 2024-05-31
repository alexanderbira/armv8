///
/// mask.h
/// Functions for generating and applying masks
///
/// Created by Alexander Biraben-Renard and Billy Highley on 27/05/2024.
///

#ifndef MASK_H
#define MASK_H

#include "instructions.h"

/// Calls b(x) with x as a string
/// @example \code{.c}B(1011) == b("1011") == 0xB\endcode
#define B(x) b(#x)

/// Calls bitMask(msb, lsb)
#define BitMask(msb, lsb) bitMask(msb, lsb)

typedef uint32_t Mask;

/// Given msb and lsb, return a mask which will be used to obtain the
/// bits in the range msb >= b >= lsb inclusive from a 32-bit instruction.
/// @param msb The most significant bit of the range.
/// @param lsb The least significant bit of the range.
/// @return A 32-bit Mask with 1s in the range msb to lsb (inclusive).
/// @example \code{.c}bitMask(5, 3) == 111000\endcode
static inline Mask bitMask(int msb, int lsb) {
    return ((1 << (msb - lsb + 1)) - 1) << lsb;
}

/// Converts a binary string to its uint32_t representation.
/// @param s Binary string.
/// @return  32-bit (unsigned) integer representation.
static inline uint32_t b(const char *s) {
    uint32_t i = 0;
    while (*s) {
        i <<= 1; i += *s++ - '0';
    }
    return i;
}

Component decompose(Instruction word, Mask mask);

#endif //MASK_H
