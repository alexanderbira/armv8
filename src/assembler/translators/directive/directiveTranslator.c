///
/// directiveTranslator.c
/// Transform a [IR] of a Directive to binary.
///
/// Created by Lancelot Liu on 06/06/2024.
///

#include "directiveTranslator.h"

/// Returns the immediate value from the IR form of a constant.
/// @param irObject The [IR] struct representing the immediate value.
/// @param state The current state of the assembler.
/// @return 32-bit binary word of the constant.
Instruction translateDirective(IR *irObject, unused AssemblerState *state) {
    assertFatal(irObject->type == DIRECTIVE, "[translateConstant] Received non-constant IR!");
    return irObject->ir.memoryData;
}
