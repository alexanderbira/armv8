///
/// assemble.h
/// Assembles an AArch64 assembly source file.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_ASSEMBLE_H
#define ASSEMBLER_ASSEMBLE_H

#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "assemblerDelegate.h"
#include "helpers.h"
#include "handleAssembly.h"

int main(int argc, char **argv);

void handleAssembly(char *assembly, AssemblerState *state);

bool JUMP_ON_ERROR = false;
jmp_buf fatalBuffer;
char *fatalError;

#endif // ASSEMBLER_ASSEMBLE_H
