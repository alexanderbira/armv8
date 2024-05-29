///
/// sdtAssem.h
/// Functions to parse from assembly and write as binary a Single Data Transfer instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_SDT_H
#define ASSEMBLER_SDT_H

#include "../../common/ir/sdt.h"
#include "../../common/const.h"

SDT_IR asmToSDT(char *line);

BitInst SDTToInst(SDT_IR ir);

#endif //ASSEMBLER_SDT_H
