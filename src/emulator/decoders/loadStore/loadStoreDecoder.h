///
/// loadStoreDecode.h
/// Decode a load and store instruction to its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 27/05/2024.
///

#ifndef EMULATOR_LOAD_STORE_DECODER_H
#define EMULATOR_LOAD_STORE_DECODER_H

#include <stdio.h>
#include <stdlib.h>

#include "../../../common/const.h"
#include "../../../common/error.h"
#include "../../../common/ir/ir.h"

IR decodeLoadStore(Instruction word);

#endif // EMULATOR_LOAD_STORE_DECODER_H
