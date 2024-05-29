///
/// regAssem.h
/// Functions to parse from assembly and write as binary a Data Processing (Register) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_REG_H
#define ASSEMBLER_REG_H

#include "../../common/ir/reg.h"
#include "../../common/const.h"

Reg_IR asmToReg(char *line);

BitInst regToInst(Reg_IR ir);

#endif //ASSEMBLER_REG_H
