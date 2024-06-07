///
/// register.c
/// Structure, manipulation, and allocation of the virtual register.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#include "registers.h"

/// Initialises a register to desired state at startup.
/// @param registers Pointer to the registers.
static void initRegs(Registers registers) {
    // All registers are initialised to zero. Zero register not in struct.
    for (int i = 0; i < NO_GPRS; i++) {
        registers->gprs[i] = 0;
    }
    registers->pc = 0;
    registers->sp = 0;

    // All flags are cleared on init except the zero-flag.
    registers->pstate = (PState) { false, true, false, false };
}

/// Creates fresh registers, properly initialised at startup.
/// @return Fresh registers.
Registers_s createRegs(void) {
    Registers_s regs;
    initRegs(&regs);
    return regs;
}

/// Gets the register X[id] as a 64-bit value.
/// @param registers Pointer to the registers.
/// @param id The ID of the register to access.
/// @return The value of the register, or 0 if [id] is out of range.
BitData getReg(Registers registers, size_t id) {
    if (id == ZERO_REGISTER) return 0; // Zero Register
    assertFatal(id < NO_GPRS, "Invalid register ID!");
    return registers->gprs[id];
}

/// Gets the program counter.
/// @param registers Pointer to the registers.
/// @return The value of the program counter.
BitData getRegPC(Registers registers) {
    return registers->pc;
}

/// Gets the stack pointer as a 64-bit value.
/// @param registers Pointer to the registers.
/// @return The value of the stack pointer.
BitData getRegSP(Registers registers) {
    return registers->sp;
}

/// Gets the requested PState flag.
/// @param registers Pointer to the registers.
/// @param field The field required.
/// @return The value of the PState flag [field].
bool getRegState(Registers registers, PStateField field) {
    switch (field) {
        case N:
            return registers->pstate.ng;
        case Z:
            return registers->pstate.zr;
        case C:
            return registers->pstate.cr;
        case V:
            return registers->pstate.ov;
        default:
            throwFatal("Invalid PState field!");
    }
}

/// Sets the value of a register; choice between 32 or 64-bit.
/// @param registers Pointer to the registers.
/// @param id The ID of the register to access.
/// @param as64 Whether or not to write to the 64-bit register, i.e., X[id].
/// @param value The value to write.
void setReg(Registers registers, size_t id, bool as64, BitData value) {
    if (id == ZERO_REGISTER) return;
    assertFatal(id < NO_GPRS, "Invalid register ID!");
    registers->gprs[id] = as64 ? value : (uint32_t) value;
}

/// Sets the value of the program counter.
/// @param registers Pointer to the registers.
/// @param value The value to write.
void setRegPC(Registers registers, BitData value) {
    registers->pc = value;
}

/// Increments the program counter by 4.
/// @param registers Pointer to the registers.
void incRegPC(Registers registers) {
    registers->pc += 0x4;
}

/// Sets the value of stack pointer from a 64-bit value.
/// @param registers Pointer to the registers.
/// @param as64 Whether or not to write to the 64-bit stack pointer, i.e., xsp.
/// @param value The value to write.
void setRegSP(Registers registers, bool as64, BitData value) {
    registers->sp = as64 ? value : (uint32_t) value;
}

/// Sets the value of a particular PState flags.
/// @param registers Pointer to the registers.
/// @param field The field required.
/// @param state The value to write.
void setRegState(Registers registers, PStateField field, bool state) {
    switch (field) {
        case N:
            registers->pstate.ng = state;
            break;
        case Z:
            registers->pstate.zr = state;
            break;
        case C:
            registers->pstate.cr = state;
            break;
        case V:
            registers->pstate.ov = state;
            break;
        default:
            throwFatal("Invalid PState field!");
    }
}

/// Sets the value of all PState flags.
/// @param registers Pointer to the registers.
/// @param state The states to write.
void setRegStates(Registers registers, PState state) {
    registers->pstate = state;
}
