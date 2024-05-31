///
/// emulate.h
/// Emulate ARM64 code
///
/// Created by Jack Wong on 29/05/2024.
///

#ifndef EMULATE_H
#define EMULATE_H

#include <stdlib.h>

#include "emulator/system/registers.h"
#include "emulator/system/memory.h"
#include "emulator/io/output.h"
#include "emulator/instructions/decode.h"
#include "emulator/instructions/execute.h"

#define HALT_INSTR_C 0x8a000000

#endif //EMULATE_H
