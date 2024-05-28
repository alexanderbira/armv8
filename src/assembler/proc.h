///
/// proc.h
/// Functions to turn assembly text into ARMv9 machine code.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_PROC_H
#define ASSEMBLER_PROC_H

#include <stdint.h>

uint32_t procEmpty(const char *line);

uint32_t procDirective(const char *line);

uint32_t procLabel(const char *line);

uint32_t procInst(const char *line);

#endif //ASSEMBLER_PROC_H
