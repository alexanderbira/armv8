///
/// registerDecoder.h
/// Decodes a register instruction into its IR
///
/// Created by Billy Highley on 30/05/2024.
///

#ifndef EMULATOR_REGISTER_DECODER_H
#define EMULATOR_REGISTER_DECODER_H

#include "../../common/const.h"
#include "../../common/error.h"
#include "../../common/ir/ir.h"

IR decodeRegister(Instruction word);

#endif // EMULATOR_REGISTER_DECODER_H