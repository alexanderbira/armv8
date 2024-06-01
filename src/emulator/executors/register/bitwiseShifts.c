///
/// bitwiseShifts.c
/// Performs a bitwise shift and returns the result.
///
/// Created by Billy Highley on 31/05/2024.
///

#include "bitwiseShifts.h"

/// Returns the op2 value calculated by shifting rm by operand many bits using the encoded shift type
/// @param shiftType The type of shift (LSL, LSR, ASR or ROR)
/// @param operand The 6-bit immediate operand (shift amount)
/// @param rm The register value encoded by rm
/// @param sf Whether to consider rm as a 64-bit or 32-bit value
/// @return The op2 value
static uint64_t bitShift(enum ShiftType shiftType, uint8_t operand, uint64_t rm, bool sf) {
    switch (shiftType) {
        case LSL:
            return (sf ? rm : (uint32_t) rm) << operand;
        case LSR:
            return (sf ? rm : (uint32_t) rm) >> operand;
        case ASR:
            return (sf ? (int64_t) rm : (int32_t) rm) >> operand;
        case ROR:
            return ((sf ? rm : (uint32_t) rm) >> operand) +
                   (rm << (sf ? (64 - operand) : (32 - operand)));
    }
}
