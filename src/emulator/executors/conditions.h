///
/// conditions.h
/// Determines whether overflow or underflow has occurred as a result of an arithmetic operation
///
/// Created by Billy Highley on 06/06/2024.
///

#ifndef CONDITIONS_H
#define CONDITIONS_H

#include <stdbool.h>
#include <stdint.h>

bool overflow64(int64_t rn, int64_t op2, int64_t res);
bool overflow32(int32_t rn, int32_t op2, int32_t res);
bool underflow64(int64_t rn, int64_t op2, int64_t res);
bool underflow32(int32_t rn, int32_t op2, int32_t res);

#endif //CONDITIONS_H
