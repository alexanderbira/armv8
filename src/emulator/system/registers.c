///
/// register.c
/// Structure, manipulation, and allocation of the virtual register.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#include <assert.h>
#include "registers.h"

/// Initialises a register to desired state at startup.
/// @param reg Pointer to the register.
static void initReg(Regs *reg) {
    // All registers are initialised to zero. Zero register not in struct.
    for (int i = 0; i < 31; i++) {
        reg->gprs[i] = 0;
    }
    reg->pc = 0;
    reg->sp = 0;

    // All flags are cleared on init except the zero-flag.
    reg->pstate = (PState){false, true, false, false};
}

/// Creates a fresh register, properly initialised at startup.
/// @return A fresh register.
Regs createReg(void) {
    Regs reg;
    initReg(&reg);
    return reg;
}

/// Gets the register X[id] as a 64-bit value.
/// @param reg Pointer to the register.
/// @param id The ID of the register to access.
/// @return The value of the register, or 0 if [id] is out of range.
Reg getReg(Regs *reg, size_t id) {
    assert(id < 31);
    return reg->gprs[id];
}

/// Gets the program counter.
/// @param reg Pointer to the register.
/// @return The value of the program counter.
Reg getRegPC(Regs *reg) {
    return reg->pc;
}

/// Gets the stack pointer as a 64-bit value.
/// @param reg Pointer to the register.
/// @return The value of the stack pointer.
Reg getRegSP(Regs *reg) {
    return reg->sp;
}

/// Gets the requested PState flag.
/// @param reg Pointer to the register.
/// @param field The field required.
/// @return The value of the PState flag [field].
bool getRegState(Regs *reg, PStateField field) {
    switch (field) {
        case N: return reg->pstate.ng;
        case Z: return reg->pstate.zr;
        case C: return reg->pstate.cr;
        case V: return reg->pstate.ov;
        // TODO: Change after error handling protocol settled.
        default: return false;
    }
}

/// Sets the value of a register; choice between 32 or 64-bit.
/// @param reg Pointer to the register.
/// @param id The ID of the register to access.
/// @param as64 Whether or not to write to the 64-bit register, i.e., X[id].
/// @param value The value to write.
void setReg(Regs *reg, size_t id, bool as64, Reg value) {
    assert(id < 31);
    reg->gprs[id] = as64 ? value : (uint32_t) value;
}

/// Sets the value of the program counter.
/// @param reg Pointer to the register.
/// @param value The value to write.
void setRegPC(Regs *reg, Reg value) {
    reg->pc = value;
}

/// Increments the program counter by 4.
/// @param reg Pointer to the register.
void incRegPC(Regs *reg) {
    reg->pc += 0x4;
}

/// Sets the value of stack pointer from a 64-bit value.
/// @param reg Pointer to the register.
/// @param as64 Whether or not to write to the 64-bit stack pointer, i.e., xsp.
/// @param value The value to write.
void setRegSP(Regs *reg, bool as64, Reg value) {
    reg->sp = as64 ? value : (uint32_t) value;
}

/// Sets the value of a particular PState flags.
/// @param reg Pointer to the register.
/// @param field The field required.
/// @param state The value to write.
void setRegState(Regs *reg, PStateField field, bool state) {
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
void setRegStates(Regs *reg, PState state) {
    reg->pstate = state;
}
