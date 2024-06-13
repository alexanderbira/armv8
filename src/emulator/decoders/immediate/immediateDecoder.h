///
/// immediateDecoder.h
/// ecodes a binary word of a data processing (immediate) instruction to its [IR].
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_IMMEDIATE_DECODER_H
#define EMULATOR_IMMEDIATE_DECODER_H

#include "const.h"
#include "error.h"
#include "ir.h"

IR decodeImmediate(Instruction word);

#endif // EMULATOR_IMMEDIATE_DECODER_H
