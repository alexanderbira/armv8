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
#include <stddef.h>

/// Alias for uint64_t, the size of a virtual X register.
typedef uint64_t Reg64;

/// Alias for uint32_t, the size of a virtual W register.
typedef uint32_t Reg32;

/// A struct representing the PSTATE fields of the processor.
typedef struct {
    /// Negative condition flag.
    bool ng;

    /// Zero condition flag (last result was zero).
    bool zr;

    /// Carry condition flag (last result had carry-out set).
    bool cr;

    /// Overflow condition flag (last result overflowed).
    bool ov;
} PState;

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
typedef struct {
    /// General purpose registers.
    Reg64 gprs[31];

    /// Zero register. Always returns zero, ignores writes.
    /// -- Not required since we will hardcode the value. --
    /// uint64_t zr;

    /// Program counter. Contains address of *current* instruction.
    Reg64 pc;

    /// Stack pointer.
    Reg64 sp;

    /// Program state register. Contains boolean flags.
    PState pstate;
} Registers;

Registers createReg(void);

Reg64 getReg64(Registers *reg, size_t id);

Reg32 getReg32(Registers *reg, size_t id);

Reg64 getRegPC(Registers *reg);

Reg64 getRegSP64(Registers *reg);

Reg32 getRegSP32(Registers *reg);

bool getRegState(Registers *reg, PStateField field);

void setReg64(Registers *reg, size_t id, Reg64 value);

void setReg32(Registers *reg, size_t id, Reg32 value);

void setRegPC(Registers *reg, Reg64 value);

void incRegPC(Registers *reg);

void setRegSP64(Registers *reg, Reg64 value);

void setRegSP32(Registers *reg, Reg32 value);

void setRegState(Registers *reg, PStateField field, bool state);

void setRegStates(Registers *reg, PState state);

#endif //EMULATOR_REGISTER_H
