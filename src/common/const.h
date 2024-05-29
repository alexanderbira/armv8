///
/// const.h
/// Constants for the rest of the program.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#ifndef CONST_H
#define CONST_H

/// The virtual memory size of the emulated machine.
#define MEMORY_SIZE (2 << 20) // 2MB

/// The number of general purpose registers in the virtual machine.
#define NUM_GPRS 31

/// Alias for a chunk of data passed to and from the virtual registers or memory.
typedef uint64_t BitData;

#endif //CONST_H
