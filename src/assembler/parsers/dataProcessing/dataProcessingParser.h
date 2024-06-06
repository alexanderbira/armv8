///
/// dataProcessingParser.h
/// Transform a [TokenisedLine] to an [IR] of a Data Processing instruction.
///
/// Created by Jack Wong on 03/06/2024.
///

#ifndef ASSEMBLER_DATA_PROCESSING_PARSER_H
#define ASSEMBLER_DATA_PROCESSING_PARSER_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "../../../common/ir/ir.h"
#include "../../helpers.h"
#include "../../state.h"
#include "immediateParser.h"
#include "registerParser.h"

IR parseDataProcessing(TokenisedLine *line, unused AssemblerState *state);

#endif // ASSEMBLER_DATA_PROCESSING_PARSER_H
