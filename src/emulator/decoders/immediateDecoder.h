///
/// immediateDecoder.h
/// Decodes an immediate instruction into its IR.
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef EMULATOR_IMMEDIATE_DECODER_H
#define EMULATOR_IMMEDIATE_DECODER_H

#include "../../common/const.h"
#include "../../common/error.h"
#include "../../common/ir/ir.h"

IR decodeImmediate(Instruction word);

#endif // EMULATOR_IMMEDIATE_DECODER_H