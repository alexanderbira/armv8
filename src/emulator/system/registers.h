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

#include "../../defs/const.h"

/// Alias for uint64_t, the size of a virtual X register.
typedef uint64_t Reg;

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
    Reg gprs[NUM_GPRS];

    /// Zero register. Always returns zero, ignores writes.
    /// -- Not required since we will hardcode the value. --
    /// uint64_t zr;

    /// Program counter. Contains address of *current* instruction.
    Reg pc;

    /// Stack pointer.
    Reg sp;

    /// Program state register. Contains boolean flags.
    PState pstate;
} Regs;

Regs createReg(void);

Reg getReg(Regs *reg, size_t id);

Reg getRegPC(Regs *reg);

Reg getRegSP(Regs *reg);

bool getRegState(Regs *reg, PStateField field);

void setReg(Regs *reg, size_t id, bool as64, Reg value);

void setRegPC(Regs *reg, Reg value);

void incRegPC(Regs *reg);

void setRegSP(Regs *reg, bool as64, Reg value);

void setRegState(Regs *reg, PStateField field, bool state);

void setRegStates(Regs *reg, PState state);

#endif //EMULATOR_REGISTER_H
