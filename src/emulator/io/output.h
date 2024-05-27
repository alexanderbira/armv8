///
/// output.h
/// The printing of register and memory content after execution of program.
///
/// Created by Lancelot Liu on 27/05/2024.
///

#ifndef EMULATOR_OUTPUT_H
#define EMULATOR_OUTPUT_H

#include <stdio.h>

#include "../system/registers.h"

void dumpRegs(Registers *reg);

void dumpMem(void *mem);

#endif //EMULATOR_OUTPUT_H
