///
/// loadStoreParser.h
/// Transform a [TokenisedLine] to an [IR] of a Load-Store instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_LOAD_STORE_PARSER_H
#define ASSEMBLER_LOAD_STORE_PARSER_H

#include <stdio.h>
#include <string.h>

#include "error.h"
#include "helpers.h"
#include "ir.h"
#include "state.h"

IR parseLoadStore(TokenisedLine *tokenisedLine, unused AssemblerState *state);

#endif // ASSEMBLER_LOAD_STORE_PARSER_H
