///
/// directiveParser.c
/// Transform a [TokenisedLine] to an [IR] of a Directive.
///
/// Created by Lancelot Liu on 06/06/2024.
///

#include "directiveParser.h"

/// Transform a [TokenisedLine] to an [IR] of a directive.
/// @param line The [TokenisedLine] of the directive.
/// @param state The current state of the assembler.
/// @returns The [IR] form of the directive.
/// @pre The [line] has [mnemonic] == NULL and [subMnemonic] present.
IR parseDirective(TokenisedLine *tokenisedLine, unused AssemblerState *state) {
    assertFatal(tokenisedLine->operandCount == 1,
                "Incorrect number of operands; directives instructions need 1!");

    if (!strcmp(tokenisedLine->subMnemonic, "int")) {
        // Reserve space for line, null terminator, and prepended '#'.
        char *immediateStr = (char *) malloc(strlen(tokenisedLine->operands[0]) + 2);
        assertFatalNotNull(immediateStr, "<Memory> Unable to allocate [char *]!");
        *immediateStr = '#';
        strcpy(immediateStr + 1, tokenisedLine->operands[0]);

        // Very cheesy trick to reuse [parseImmediateStr].
        BitData immediate = parseImmediateStr(immediateStr, 8 * sizeof(int32_t));
        free(immediateStr);
        return (IR) { .type = DIRECTIVE, .ir.memoryData = immediate };
    } else {
        // We do not handle any other directives.
        throwFatal("Invalid directive!");
    }
}
