///
/// mask.c
/// Functions for generating and applying masks
///
/// Created by Billy Highley and Alexander Biraben-Renard on 28/05/24.
///

#include "mask.h"

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
