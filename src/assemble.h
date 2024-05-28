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

#include "assembler/classify.h"

/// The function signature of a line processor.
typedef uint32_t (*LineProcessor)(const char *line);

/// Functions corresponding to each [AsmType].
const LineProcessor procTable[] = {

};

#endif //ASSEMBLER_ASSEMBLE_H
