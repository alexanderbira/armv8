///
/// immediateHandler.h
/// Functions to parse from assembly and write as binary a Data Processing (Immediate) instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_IMMDIATE_HANDLER_H
#define ASSEMBLER_IMMDIATE_HANDLER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "instructionHandler.h"
#include "../../common/ir/imm.h"
#include "../../common/const.h"

Imm_IR handleImmediate(TokenisedLine line);

BitInst immToInst(Imm_IR ir);

#endif //ASSEMBLER_IMMDIATE_HANDLER_H
