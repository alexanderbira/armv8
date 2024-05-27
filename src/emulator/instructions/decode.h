///
/// decode.h
/// Delegate the work to the correct instruction handler
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "op0.h"
#include "../system/registers.h"
#include "../system/memory.h"

void decode(Instruction word, Registers regs, Memory mem);

#endif //DECODE_H
