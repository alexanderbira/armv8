///
/// registerDecoder.h
/// Decodes a binary word of a data processing (register) instruction to its [IR].
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef EMULATOR_REGISTER_DECODER_H
#define EMULATOR_REGISTER_DECODER_H

#include "const.h"
#include "error.h"
#include "ir.h"

IR decodeRegister(Instruction word);

#endif // EMULATOR_REGISTER_DECODER_H
