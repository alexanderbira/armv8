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
                     "Load 0x%08lx",
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
                    format = "R%d = R%d + %d %s";
                    break;

                    // Rd := Rn + Op2 (update flags)
                case ADDS:
                    format = "R%d = R%d + %d %s w/ flags";
                    break;

                    // Rd := Rn - Op2
                case SUB:
                    format = "R%d = R%d - %d %s";
                    break;

                    // Rd := Rn - Op2 (update flags)
                case SUBS:
                    format = "R%d = R%d - %d %s w/ flags";
                    break;
            }

            asprintf(&str,
                     format,
                     immediateIR.rd,
                     immediateIR.operand.arithmetic.rn,
                     immediateIR.operand.arithmetic.imm12,
                     nBits);

            shiftVal = 12 * immediateIR.operand.arithmetic.sh;
            break;
        }
        case IMMEDIATE_WIDE_MOVE: {

            switch (immediateIR.opc.wideMoveType) {
                // Rd := ~Op
                case MOVN:
                    format = "R%d = ~%d %s";
                    break;

                // Rd := Op
                case MOVZ:
                    format = "R%d = %d %s";
                    break;

                // Rd[shift + 15:shift] := imm16
                case MOVK: {
                    int shift = 16 * immediateIR.operand.wideMove.hw;
                    asprintf(&str,
                             "R%d[%d:%d] = %d %s",
                             immediateIR.rd,
                             shift + 15,
                             shift,
                             immediateIR.operand.wideMove.imm16,
                             nBits);
                    return str;
                }
            }

            asprintf(&str,
                     format,
                     immediateIR.rd,
                     immediateIR.operand.wideMove.imm16,
                     nBits);

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
                    format = "R%d = R%d + R%d %s";
                    break;

                // Rd := Rn + Op2 (update flags)
                case ADDS:
                    format = "R%d = R%d + R%d %s w/ flags";
                    break;

                // Rd := Rn - Op2
                case SUB:
                    format = "R%d = R%d - R%d %s";
                    break;

                // Rd := Rn - Op2 (update flags)
                case SUBS:
                    format = "R%d = R%d - R%d %s w/ flags";
                    break;
            }
            break;

        case BIT_LOGIC:

            if (registerIr.negated) {
                switch (registerIr.opc.logic.negated) {
                    // Rd := Rn & ∼Op2
                    case BIC:
                        format = "R%d = R%d & ~R%d %s";
                        break;

                    // Rd := Rn | ∼Op2
                    case ORN:
                        format = "R%d = R%d | ~R%d %s";
                        break;

                    // Rd := Rn ∧ ∼Op2
                    case EON:
                        format = "R%d = R%d ∧ ∼R%d %s";
                        break;

                    // Rd := Rn & ∼Op2 (update condition flags)
                    case BICS:
                        format = "R%d = R%d & ∼R%d %s w/ flags";
                        break;
                }
            } else {
                switch (registerIr.opc.logic.standard) {
                    // Rd := Rn & Op2
                    case AND:
                        format = "R%d = R%d & R%d %s";
                        break;

                    // Rd := Rn | Op2
                    case ORR:
                        format = "R%d = R%d | R%d %s";
                        break;

                    // Rd := Rn ∧ Op2
                    case EOR:
                        format = "R%d = R%d ∧ R%d %s";
                        break;

                    // Rd := Rn & Op2 (update condition flags)
                    case ANDS:
                        format = "R%d = R%d & R%d %s w/ flags";
                        break;
                }
            }
            break;

        case MULTIPLY:

            switch (registerIr.opc.multiply) {
                // Rd := Ra + (Rn ∗ Rm)
                case MADD:
                    format = "R%d = R%d + (R%d * R%d) %s";
                    break;

                // Rd := Ra − (Rn ∗ Rm)
                case MSUB:
                    format = "R%d = R%d - (R%d * R%d) %s";
                    break;
            }

            asprintf(&str,
                     format,
                     registerIr.rd,
                     registerIr.operand.multiply.ra,
                     registerIr.rn,
                     registerIr.rm,
                     nBits);

            return str;
    }
    // Arithmetic or Big-logic.
    // Create the base string without shift.
    asprintf(&str,
             format,
             registerIr.rd,
             registerIr.rn,
             registerIr.rm,
             nBits);

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
                            ? "R%d = M[R%d + %d] %s"
                            : "M[R%d + %d] = R%d %s";
                    asprintf(&str,
                             format,
                             loadStoreIr.rt,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.uoffset,
                             nBits);
                    break;

                // Xn := Xn + simm9; Transfer Address: Xn + simm9
                case PRE_INDEXED:
                    format = loadStoreIr.data.sdt.l
                            ? "R%d = R%d + %d; R%d = M[R%d + %d] %s"
                            : "R%d = R%d + %d; M[R%d + %d] = R%d %s";
                    asprintf(&str,
                             format,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.prePostIndex.simm9,
                             loadStoreIr.rt,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.prePostIndex.simm9,
                             nBits);
                    break;

                // Transfer Address: Xn; Xn := Xn + simm9
                case POST_INDEXED:
                    format = loadStoreIr.data.sdt.l
                             ? "R%d = M[R%d]; R%d = R%d + %d %s"
                             : "M[R%d] = R%d; R%d = R%d + %d %s";
                    asprintf(&str,
                             format,
                             loadStoreIr.rt,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.prePostIndex.simm9,
                             nBits);
                    break;

                // Transfer Address: Xn + Xm
                case REGISTER_OFFSET:
                    format = loadStoreIr.data.sdt.l
                             ? "R%d = M[R%d + R%d] %s"
                             : "M[R%d + R%d] = R%d %s";
                    asprintf(&str,
                             format,
                             loadStoreIr.rt,
                             loadStoreIr.data.sdt.xn,
                             loadStoreIr.data.sdt.offset.xm,
                             nBits);
                    break;
            }
            break;

        // Transfer Address: PC + simm19 ∗ 4
        case LOAD_LITERAL:
            if (loadStoreIr.data.simm19.isLabel) {
                asprintf(&str,
                         "R%d = M[PC + 4 * %s] %s",
                         loadStoreIr.rt,
                         loadStoreIr.data.simm19.data.label,
                         nBits);
            } else {
                asprintf(&str,
                         "R%d = M[PC + 4 * %d] %s",
                         loadStoreIr.rt,
                         loadStoreIr.data.simm19.data.immediate,
                         nBits);
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
                         "Jump to %s",
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
                         "If %s, jump to %s",
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
