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

/// Reads 64/32-bits from virtual memory. If 32-bits is selected, higher bits will be set to 0.
/// @param mem The address of the virtual memory.
/// @param as64 Whether to read 64 or 32 bits.
/// @param addr The address within the virtual memory.
/// @return The 64-bit value at mem + addr.
uint64_t readMem(Memory mem, bool as64, size_t addr) {
    size_t readSize = as64 ? sizeof(uint64_t) : sizeof(uint32_t);
    assert(addr + readSize <= MEMORY_SIZE);

    // Read virtual memory as little-endian.
    uint8_t *ptr = (uint8_t *) mem + addr;
    uint64_t res = ptr[0];
    for (int i = 1; i < readSize; i++) {
        res |= ((uint64_t) ptr[i] << i * 8);
    }
    return res;
}

/// Writes 64/32-bits to virtual memory. If 32-bits is selected, the higher bits of [value] will be ignored.
/// @param mem The address of the virtual memory.
/// @param as64 Whether to write 64 or 32 bits.
/// @param addr The address within the virtual memory.
/// @param value The value to write.
void writeMem(Memory mem, bool as64, size_t addr, uint64_t value) {
    size_t writeSize = as64 ? sizeof(uint64_t) : sizeof(uint32_t);
    assert(addr + writeSize <= MEMORY_SIZE);

    uint8_t *ptr = (uint8_t *) mem + addr;
    for (int i = 0; i < writeSize; i++) {
        ptr[i] = (uint8_t)(value >> 8 * i);
    }
}
