///
/// assemble.h
/// The entrypoint to the assembler program.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_ASSEMBLE_H
#define ASSEMBLER_ASSEMBLE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "assembler/handler.h"
#include "assembler/proc.h"

int main(int argc, char **argv);

/// Functions corresponding to each [AsmType].
const LineProcessor procTable[4] = {
        procEmpty,
        procDirective,
        procLabel,
        procInst,
};

#endif //ASSEMBLER_ASSEMBLE_H
