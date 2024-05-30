///
/// immAssem.h
/// Functions to parse from assembly and write as binary a Data Processing (Immediate) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_IMM_H
#define ASSEMBLER_IMM_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../common/ir/imm.h"
#include "../../common/const.h"

Imm_IR asmToImm(char *line);

BitInst immToInst(Imm_IR ir);

#endif //ASSEMBLER_IMM_H
