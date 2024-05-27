///
/// masks.h
/// Masks, types, and mask functions
///
/// Created by Alexander Biraben-Renard and Billy Highley on 27/05/2024.
///

#ifndef MASKS_H
#define MASKS_H

#include <stdint.h>

typedef uint32_t Instruction;
typedef uint32_t Component;
typedef uint32_t Mask;

Mask bitMask(int, int);

static inline uint32_t B(const char *);
#define b(x) B(#x)

#define OP0_MASK bitMask(28, 25)

#define DP_IMM_OP0_MASK     b(1110)
#define DP_REG_OP0_MASK     b(0111)
#define LOAD_STORE_OP0_MASK b(0101)
#define BRANCH_OP0_MASK     b(1110)

#define DP_IMM_OP0_CODE     b(1000)
#define DP_REG_OP0_CODE     b(0101)
#define LOAD_STORE_OP0_CODE b(0100)
#define BRANCH_OP0_CODE     b(1010)

/// Converts a binary string to its uint32_t representation.
/// @param s Binary string.
/// @return  32-bit (unsigned) integer representation.
static inline uint32_t B(const char *s) {
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

#endif //MASKS_H
