///
/// bitwiseShifts.h
/// Performs a bitwise shift and returns the result.
///
/// Created by Billy Highley on 31/05/2024.
///

#ifndef EMULATOR_BITWISE_SHIFTS_H
#define EMULATOR_BITWISE_SHIFTS_H

#include <stdint.h>

#include "error.h"
#include "register.h"

uint64_t bitShift(enum ShiftType shiftType, uint8_t operand, uint64_t rm, bool sf);

#endif // EMULATOR_BITWISE_SHIFTS_H
