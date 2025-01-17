///
/// immediateParser.h
/// Transform a [TokenisedLine] to an [IR] of a Data Processing (Immediate) instruction.
///
/// Created by Jack Wong on 29/05/2024.
///

#ifndef ASSEMBLER_IMMEDIATE_PARSER_H
#define ASSEMBLER_IMMEDIATE_PARSER_H

#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "helpers.h"
#include "ir.h"
#include "state.h"

IR parseImmediate(TokenisedLine *line, unused AssemblerState *state);

#endif // ASSEMBLER_IMMEDIATE_PARSER_H
