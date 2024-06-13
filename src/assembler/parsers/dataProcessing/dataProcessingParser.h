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

#include "error.h"
#include "helpers.h"
#include "immediateParser.h"
#include "ir.h"
#include "registerParser.h"
#include "state.h"

IR parseDataProcessing(TokenisedLine *line, unused AssemblerState *state);

#endif // ASSEMBLER_DATA_PROCESSING_PARSER_H
