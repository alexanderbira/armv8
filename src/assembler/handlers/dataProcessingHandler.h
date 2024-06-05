///
/// dataProcessingHandler.c
/// Functions to parse from assembly a Data Processing instruction.
///
/// Created by Jack Wong on 03/06/2024.
///

#ifndef ASSEMBLER_DATA_PROCESSING_HANDLER_H
#define ASSEMBLER_DATA_PROCESSING_HANDLER_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/ir/ir.h"
#include "../helpers.h"
#include "../state.h"
#include "immediateHandler.h"
#include "registerHandler.h"

IR parseDataProcessing(TokenisedLine *line, AssemblerState *state);

#endif // ASSEMBLER_DATA_PROCESSING_HANDLER_H
