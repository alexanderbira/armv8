///
/// decode.h
/// Links instruction process functions and system resources for decoding.
///
/// Created by Billy Highley on 27/05/2024.
///

#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "masks.h"
#include "../system/registers.h"
#include "../system/memory.h"

void decode(Instruction word, Registers regs, Memory mem);

#endif //DECODE_H
