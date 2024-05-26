///
/// register.c
/// Structure, manipulation, and allocation of the virtual register.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#include <assert.h>
#include "registers.h"

/// Creates a fresh register, properly initialised at startup.
/// @return A fresh register.
Registers createReg() {
    Registers reg;
    initReg(&reg);
    return reg;
};

/// Initialises a register to desired state at startup.
/// @param reg Pointer to the register.
void initReg(Registers *reg) {
    // All registers are initialised to zero.
    for (int i = 0; i < 31; i++) {
        reg->gprs[i] = 0;
    }
    reg->zr = 0;
    reg->pc = 0;
    reg->sp = 0;

    // All flags are cleared on init except the zero-flag.
    reg->pstate = (PState){false, true, false, false};
}

/// Gets the register X[id] as a 64-bit value.
/// @param reg Pointer to the register.
/// @param id The ID of the register to access.
/// @return The value of the register, or 0 if [id] is out of range.
uint64_t getReg(Registers *reg, uint8_t id) {
    if (id < 1 || id > 31) return 0;
    return reg->gprs[id];
}

/// Gets the zero register as a 64-bit value. Always returns 0.
/// @param reg Pointer to the register.
/// @return 0
uint64_t getRegZR(__attribute__((unused)) Registers *reg) {
    return 0;
}

/// Gets the program counter.
/// @param reg Pointer to the register.
/// @return The value of the program counter.
uint64_t getRegPC(Registers *reg) {
    return reg->pc;
}

/// Gets the stack pointer as a 64-bit value.
/// @param reg Pointer to the register.
/// @return The value of the stack pointer.
uint64_t getRegSP(Registers *reg) {
    return reg->sp;
}

/// Gets the requested PState flag.
/// @param reg Pointer to the register.
/// @param field The field required.
/// @return The value of the PState flag [field].
bool getRegState(Registers *reg, PStateField field) {
    switch (field) {
        case N: return reg->pstate.ng;
        case Z: return reg->pstate.zr;
        case C: return reg->pstate.cr;
        case V: return reg->pstate.ov;
    }
}

/// Sets the value of register X[id] from a 64-bit value. Will ignore [id] out of bounds.
/// @param reg Pointer to the register.
/// @param id The ID of the register to access.
/// @param value The value to write.
void setReg64(Registers *reg, uint8_t id, uint64_t value) {
    assert(id < 31);
    reg->gprs[id] = value;
}

/// Sets the value of register W[id] from a 32-bit value. (I.e., the lower 32 bits of X[id].)
/// Will ignore [id] out of bounds.
/// @param reg Pointer to the register.
/// @param id The ID of the register to access.
/// @param value The value to write.
void setReg32(Registers *reg, uint8_t id, uint32_t value) {
    assert(id < 31);
    reg->gprs[id] = value;
}

/// Sets the value of zero register from a 64-bit value. Always a no-op.
/// @param reg Pointer to the register.
/// @param value The value to write. Always ignored.
void setRegZR64(__attribute__((unused)) Registers *reg, __attribute__((unused)) uint64_t value) {}

/// Sets the value of zero register from a 32-bit value. Always a no-op.
/// @param reg Pointer to the register.
/// @param value The value to write. Always ignored.
void setRegZR32(__attribute__((unused)) Registers *reg, __attribute__((unused)) uint32_t value) {}

/// Sets the value of the program counter.
/// @param reg Pointer to the register.
/// @param value The value to write.
void setRegPC(Registers *reg, uint64_t value) {
    reg->pc = value;
}

/// Increments the program counter by 4.
/// @param reg Pointer to the register.
void incRegPC(Registers *reg) {
    reg->pc += 0x4;
}

/// Sets the value of stack pointer from a 64-bit value.
/// @param reg Pointer to the register.
/// @param value The value to write.
void setRegSP64(Registers *reg, uint64_t value) {
    reg->sp = value;
}

/// Sets the value of stack pointer from a 32-bit value.
/// @param reg Pointer to the register.
/// @param value The value to write.
void setRegSP32(Registers *reg, uint32_t value) {
    reg->sp = value;
}

/// Sets the value of a particular PState flags.
/// @param reg Pointer to the register.
/// @param field The field required.
/// @param state The value to write.
void setRegState(Registers *reg, PStateField field, bool state) {
    switch (field) {
        case N: reg->pstate.ng = state; break;
        case Z: reg->pstate.zr = state; break;
        case C: reg->pstate.cr = state; break;
        case V: reg->pstate.ov = state; break;
    }
}

/// Sets the value of all PState flags.
/// @param reg Pointer to the register.
/// @param state The states to write.
void setRegStates(Registers *reg, PState state) {
    reg->pstate = state;
}
