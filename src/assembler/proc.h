///
/// proc.h
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_PROC_H
#define ASSEMBLER_PROC_H

#include <stdint.h>

#include "../common/const.h"

/// The function signature of a line processor.
typedef BitInst (*LineProcessor)(const char *line);

BitInst procEmpty(const char *line);

BitInst procDirective(const char *line);

BitInst procLabel(const char *line);

BitInst procInst(const char *line);

#endif //ASSEMBLER_PROC_H
