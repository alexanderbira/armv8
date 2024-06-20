///
/// adecl.c
/// Translates an assembly instruction into a human readable description
///
/// Created by Jack Wong on 19/06/24.
///

#include "adecl.h"

static char *adeclImmediate(Immediate_IR immediateIR);

static char *adeclRegister(Register_IR registerIr);

static char *adeclLoadStore(LoadStore_IR loadStoreIr);

static char *adeclBranch(Branch_IR branchIr);

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
            return adeclLoadStore(irObject->ir.loadStoreIR);
        }
        case BRANCH: {
            return adeclBranch(irObject->ir.branchIR);
        }
        case DIRECTIVE: {
            char *str;
            asprintf(&str,
                     "Load 0x%08llx",
                     irObject->ir.memoryData);
            return str;
        }
    }
    return "";
}

static char *applyShiftFormat(char *str, enum ShiftType shift, int shiftVal);

static char *getBranchCondition(enum BranchCondition condition);

/// Translates [immediateIR] to its human readable description.
/// @param immediateIR The immediate instruction to interpret.
/// @returns The human readable description.
static char *adeclImmediate(Immediate_IR immediateIR) {
    char *str;
    char *nBits = immediateIR.sf ? "(64b)" : "(32b)";
    char *format;
    int shiftVal;

    switch (immediateIR.opi) {
        case IMMEDIATE_ARITHMETIC: {

            switch (immediateIR.opc.arithmeticType) {
                // Rd := Rn + Op2
                case ADD:
                    format = "%s R%d = R%d + %d";
                    break;

                    // Rd := Rn + Op2 (update flags)
                case ADDS:
                    format = "%s R%d = R%d + %d w/ flags";
                    break;

                    // Rd := Rn - Op2
                case SUB:
                    format = "%s R%d = R%d - %d";
                    break;

                    // Rd := Rn - Op2 (update flags)
                case SUBS:
                    format = "%s R%d = R%d - %d w/ flags";
                    break;
            }

            asprintf(&str,
                     format,
                     nBits,
                     immediateIR.rd,
                     immediateIR.operand.arithmetic.rn,
                     immediateIR.operand.arithmetic.imm12);

            shiftVal = 12 * immediateIR.operand.arithmetic.sh;
            break;
        }
        case IMMEDIATE_WIDE_MOVE: {

            switch (immediateIR.opc.wideMoveType) {
                // Rd := ~Op
                case MOVN:
                    format = "%s R%d = ~%d";
                    break;

                // Rd := Op
                case MOVZ:
                    format = "%s R%d = %d";
                    break;

                // Rd[shift + 15:shift] := imm16
                case MOVK: {
                    int shift = 16 * immediateIR.operand.wideMove.hw;
                    asprintf(&str,
                             "%s R%d[%d:%d] = %d",
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
                     immediateIR.operand.wideMove.imm16);

            shiftVal = 16 * immediateIR.operand.wideMove.hw;
            break;
        }
    }
    return applyShiftFormat(str, LSL, shiftVal);
}

/// Translates [registerIR] to its human readable description.
/// @param registerIr The register instruction to interpret.
/// @returns The human readable description.
static char *adeclRegister(Register_IR registerIr) {
    char *str;
    char *nBits = registerIr.sf ? "(64b)" : "(32b)";
    char *format;

    switch (registerIr.group) {
        case ARITHMETIC:

            switch (registerIr.opc.arithmetic) {
                // Rd := Rn + Op2
                case ADD:
                    format = "%s R%d = R%d + R%d";
                    break;

                // Rd := Rn + Op2 (update flags)
                case ADDS:
                    format = "%s R%d = R%d + R%d w/ flags";
                    break;

                // Rd := Rn - Op2
                case SUB:
                    format = "%s R%d = R%d - R%d";
                    break;

                // Rd := Rn - Op2 (update flags)
                case SUBS:
                    format = "%s R%d = R%d - R%d w/ flags";
                    break;
            }
            break;

        case BIT_LOGIC:

            if (registerIr.negated) {
                switch (registerIr.opc.logic.negated) {
                    // Rd := Rn & ∼Op2
                    case BIC:
                        format = "%s R%d = R%d & ~R%d";
                        break;

                    // Rd := Rn | ∼Op2
                    case ORN:
                        format = "%s R%d = R%d | ~R%d";
                        break;

                    // Rd := Rn ∧ ∼Op2
                    case EON:
                        format = "%s R%d = R%d ∧ ∼R%d";
                        break;

                    // Rd := Rn & ∼Op2 (update condition flags)
                    case BICS:
                        format = "%s R%d = R%d & ∼R%d w/ flags";
                        break;
                }
            } else {
                switch (registerIr.opc.logic.standard) {
                    // Rd := Rn & Op2
                    case AND:
                        format = "%s R%d = R%d & R%d";
                        break;

                    // Rd := Rn | Op2
                    case ORR:
                        format = "%s R%d = R%d | R%d";
                        break;

                    // Rd := Rn ∧ Op2
                    case EOR:
                        format = "%s R%d = R%d ∧ R%d";
                        break;

                    // Rd := Rn & Op2 (update condition flags)
                    case ANDS:
                        format = "%s R%d = R%d & R%d w/ flags";
                        break;
                }
            }
            break;

        case MULTIPLY:

            switch (registerIr.opc.multiply) {
                // Rd := Ra + (Rn ∗ Rm)
                case MADD:
                    format = "%s R%d = R%d + (R%d * R%d)";
                    break;

                // Rd := Ra − (Rn ∗ Rm)
                case MSUB:
                    format = "%s R%d = R%d - (R%d * R%d)";
                    break;
            }

            asprintf(&str,
                     format,
                     nBits,
                     registerIr.rd,
                     registerIr.operand.multiply.ra,
                     registerIr.rn,
                     registerIr.rm);

            return str;
    }
    // Arithmetic or Big-logic.
    // Create the base string without shift.
    asprintf(&str,
             format,
             nBits,
             registerIr.rd,
             registerIr.rn,
             registerIr.rm);

    // Add in shift if applicable.
    return applyShiftFormat(str, registerIr.shift, registerIr.operand.imm6);
}

/// Translates [loadStoreIr] to its human readable description.
/// @param loadStoreIr The load-store instruction to interpret.
/// @returns The human readable description.
static char *adeclLoadStore(LoadStore_IR loadStoreIr) {
    char *str;
    char *nBits = loadStoreIr.sf ? "(64b)" : "(32b)";
    char *format;

    switch (loadStoreIr.type) {

        case SINGLE_DATA_TRANSFER:

            switch (loadStoreIr.data.sdt.addressingMode) {

                // Transfer Address: Xn + uoffset
                case UNSIGNED_OFFSET:
                    format = loadStoreIr.data.sdt.l
                            ? "%s R%d = M[R%d + %d]"
                            : "%s M[R%d + %d] = R%d";
                    asprintf(&str,
                             format,
                             nBits,
                             loadStoreIr.rt,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.uoffset);
                    break;

                // Xn := Xn + simm9; Transfer Address: Xn + simm9
                case PRE_INDEXED:
                    format = loadStoreIr.data.sdt.l
                            ? "%s R%d = R%d + %d; R%d = M[R%d + %d]"
                            : "%s R%d = R%d + %d; M[R%d + %d] = R%d";
                    asprintf(&str,
                             format,
                             nBits,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.prePostIndex.simm9,
                             loadStoreIr.rt,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.prePostIndex.simm9);
                    break;

                // Transfer Address: Xn; Xn := Xn + simm9
                case POST_INDEXED:
                    format = loadStoreIr.data.sdt.l
                             ? "%s R%d = M[R%d]; R%d = R%d + %d"
                             : "%s M[R%d] = R%d; R%d = R%d + %d";
                    asprintf(&str,
                             format,
                             nBits,
                             loadStoreIr.rt,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.prePostIndex.simm9);
                    break;

                // Transfer Address: Xn + Xm
                case REGISTER_OFFSET:
                    format = loadStoreIr.data.sdt.l
                             ? "%s R%d = M[R%d + R%d]"
                             : "%s M[R%d + R%d] = R%d]";
                    asprintf(&str,
                             format,
                             nBits,
                             loadStoreIr.rt,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.xm);
                    break;
            }
            break;

        // Transfer Address: PC + simm19 ∗ 4
        case LOAD_LITERAL:
            if (loadStoreIr.data.simm19.isLabel) {
                asprintf(&str,
                         "%s R%d = M[PC + 4 * '%s']",
                         nBits,
                         loadStoreIr.rt,
                         loadStoreIr.data.simm19.data.label);
            } else {
                asprintf(&str,
                         "%s R%d = M[PC + 4 * %d]",
                         nBits,
                         loadStoreIr.rt,
                         loadStoreIr.data.simm19.data.immediate);
            }
            break;
    }
    return str;
}

/// Translates [branchIr] to its human readable description.
/// @param branchIr The branch instruction to interpret.
/// @returns The human readable description.
static char *adeclBranch(Branch_IR branchIr) {
    char *str;

    switch (branchIr.type) {

        // PC := PC + offset
        case BRANCH_UNCONDITIONAL:
            if (branchIr.data.simm26.isLabel) {
                asprintf(&str,
                         "Jump to '%s'",
                         branchIr.data.simm26.data.label);
            } else {
                asprintf(&str,
                         "Jump %d lines",
                         branchIr.data.simm26.data.immediate);
            }
            break;

        // PC := Xn
        case BRANCH_REGISTER:
            asprintf(&str,
                     "Jump to address in R%d",
                     branchIr.data.xn);
            break;

        // If cond, PC := PC + offset
        case BRANCH_CONDITIONAL:
            if (branchIr.data.conditional.simm19.isLabel) {
                asprintf(&str,
                         "If %s, jump to '%s'",
                         getBranchCondition(branchIr.data.conditional.condition),
                         branchIr.data.conditional.simm19.data.label);
            } else {
                asprintf(&str,
                         "If %s, jump %d lines",
                         getBranchCondition(branchIr.data.conditional.condition),
                         branchIr.data.conditional.simm19.data.immediate);
            }
            break;
    }
    return str;
}

/// Adds the shift description to the human readable description.
/// @param str The human readable description.
/// @param shift The shift stored in the IR.
/// @param shiftVal The amount to shift.
/// @return The human readable description with shift description.
static char *applyShiftFormat(char *str, enum ShiftType shift, int shiftVal) {
    char *newStr;
    char *shiftType;

    // No shift.
    if (shiftVal == 0) return str;

    switch (shift) {

        case LSL:
            shiftType = "LSL";
            break;

        case LSR:
            shiftType = "LSR";
            break;

        case ASR:
            shiftType = "ASR";
            break;

        case ROR:
            shiftType = "ROR";
            break;
    }

    // Check if "/w" already in string and act accordingly.
    if (strstr(str, "w/") != NULL) {
        asprintf(&newStr,
                 "%s, %s %d",
                 str,
                 shiftType,
                 shiftVal);
    } else {
        asprintf(&newStr,
                 "%s w/ %s %d",
                 str,
                 shiftType,
                 shiftVal);
    }
    free(str);
    return newStr;
}

/// Converts the branch condition to a human readable description.
/// @param condition The branch condition stored in the [Branch_IR].
/// @return The human readable description of the branch condition.
static char *getBranchCondition(enum BranchCondition condition) {

    switch (condition) {

        // Equal to.
        // Z == 1
        case EQ:
            return "==";

        // Not equal to.
        // Z == 0
        case NE:
            return "!=";

        // Signed greater or equal to.
        // N == V
        case GE:
            return "signed >=";

        // Signed less than.
        // N != V
        case LT:
            return "signed <";

        // Signed greater than.
        // Z == 0 && N == V
        case GT:
            return "signed >";

        // Signed less than or equal to.
        // !(Z == 0 && N == V)
        case LE:
            return "signed <=";

        // Always branches (no condition).
        // Any [PState] flags.
        case AL:
            return "always";
    }
    return "";
}
