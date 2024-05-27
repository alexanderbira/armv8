///
/// memory.h
/// Structure, manipulation, allocation, and freeing of the virtual memory.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H

#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <assert.h>

#include "../../defs/const.h"

typealias (void *) Memory;

Memory allocMemFromFile(int fd);

Memory allocMem(void);

void freeMem(Memory mem);

uint32_t readMem32(Memory mem, size_t addr);

uint64_t readMem64(Memory mem, size_t addr);

void writeMem32(Memory mem, size_t addr, uint32_t value);

void writeMem64(Memory mem, size_t addr, uint64_t value);

#endif //EMULATOR_MEMORY_H
