///
/// adecl.c
/// Translates an assembly instruction into a human readable description
///
/// Created by Jack Wong on 19/06/24.
///

#include "adecl.h"

static char *adeclImmediate(Immediate_IR immediateIR);

static char*adeclRegister(Register_IR registerIr);

static char*adeclLoadStore(LoadStore_IR loadStoreIr);

static char*adeclBranch(Branch_IR branchIr);

/// Translates [irObject] to its human readable description.
/// @param irObject The instruction to interpret.
/// @returns The human readable description.
char *adecl(IR *irObject) {

    switch (irObject->type) {
        case IMMEDIATE: {
            return adeclImmediate(irObject->ir.immediateIR);
        }
        case REGISTER: {
            return adeclRegister(irObject->ir.registerIR);
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
    return "";
}

static char *adeclImmediate(Immediate_IR immediateIR) {
    char *str;
    char *nBits = immediateIR.sf ? "(64-bit)" : "(32-bit)";
    char *format;
    switch (immediateIR.opi) {
        case IMMEDIATE_ARITHMETIC: {
            switch (immediateIR.opc.arithmeticType) {
                // Rd := Rn + Op2
                case ADD:
                    format = "%s R%d = R%d + %d.";
                    break;

                    // Rd := Rn + Op2 (update flags)
                case ADDS:
                    format = "%s R%d = R%d + %d with flags.";
                    break;

                    // Rd := Rn - Op2
                case SUB:
                    format = "%s R%d = R%d - %d.";
                    break;

                    // Rd := Rn - Op2 (update flags)
                case SUBS:
                    format = "%s R%d = R%d - %d with flags.";
                    break;
            }
            asprintf(&str,
                     format,
                     nBits,
                     immediateIR.rd,
                     immediateIR.operand.arithmetic.rn,
                     immediateIR.operand.arithmetic.imm12 << 12 * immediateIR.operand.arithmetic.sh);
            break;
        }
        case IMMEDIATE_WIDE_MOVE: {
            switch (immediateIR.opc.wideMoveType) {
                // Rd := ~Op
                case MOVN:
                    format = "%s R%d = ~%d.";
                    break;

                // Rd := Op
                case MOVZ:
                    format = "%s R%d = %d.";
                    break;

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
                    return str;
                }
            }
            asprintf(&str,
                     format,
                     nBits,
                     immediateIR.rd,
                     immediateIR.operand.wideMove.imm16 << (16 * immediateIR.operand.wideMove.hw));
            break;
        }
    }
    return str;
}

static char*adeclRegister(Register_IR registerIr) {

}
