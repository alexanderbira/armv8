///
/// memory.h
/// Structure, manipulation, allocation, and freeing of the virtual memory.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#include "memory.h"

/// Allocates a chunk of virtual memory preloaded with the contents of the given file handler.
/// @pre No errors occurred when the file handler [fd] was created.
/// @param fd File handler of initial contents.
/// @return Generic pointer to memory.
Memory allocMemFromFile(char *path) {
    // Open the file
    int fd = open(path, O_RDONLY);

    // Get statistics on the file.
    struct stat sb;
    assert(fstat(fd, &sb) == 0);

    // Must have enough space to store file.
    assert(sb.st_size <= MEMORY_SIZE);

    // Allocate memory.
    Memory mem = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assert(mem != NULL);

    // Zero out rest of memory, then read file into beginning.
    memset(mem, 0, MEMORY_SIZE);
    ssize_t bytes_read = pread(fd, mem, sb.st_size, 0);
    assert(bytes_read == sb.st_size);

    return mem;
}

/// Allocates a chunk of blank virtual memory.
/// @return Generic pointer to memory.
Memory allocMem(void) {
    Memory mem = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assert(mem != NULL);

    // Zero out rest of memory.
    uint8_t *ptr = mem;
    while (ptr != (uint8_t *) mem + MEMORY_SIZE) *ptr++ = 0;

    return mem;
}

/// Frees the given chunk of virtual memory.
/// @param mem Generic pointer to virtual memory to free.
void freeMem(Memory mem) {
    assert(munmap(mem, MEMORY_SIZE) == 0);
}

/// Reads 32-bits from virtual memory.
/// @param mem The address of the virtual memory.
/// @param addr The address within the virtual memory.
/// @return The 32-bit value at mem + addr.
uint32_t readMem32(Memory mem, size_t addr) {
    assert(addr + sizeof(uint32_t) <= MEMORY_SIZE);

    // Read virtual memory as little-endian.
    uint8_t *ptr = (uint8_t *) mem + addr;
    uint32_t res = ptr[0];
    for (int i = 1; i < sizeof(uint32_t); i++) {
        res |= ((uint32_t) ptr[i] << i * 8);
    }
    return res;
}

/// Reads 64-bits from virtual memory.
/// @param mem The address of the virtual memory.
/// @param addr The address within the virtual memory.
/// @return The 64-bit value at mem + addr.
uint64_t readMem64(Memory mem, size_t addr) {
    assert(addr + sizeof(uint64_t) <= MEMORY_SIZE);

    // Read virtual memory as little-endian.
    uint8_t *ptr = (uint8_t *) mem + addr;
    uint64_t res = ptr[0];
    for (int i = 1; i < sizeof(uint64_t); i++) {
        res |= ((uint64_t) ptr[i] << i * 8);
    }
    return res;
}

/// Writes 32-bits to virtual memory.
/// @param mem The address of the virtual memory.
/// @param addr The address within the virtual memory.
/// @param value The value to write.
void writeMem32(Memory mem, size_t addr, uint32_t value) {
    assert(addr + sizeof(uint32_t) <= MEMORY_SIZE);

    uint8_t *ptr = (uint8_t *) mem + addr;
    for (int i = 0; i < sizeof(uint32_t); i++) {
        ptr[i] = (uint8_t)(value >> 8 * i);
    }
}

/// Writes 64-bits to virtual memory.
/// @param mem The address of the virtual memory.
/// @param addr The address within the virtual memory.
/// @param value The value to write.
void writeMem64(Memory mem, size_t addr, uint64_t value) {
    assert(addr + sizeof(uint64_t) <= MEMORY_SIZE);

    uint8_t *ptr = (uint8_t *) mem + addr;
    for (int i = 0; i < sizeof(uint64_t); i++) {
        ptr[i] = (uint8_t)(value >> 8 * i);
    }
}
