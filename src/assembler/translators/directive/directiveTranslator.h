///
/// directiveTranslator.h
/// Transform a [IR] of a Directive to binary.
///
/// Created by Lancelot Liu on 06/06/2024.
///

#ifndef ASSEMBLER_DIRECTIVE_TRANSLATOR_H
#define ASSEMBLER_DIRECTIVE_TRANSLATOR_H

#include "error.h"
#include "ir.h"
#include "state.h"

Instruction translateDirective(IR *irObject, unused AssemblerState *state);

#endif // ASSEMBLER_DIRECTIVE_TRANSLATOR_H
