///
/// emulate.h
/// Emulate ARM64 code
///
/// Created by Jack Wong on 29/05/2024.
///

#ifndef EMULATE_H
#define EMULATE_H

#include <stdlib.h>

#include "common/ir/ir.h"
#include "emulator/process.h"
#include "emulator/system/memory.h"
#include "emulator/system/output.h"
#include "emulator/system/registers.h"

#define HALT_INSTR_C 0x8a000000

#endif //EMULATE_H
