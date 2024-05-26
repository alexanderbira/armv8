///
/// memory.h
/// Structure, manipulation, allocation, and freeing of the virtual memory.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#include "memory.h"

/// Allocates a chunk of virtual memory preloaded with the contents of the given file handler.
/// @pre No errors occurred when the file handler [fd] was created.
/// @param size The size of the memory to allocate.
/// @param fd File handler of initial contents.
/// @return Generic pointer to memory.
void *allocMemFromFile(int fd) {
    // Get statistics on the file.
    struct stat sb;
    assert(fstat(fd, &sb) == 0);

    // Must have enough space to store file.
    assert(sb.st_size <= MEMORY_SIZE);

    // Allocate and read file into memory.
    void *memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, fd, 0);
    assert(memory != NULL);

    // Zero out rest of memory.
    uint8_t *ptr = memory + sb.st_size;
    while (ptr != memory + MEMORY_SIZE) *ptr++ = 0;

    return memory;
}

/// Allocates a chunk of blank virtual memory.
/// @param size The size of the memory to allocate.
/// @return Generic pointer to memory.
void *allocMem() {
    void *memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
    assert(memory != NULL);

    // Zero out rest of memory.
    uint8_t *ptr = memory;
    while (ptr != memory + MEMORY_SIZE) *ptr++ = 0;

    return memory;
}

/// Frees the given chunk of virtual memory.
/// @param mem Generic pointer to virtual memory to free.
void freeMem(void *mem) {
    assert(munmap(mem, MEMORY_SIZE) == 0);
}

/// Reads 32-bits from virtual memory.
/// @param mem The address of the virtual memory.
/// @param addr The address within the virtual memory.
/// @return The 32-bit value at mem + addr.
uint32_t readMem32(void *mem, size_t addr) {
    assert(addr + 32 <= MEMORY_SIZE);

    uint8_t *ptr = (uint8_t *) mem + addr;
    return (uint32_t) ptr[0]
           || (uint32_t) ptr[1] << 8
           || (uint32_t) ptr[2] << 16
           || (uint32_t) ptr[3] << 24;
}

/// Reads 64-bits from virtual memory.
/// @param mem The address of the virtual memory.
/// @param addr The address within the virtual memory.
/// @return The 64-bit value at mem + addr.
uint64_t readMem64(void *mem, size_t addr) {
    assert(addr + 64 <= MEMORY_SIZE);

    uint8_t *ptr = (uint8_t *) mem + addr;
    return (uint64_t) ptr[0]
           || (uint64_t) ptr[1] << 8
           || (uint64_t) ptr[2] << 16
           || (uint64_t) ptr[3] << 24
           || (uint64_t) ptr[4] << 32
           || (uint64_t) ptr[5] << 40
           || (uint64_t) ptr[6] << 48
           || (uint64_t) ptr[7] << 56;
}

/// Writes 32-bits to virtual memory.
/// @param mem The address of the virtual memory.
/// @param addr The address within the virtual memory.
/// @param value The value to write.
void writeMem32(void *mem, size_t addr, uint32_t value) {
    assert(addr + 32 <= MEMORY_SIZE);

    uint8_t *ptr = (uint8_t *) mem + addr;
    ptr[0] = (uint8_t) value;
    ptr[1] = (uint8_t) (value >> 8);
    ptr[2] = (uint8_t) (value >> 16);
    ptr[3] = (uint8_t) (value >> 24);
}

/// Writes 64-bits from virtual memory.
/// @param mem The address of the virtual memory.
/// @param addr The address within the virtual memory.
/// @param value The value to write.
void writeMem64(void *mem, size_t addr, uint64_t value) {
    assert(addr + 64 <= MEMORY_SIZE);

    uint8_t *ptr = (uint8_t *) mem + addr;
    ptr[0] = (uint8_t) value;
    ptr[1] = (uint8_t) (value >> 8);
    ptr[2] = (uint8_t) (value >> 16);
    ptr[3] = (uint8_t) (value >> 24);
    ptr[4] = (uint8_t) (value >> 32);
    ptr[5] = (uint8_t) (value >> 40);
    ptr[6] = (uint8_t) (value >> 48);
    ptr[7] = (uint8_t) (value >> 56);
}
