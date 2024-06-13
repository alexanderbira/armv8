///
/// directiveParser.h
/// Transform a [TokenisedLine] to an [IR] of a Directive.
///
/// Created by Lancelot Liu on 06/06/2024.
///

#ifndef ASSEMBLER_DIRECTIVE_PARSER_H
#define ASSEMBLER_DIRECTIVE_PARSER_H

#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "helpers.h"
#include "ir.h"
#include "state.h"

IR parseDirective(TokenisedLine *tokenisedLine, unused AssemblerState *state);

#endif // ASSEMBLER_DIRECTIVE_PARSER_H
