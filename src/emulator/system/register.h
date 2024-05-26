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

/// An enum representing the PSTATE fields of the processor.
typedef enum {
    /// Marker for Negative condition flag.
    N,

    /// Marker for Zero condition flag (last result was zero).
    Z,

    /// Marker for Carry condition flag (last result had carry-out set).
    C,

    /// Marker for oVerflow condition flag (last result overflowed).
    V
} PStateField;

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

Register createReg(void);

void initReg(Register* reg);

uint64_t getReg(Register* reg, int id);

uint64_t getRegZR(Register* reg);

uint64_t getRegPC(Register* reg);

uint64_t getRegSP(Register* reg);

bool getRegState(Register* reg, PStateField field);

void setReg64(Register* reg, int id, uint64_t value);

void setReg32(Register* reg, int id, uint32_t value);

void setRegZR64(Register* reg, uint64_t value);

void setRegZR32(Register* reg, uint32_t value);

void setRegPC(Register* reg, uint64_t value);

void incRegPC(Register* reg);

void setRegSP64(Register* reg, uint64_t value);

void setRegSP32(Register* reg, uint32_t value);

void setRegState(Register* reg, PStateField field, bool state);

void setRegStates(Register* reg, PState state);

#endif //EMULATOR_REGISTER_H
