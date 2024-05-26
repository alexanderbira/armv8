///
/// memory.h
/// Structure, manipulation, allocation, and freeing of the virtual memory.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H

#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>

#include "../../defs/const.h"

void *allocMemFromFile(int fd);

void *allocMem(void);

uint32_t readMem32(void *mem, size_t addr);

uint64_t readMem64(void *mem, size_t addr);

void writeMem32(void *mem, size_t addr, uint32_t value);

void writeMem64(void *mem, size_t addr, uint64_t value);

#endif //EMULATOR_MEMORY_H
