///
/// memory.h
/// Structure, manipulation, allocation, and freeing of the virtual memory.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#include "memory.h"

/// Allocates a chunk of virtual memory preloaded with the contents of the given file handler.
/// @param fd File handler of initial contents.
/// @returns Generic pointer to memory.
/// @pre No errors occurred when the file handler [fd] was created.
Memory allocMemFromFile(char *path) {
    // Open the file
    int fd = open(path, O_RDONLY);

    // Get statistics on the file.
    struct stat sb;
    assertFatal(fstat(fd, &sb) == 0, "Unable to get statistics on file!");

    // Must have enough space to store file.
    assertFatal(sb.st_size <= MEMORY_SIZE, "Virtual memory not big enough for binary file!");

    // Allocate memory.
    Memory memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assertFatal(memory != MAP_FAILED, "<Memory> Unable to allocate memory!");

    // Zero out rest of memory, then read file into beginning.
    memset(memory, 0, MEMORY_SIZE);
    ssize_t bytes_read = pread(fd, memory, sb.st_size, 0);
    assertFatal(bytes_read == sb.st_size, "<Memory> Something went wrong during reading-in of binary file!");

    close(fd);

    return memory;
}

/// Allocates a chunk of blank virtual memory.
/// @return Generic pointer to memory.
Memory allocMem(void) {
    Memory memory = mmap(NULL, MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    assertFatalNotNull(memory, "<Memory> Unable to allocate memory!");

    // Zero out rest of memory.
    uint8_t *ptr = memory;
    while (ptr != (uint8_t *) memory + MEMORY_SIZE) *ptr++ = 0;

    return memory;
}

/// Frees the given chunk of virtual memory.
/// @param memory Generic pointer to virtual memory to free.
void freeMem(Memory memory) {
    assertFatal(munmap(memory, MEMORY_SIZE) == 0, "<Memory> Unable to un-map memory!");
}

/// Reads 64/32-bits from virtual memory. If 32-bits is selected, higher bits will be set to 0.
/// @param memory The address of the virtual memory.
/// @param as64 Whether to read 64 or 32 bits.
/// @param addr The address within the virtual memory.
/// @returns The 64-bit value at mem + addr.
BitData readMem(Memory memory, bool as64, size_t addr) {
    size_t readSize = as64 ? sizeof(uint64_t) : sizeof(uint32_t);
    assertFatal(addr + readSize <= MEMORY_SIZE, "<Memory> Received out-of-bound read to memory!");

    // Read virtual memory as little-endian.
    uint8_t *ptr = (uint8_t *) memory + addr;
    uint64_t result = ptr[0];
    for (size_t i = 1; i < readSize; i++) {
        result |= ((uint64_t) ptr[i] << i * 8);
    }
    return result;
}

/// Writes 64/32-bits to virtual memory. If 32-bits is selected, the higher bits of [value] will be ignored.
/// @param memory The address of the virtual memory.
/// @param as64 Whether to write 64 or 32 bits.
/// @param addr The address within the virtual memory.
/// @param value The value to write.
void writeMem(Memory memory, bool as64, size_t addr, BitData value) {
    size_t writeSize = as64 ? sizeof(uint64_t) : sizeof(uint32_t);
    assertFatal(addr + writeSize <= MEMORY_SIZE, "Received out-of-bound read to memory!");

    uint8_t *ptr = (uint8_t *) memory + addr;
    for (size_t i = 0; i < writeSize; i++) {
        ptr[i] = (uint8_t)(value >> 8 * i);
    }
}
