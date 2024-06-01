///
/// register.c
/// Structure, manipulation, and allocation of the virtual register.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#include <assert.h>
#include "registers.h"

/// Initialises a register to desired state at startup.
/// @param regs Pointer to the registers.
static void initRegs(Registers regs) {
    // All registers are initialised to zero. Zero register not in struct.
    for (int i = 0; i < NUM_GPRS; i++) {
        regs->gprs[i] = 0;
    }
    regs->pc = 0;
    regs->sp = 0;

    // All flags are cleared on init except the zero-flag.
    regs->pstate = (PState){false, true, false, false};
}

/// Creates fresh registers, properly initialised at startup.
/// @return Fresh registers.
Regs_s createRegs(void) {
    Regs_s regs;
    initRegs(&regs);
    return regs;
}

/// Gets the register X[id] as a 64-bit value.
/// @param regs Pointer to the registers.
/// @param id The ID of the register to access.
/// @return The value of the register, or 0 if [id] is out of range.
BitData getReg(Registers regs, size_t id) {
    if (id == 31) return 0; // Zero Register
    assert(id < NUM_GPRS);
    return regs->gprs[id];
}

/// Gets the program counter.
/// @param regs Pointer to the registers.
/// @return The value of the program counter.
BitData getRegPC(Registers regs) {
    return regs->pc;
}

/// Gets the stack pointer as a 64-bit value.
/// @param regs Pointer to the registers.
/// @return The value of the stack pointer.
BitData getRegSP(Registers regs) {
    return regs->sp;
}

/// Gets the requested PState flag.
/// @param regs Pointer to the registers.
/// @param field The field required.
/// @return The value of the PState flag [field].
bool getRegState(Registers regs, PStateField field) {
    switch (field) {
        case N: return regs->pstate.ng;
        case Z: return regs->pstate.zr;
        case C: return regs->pstate.cr;
        case V: return regs->pstate.ov;
        // TODO: Change after error handling protocol settled.
        default: return false;
    }
}

/// Sets the value of a register; choice between 32 or 64-bit.
/// @param regs Pointer to the registers.
/// @param id The ID of the register to access.
/// @param as64 Whether or not to write to the 64-bit register, i.e., X[id].
/// @param value The value to write.
void setReg(Registers regs, size_t id, bool as64, BitData value) {
    if (id == 31) return; // Zero Register
    assert(id < NUM_GPRS);
    regs->gprs[id] = as64 ? value : (uint32_t) value;
}

/// Sets the value of the program counter.
/// @param regs Pointer to the registers.
/// @param value The value to write.
void setRegPC(Registers regs, BitData value) {
    regs->pc = value;
}

/// Increments the program counter by 4.
/// @param regs Pointer to the registers.
void incRegPC(Registers regs) {
    regs->pc += 0x4;
}

/// Sets the value of stack pointer from a 64-bit value.
/// @param regs Pointer to the registers.
/// @param as64 Whether or not to write to the 64-bit stack pointer, i.e., xsp.
/// @param value The value to write.
void setRegSP(Registers regs, bool as64, BitData value) {
    regs->sp = as64 ? value : (uint32_t) value;
}

/// Sets the value of a particular PState flags.
/// @param regs Pointer to the registers.
/// @param field The field required.
/// @param state The value to write.
void setRegState(Registers regs, PStateField field, bool state) {
    switch (field) {
        case N: regs->pstate.ng = state; break;
        case Z: regs->pstate.zr = state; break;
        case C: regs->pstate.cr = state; break;
        case V: regs->pstate.ov = state; break;
    }
}

/// Sets the value of all PState flags.
/// @param regs Pointer to the registers.
/// @param state The states to write.
void setRegStates(Registers regs, PState state) {
    regs->pstate = state;
}
