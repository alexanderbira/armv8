///
/// loadStoreParser.c
/// Transform a [TokenisedLine] to an [IR] of a Load-Store instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "loadStoreParser.h"

/// Transform a [TokenisedLine] to an [IR] of a load/store instruction.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @returns The [IR] form of the load/store instruction.
/// @pre The [line]'s mnemonic is that of a load/store instruction.
IR parseLoadStore(TokenisedLine *line, unused AssemblerState *state) {
    assertFatal(line->operandCount == 2 || line->operandCount == 3,
                "Incorrect number of operands; load-store instructions need 2 or 3!");
    LoadStore_IR loadStoreIR;

    bool sf;
    const uint8_t reg = parseRegisterStr(line->operands[0], &sf);

    if (line->operands[1][0] == '[') {
        // Single data transfer
        bool u = false;
        enum AddressingMode mode;
        union Offset offset;
        uint8_t xn;
        assertFatal(sscanf(line->operands[1], "[%*c%" SCNu8, &xn) == 1,
                    "Could not scan <xn>!");
        if (line->operandCount == 2) {
            // Zero Unsigned Offset
            u = true;
            mode = UNSIGNED_OFFSET;
            offset.uoffset = 0;
        } else {
            char *lastOperand = line->operands[2];

            switch (lastOperand[strlen(lastOperand) - 1]) {
                case '!':
                    // Pre-Index
                    mode = PRE_INDEXED;
                    offset.prePostIndex.i = true;
                    offset.prePostIndex.simm9 = parseImmediateStr(line->operands[2]);
                    break;

                case ']':
                    if (lastOperand[0] == '#') {
                        // Unsigned Offset
                        mode = UNSIGNED_OFFSET;
                        u = true;
                        offset.uoffset = parseImmediateStr(line->operands[2]);
                    } else {
                        //Register Offset
                        mode = REGISTER_OFFSET;
                        offset.xm = parseRegisterStr(line->operands[2], NULL);
                    }
                    break;

                default:
                    // Post-Index
                    mode = POST_INDEXED;
                    offset.prePostIndex.i = false;
                    offset.prePostIndex.simm9 = parseImmediateStr(line->operands[2]);
            }
        }

        loadStoreIR = (LoadStore_IR) {
            .sf = sf,
            .type = SINGLE_DATA_TRANSFER,
            .data.sdt = {
                .u = u,
                .l = !strcmp(line->mnemonic, "ldr"),
                .addressingMode = mode,
                .offset = offset,
                .xn = xn
            },
            .rt = reg
        };
    } else {
        // Load literal
        const Literal literal = parseLiteral(line->operands[1]);
        loadStoreIR = (LoadStore_IR) { sf, .type = LOAD_LITERAL, .data.simm19 = literal, .rt = reg };
    }

    return (IR) { .type = LOAD_STORE, .ir.loadStoreIR = loadStoreIR };
}
