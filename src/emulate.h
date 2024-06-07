///
/// emulate.h
/// Emulates execution of an AArch64 binary file.
///
/// Created by Jack Wong on 29/05/2024.
///

#ifndef EMULATE_H
#define EMULATE_H

#include <stdlib.h>

#include "common/ir/ir.h"
#include "emulator/emulatorDelegate.h"
#include "emulator/system/memory.h"
#include "emulator/system/output.h"
#include "emulator/system/registers.h"

#define HALT 0x8a000000

#endif //EMULATE_H
