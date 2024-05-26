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
    uint64_t gprs[31];

    /// Zero register. Always returns zero, ignores writes.
    uint64_t zr;

    /// Program counter. Contains address of *current* instruction.
    uint64_t pc;

    /// Stack pointer.
    uint64_t sp;

    /// Program state register. Contains boolean flags.
    PState pstate;
} Registers;

Registers createReg(void);

static void initReg(Registers *reg);

uint64_t getReg(Registers *reg, uint8_t id);

uint64_t getRegZR(Registers *reg);

uint64_t getRegPC(Registers *reg);

uint64_t getRegSP(Registers *reg);

bool getRegState(Registers *reg, PStateField field);

void setReg64(Registers *reg, uint8_t id, uint64_t value);

void setReg32(Registers *reg, uint8_t id, uint32_t value);

void setRegZR64(__attribute__((unused)) Registers *reg, __attribute__((unused)) uint64_t value);

void setRegZR32(Registers *reg, uint32_t value);

void setRegPC(Registers *reg, uint64_t value);

void incRegPC(Registers *reg);

void setRegSP64(Registers *reg, uint64_t value);

void setRegSP32(Registers *reg, uint32_t value);

void setRegState(Registers *reg, PStateField field, bool state);

void setRegStates(Registers *reg, PState state);

#endif //EMULATOR_REGISTER_H
