///
/// bitwiseShifts.h
/// Performs a bitwise shift and returns the result
///
/// Created by Billy Highley on 31/05/2024.
///

#ifndef BITWISESHIFTS_H
#define BITWISESHIFTS_H

#include "../../../common/ir/register.h"

uint64_t bitShift(enum ShiftType shiftType, uint8_t operand, uint64_t rm, bool sf);

#endif //BITWISESHIFTS_H
