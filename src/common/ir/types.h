///
/// types.h
/// Common types to all types of instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <stdint.h>

/// An intermediate representation of a <literal>, which is either a signed immediate
/// or a label reference. This is needed since not all label references are backwards.
typedef union {
    char *label;
    int32_t immediate;
} Literal;

#endif //COMMON_TYPES_H
