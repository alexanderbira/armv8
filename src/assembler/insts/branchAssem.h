///
/// branchAssem.h
/// Functions to parse from assembly and write as binary a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_BRANCHASSEM_H
#define ASSEMBLER_BRANCHASSEM_H

#include <string.h>

#include "../../common/ir/branch.h"
#include "../../common/const.h"

Branch_IR asmToImm(char *line);

BitInst immToInst(Branch_IR ir);

#endif //ASSEMBLER_BRANCHASSEM_H
