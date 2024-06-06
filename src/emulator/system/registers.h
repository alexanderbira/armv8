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

#include "../../common/const.h"
#include "../../common/error.h"

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
    BitData gprs[NO_GPRS];

    /// Zero register. Always returns zero, ignores writes.
    /// -- Not required since we will hardcode the value. --
    /// uint64_t zr;

    /// Program counter. Contains address of *current* instruction.
    BitData pc;

    /// Stack pointer.
    BitData sp;

    /// Program state register. Contains boolean flags.
    PState pstate;
} Registers_s;

/// Type definition representing a pointer to the registers struct.
typedef Registers_s *Registers;

Registers_s createRegs(void);

BitData getReg(Registers regs, size_t id);

BitData getRegPC(Registers regs);

BitData getRegSP(Registers regs);

bool getRegState(Registers regs, PStateField field);

void setReg(Registers regs, size_t id, bool as64, BitData value);

void setRegPC(Registers regs, BitData value);

void incRegPC(Registers regs);

void setRegSP(Registers regs, bool as64, BitData value);

void setRegState(Registers regs, PStateField field, bool state);

void setRegStates(Registers regs, PState state);

#endif // EMULATOR_REGISTER_H
