#ifndef ASSEMBLER_HANDLEASSEMBLY_H
#define ASSEMBLER_HANDLEASSEMBLY_H

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "assemblerDelegate.h"
#include "helpers.h"

void handleAssembly(char *assembly, AssemblerState *state);

#endif // ASSEMBLER_HANDLEASSEMBLY_H