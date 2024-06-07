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
uint64_t bitShift(enum ShiftType shiftType, uint8_t operand, uint64_t rm, bool sf) {
    uint64_t shifted;

    switch (shiftType) {
        case LSL:
            shifted = rm << operand;
            break;

        case LSR:
            shifted = rm >> operand;
            break;

        case ASR:
            shifted = (sf ? (int64_t) rm : (int32_t) rm) >> operand;
            break;

        case ROR:
            shifted =  rm >> operand;
            shifted += (rm << (sf ? (64 - operand) : (32 - operand)));
            break;

        default:
            throwFatal("[bitShift] Unrecognised shift type!");
    }

    return sf ? shifted : (uint32_t) shifted;
}
