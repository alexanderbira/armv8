///
/// conditions.c
/// Determines whether overflow or underflow has occurred as a result of an arithmetic operation
///
/// Created by Billy Highley on 06/06/2024.
///

#include "conditions.h"

/// Determines whether signed overflow has occurred when performing a 64-bit addition
/// @param rn The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the addition
/// @return Whether signed overflow has occurred
bool overflow64(int64_t rn, int64_t op2, int64_t res) {
    return (rn > 0 && op2 > 0 && res < 0) || (rn < 0 && op2 < 0 && res > 0);
}

/// Determines whether signed overflow has occurred when performing a 32-bit addition
/// @param rn The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the addition
/// @return Whether signed overflow has occurred
bool overflow32(int32_t rn, int32_t op2, int32_t res) {
    return (rn > 0 && op2 > 0 && res < 0) || (rn < 0 && op2 < 0 && res > 0);
}

/// Determines whether signed underflow has occurred when performing a 64-bit subtraction
/// @param rn The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the subtraction
/// @return Whether signed underflow has occurred
bool underflow64(int64_t rn, int64_t op2, int64_t res) {
    return (rn > 0 && op2 < 0 && res < 0) || (rn < 0 && op2 > 0 && res > 0);
}

/// Determines whether signed underflow has occurred when performing a 32-bit subtraction
/// @param rn The value of the source register
/// @param op2 The value of the second operand
/// @param res Result of the subtraction
/// @return Whether signed underflow has occurred
bool underflow32(int32_t rn, int32_t op2, int32_t res) {
    return (rn > 0 && op2 < 0 && res < 0) || (rn < 0 && op2 > 0 && res > 0);
}
