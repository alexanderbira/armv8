///
/// register.c
/// Structure, manipulation, and allocation of the virtual register.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#include "register.h"

/// Creates a fresh register, properly initialised at startup.
/// @return A fresh register.
Register createReg() {
    Register reg;
    initReg(&reg);
    return reg;
};

/// Initialises a register to desired state at startup.
void initReg(Register* reg) {
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
uint64_t getReg(Register* reg, int id) {
    if (id < 1 || id > 31) return 0;
    return reg->gprs[id - 1];
}

/// Gets the zero register as a 64-bit value. Always returns 0.
/// @param reg Pointer to the register.
/// @return 0
uint64_t getRegZR(Register* reg) {
    return 0;
}

/// Gets the program counter.
/// @param reg Pointer to the register.
/// @return The value of the program counter.
uint64_t getRegPC(Register* reg) {
    return reg->pc;
}

/// Gets the stack pointer as a 64-bit value.
/// @param reg Pointer to the register.
/// @return The value of the stack pointer.
uint64_t getRegSP(Register* reg) {
    return reg->sp;
}

/// Gets the requested PState flag.
/// @param reg Pointer to the register.
/// @param field The field required.
/// @return The value of the PState flag [field].
bool getRegState(Register* reg, PStateField field) {
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
void setReg64(Register* reg, int id, uint64_t value) {
    if (id < 1 || id > 31) return;
    reg->gprs[id - 1] = value;
}

/// Sets the value of register W[id] from a 32-bit value. (I.e., the lower 32 bits of X[id].)
/// Will ignore [id] out of bounds.
/// @param reg Pointer to the register.
/// @param id The ID of the register to access.
/// @param value The value to write.
void setReg32(Register *reg, int id, uint32_t value) {
    if (id < 1 || id > 31) return;
    reg->gprs[id - 1] = value;
}

/// Sets the value of zero register from a 64-bit value. Always a no-op.
/// @param reg Pointer to the register.
/// @param value The value to write. Always ignored.
void setRegZR64(Register* reg, uint64_t value) {}

/// Sets the value of zero register from a 32-bit value. Always a no-op.
/// @param reg Pointer to the register.
/// @param value The value to write. Always ignored.
void setRegZR32(Register* reg, uint32_t value) {}

/// Sets the value of the program counter.
/// @param reg Pointer to the register.
/// @param value The value to write.
void setRegPC(Register* reg, uint64_t value) {
    reg->pc = value;
}

/// Increments the program counter by 4.
void incRegPC(Register* reg) {
    reg->pc += 0x4;
}

/// Sets the value of stack pointer from a 64-bit value.
/// @param reg Pointer to the register.
/// @param value The value to write.
void setRegSP64(Register* reg, uint64_t value) {
    reg->sp = value;
}

/// Sets the value of stack pointer from a 32-bit value.
/// @param reg Pointer to the register.
/// @param value The value to write.
void setRegSP32(Register* reg, uint32_t value) {
    reg->sp = value;
}

void setRegState(Register* reg, PStateField field, bool state) {
    switch (field) {
        case N: reg->pstate.ng = state; break;
        case Z: reg->pstate.zr = state; break;
        case C: reg->pstate.cr = state; break;
        case V: reg->pstate.ov = state; break;
    }
}

void setRegStates(Register* reg, PState state) {
    reg->pstate = state;
}
