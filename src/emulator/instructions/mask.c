///
/// mask.c
/// Functions for generating and applying masks
///
/// Created by Billy Highley on 27/05/2024.
///

#include "mask.h"

/// Converts a binary string to its uint32_t representation.
/// @param s Binary string.
/// @return  32-bit (unsigned) integer representation.
uint32_t B(const char *s) {
    uint32_t i = 0;
    while (*s) {
        i <<= 1; i += *s++ - '0';
    }
    return i;
}

/// Given msb and lsb, return a mask which will be used to obtain the
/// bits in the range msb >= b >= lsb inclusive from a 32-bit instruction.
/// @param msb The most significant bit of the range.
/// @param lsb The least significant bit of the range.
/// @return A 32-bit Mask with 1s in the range msb to lsb (inclusive).
/// @example \code{.c}bitMask(5, 3) == 111000\endcode
Mask bitMask(int msb, int lsb) {
    return ((1 << (msb - lsb + 1)) - 1) << lsb;
}

/// Applies the given mask to an instruction and returns the bits
/// shifted so that the LSB is right-aligned to yield the component.
/// @param word The instruction to mask.
/// @param mask The mask to use on the instruction.
/// @return The shifted bit pattern extracted by the mask.
/// @example \code{.c}decompose(10111, 11100) == 00101\endcode
Component decompose(Instruction word, Mask mask) {
    uint32_t bits = word & mask;
    while (!(mask & 1) && (mask >>= 1)) {
        bits >>= 1;
    }
    return bits;
}
