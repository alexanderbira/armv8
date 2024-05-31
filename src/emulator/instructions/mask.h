///
/// mask.h
/// Functions for generating and applying masks
///
/// Created by Alexander Biraben-Renard and Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_MASK_H
#define EMULATOR_MASK_H

#include "instructions.h"

typedef uint32_t Mask;

Component decompose(Instruction word, Mask mask);

#endif // EMULATOR_MASK_H
