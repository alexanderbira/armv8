///
/// output.h
/// The printing of register and memory content after execution of program.
///
/// Created by Lancelot Liu on 27/05/2024.
///

#ifndef EMULATOR_OUTPUT_H
#define EMULATOR_OUTPUT_H

#include <inttypes.h>
#include <stdio.h>

#include "../system/registers.h"
#include "../system/memory.h"

void dumpRegs(Registers *reg, FILE *fileOut);

void dumpMem(Memory mem, FILE *fileOut);

#endif //EMULATOR_OUTPUT_H
