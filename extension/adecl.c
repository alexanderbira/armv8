///
/// adecl.c
/// Translates an assembly instructoin into a human readable description
///
/// Created by Jack Wong on 19/06/24.
///

#include "adecl.h"

/// Translates [irObject] to its human readable description.
/// @param irObject The instruction to interpret.
/// @returns The human readable description.
char *adecl(IR *irObject) {
    char *str;

    switch (irObject->type) {
        case IMMEDIATE: {
            Immediate_IR immediateIR = irObject->ir.immediateIR;
            char *nBits = immediateIR.sf ? "(64-bit)" : "(32-bit)";
            switch (immediateIR.opi) {
                case IMMEDIATE_ARITHMETIC: {
                    switch (immediateIR.opc.arithmeticType) {
                        // Rd := Rn + Op2
                        case ADD: {
                            asprintf(&str,
                                     "%s R%d = R%d + %d.",
                                     nBits,
                                     immediateIR.rd,
                                     immediateIR.operand.arithmetic.rn,
                                     immediateIR.operand.arithmetic.imm12 << 12 * immediateIR.operand.arithmetic.sh);
                            break;
                        }
                        // Rd := Rn + Op2 (update flags)
                        case ADDS: {
                            asprintf(&str,
                                     "%s R%d = R%d + %d with flags.",
                                     nBits,
                                     immediateIR.rd,
                                     immediateIR.operand.arithmetic.rn,
                                     immediateIR.operand.arithmetic.imm12 << 12 * immediateIR.operand.arithmetic.sh);
                            break;
                        }
                        // Rd := Rn - Op2
                        case SUB: {
                            asprintf(&str,
                                     "%s R%d = R%d - %d.",
                                     nBits,
                                     immediateIR.rd,
                                     immediateIR.operand.arithmetic.rn,
                                     immediateIR.operand.arithmetic.imm12 << 12 * immediateIR.operand.arithmetic.sh);
                            break;
                        }
                        // Rd := Rn - Op2 (update flags)
                        case SUBS: {
                            asprintf(&str,
                                     "%s R%d = R%d - %d with flags.",
                                     nBits,
                                     immediateIR.rd,
                                     immediateIR.operand.arithmetic.rn,
                                     immediateIR.operand.arithmetic.imm12 << (12 * immediateIR.operand.arithmetic.sh));
                            break;
                        }
                    }
                    break;
                }
                case IMMEDIATE_WIDE_MOVE: {
                    switch (immediateIR.opc.wideMoveType) {
                        // Rd := ~Op
                        case MOVN: {
                            asprintf(&str,
                                     "%s R%d = ~%d.",
                                     nBits,
                                     immediateIR.rd,
                                     immediateIR.operand.wideMove.imm16 << (16 * immediateIR.operand.wideMove.hw));
                            break;
                        }
                        // Rd := Op
                        case MOVZ: {
                            asprintf(&str,
                                     "%s R%d = %d.",
                                     nBits,
                                     immediateIR.rd,
                                     immediateIR.operand.wideMove.imm16 << (16 * immediateIR.operand.wideMove.hw));
                            break;
                        }
                        // Rd[shift + 15:shift] := imm16
                        case MOVK: {
                            int shift = 16 * immediateIR.operand.wideMove.hw;
                            asprintf(&str,
                                     "%s R%d[%d:%d] = %d.",
                                     nBits,
                                     immediateIR.rd,
                                     shift + 15,
                                     shift,
                                     immediateIR.operand.wideMove.imm16);
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
        case REGISTER: {
            // TODO
            break;
        }
        case LOAD_STORE: {
            // TODO
            break;
        }
        case BRANCH: {
            // TODO
            break;
        }
        case DIRECTIVE: {
            // TODO
            break;
        }
    }
    return str;
}
