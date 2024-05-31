///
/// singleDataTransferHandler.h
/// Functions to parse from assembly and write as binary a Single Data Transfer instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_SINGLE_DATA_TRANSFER_H
#define ASSEMBLER_SINGLE_DATA_TRANSFER_H

#include <string.h>

#include "../../common/error.h"
#include "../../common/ir/ir.h"
#include "../helpers.h"

IR parseSingleDataTransfer(TokenisedLine line, AssemblerState *state);

BitInst writeSingleDataTransfer(IR ir, AssemblerState *state);

#endif //ASSEMBLER_SDT_H
