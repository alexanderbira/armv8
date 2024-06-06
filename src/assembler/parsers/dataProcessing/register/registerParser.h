///
/// registerParser.h
/// Transform a [TokenisedLine] to an [IR] of a Data Processing (Register) instruction.
///
/// Created by Jack Wong on 01/06/2024.
///

#ifndef ASSEMBLER_REGISTER_PARSER_H
#define ASSEMBLER_REGISTER_PARSER_H

#include <stdint.h>

#include "../../../../common/ir/ir.h"
#include "../../../helpers.h"
#include "../../../state.h"

IR parseRegister(TokenisedLine *line, unused AssemblerState *state);

#endif // ASSEMBLER_REGISTER_PARSER_H
