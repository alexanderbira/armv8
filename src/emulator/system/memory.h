///
/// memory.h
/// Structure, manipulation, allocation, and freeing of the virtual memory.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "../../defs/const.h"

/// Type definition representing virtual memory.
typedef void* Memory;

/// Type definition representing

Memory allocMemFromFile(char *path);

Memory allocMem(void);

void freeMem(Memory mem);

uint64_t readMem(Memory mem, bool as64, size_t addr);

void writeMem(Memory mem, bool as64, size_t addr, uint64_t value);

#endif //EMULATOR_MEMORY_H
