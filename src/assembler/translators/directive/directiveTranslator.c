///
/// directiveTranslator.c
/// Transform a [IR] of a Directive to binary.
///
/// Created by Lancelot Liu on 06/06/2024.
///

#include "directiveTranslator.h"

/// Converts the IR form of a directive to a binary word.
/// @param irObject The [IR] struct representing the directive.
/// @param state The current state of the assembler.
/// @returns 32-bit binary word of the directive.
Instruction translateDirective(IR *irObject, unused AssemblerState *state) {
    assertFatal(irObject->type == DIRECTIVE, "Received non-directive IR!");
    return irObject->ir.memoryData;
}
