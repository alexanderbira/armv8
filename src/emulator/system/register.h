///
/// register.h
/// Structure, manipulation, and allocation of the virtual register.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#ifndef EMULATOR_REGISTER_H
#define EMULATOR_REGISTER_H

#include <stdint.h>
#include <stdbool.h>

typedef struct PState PState;

typedef struct Register Register;

/// A struct representing the PSTATE fields of the processor.
struct PState {
    /// Negative condition flag.
    bool ng;

    /// Zero condition flag (last result was zero).
    bool zr;

    /// Carry condition flag (last result had carry-out set).
    bool cr;

    /// Overflow condition flag (last result overflowed).
    bool ov;
};

/// A struct representing, virtually, a machine's register contents.
struct Register {
    /// General purpose registers.
    uint64_t gprs[31];

    /// Zero register. Always returns zero, ignores writes.
    uint64_t zr;

    /// Program counter. Contains address of *current* instruction.
    uint64_t pc;

    /// Stack pointer.
    uint64_t sp;

    /// Program state register. Contains boolean flags.
    PState pstate;
};

Register createRegister(void);

void initRegister(Register* reg);

#endif //EMULATOR_REGISTER_H
