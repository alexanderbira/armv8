///
/// mask.h
/// Functions for generating and applying masks
///
/// Created by Alexander Biraben-Renard on 27/05/2024.
///

#ifndef MASK_H
#define MASK_H

#include "instructions.h"

/// Calls B(x) with x as a string
/// @example \code{.c}b(1011) == B("1011") == 0xB\endcode
#define b(x) B(#x)

typedef uint32_t Mask;

Mask bitMask(int msb, int lsb);

uint32_t B(const char *s);

Component decompose(Instruction word, Mask mask);

#endif //MASK_H
