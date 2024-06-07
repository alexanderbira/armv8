///
/// branchExecutor.c
/// Execute a branch instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 31/05/2024.
///

#include "branchExecutor.h"

/// Executes an [IR] of a branch instruction.
/// @param irObject The instruction to execute.
/// @param registers The current virtual registers.
/// @param memory The current virtual memory.
void executeBranch(IR *irObject, Registers registers, unused Memory memory) {
    assertFatal(irObject->type == BRANCH,
                "Received non-immediate instruction!");
    Branch_IR *branchIR = &irObject->ir.branchIR;

    switch (branchIR->type) {
        case BRANCH_UNCONDITIONAL: {
            // Get the address offset
            int64_t simm26 = branchIR->data.simm26.data.immediate;
            int64_t offset = signExtend(simm26, 8 * sizeof(uint32_t));

            int64_t pcVal = getRegPC(registers);

            // Perform the jump
            setRegPC(registers, pcVal + 4 * offset);
            break;
        }

        case BRANCH_REGISTER: {
            // Jump to the address stored in Xn
            BitData jumpAddress = getReg(registers, branchIR->data.xn);
            setRegPC(registers, jumpAddress);
            break;
        }

        case BRANCH_CONDITIONAL: {
            // Get the address offset
            int64_t simm19 = branchIR->data.conditional.simm19.data.immediate;
            int64_t offset = signExtend(simm19, 8 * sizeof(uint32_t));

            int64_t pcVal = getRegPC(registers);

            switch (branchIR->data.conditional.condition) {
                case EQ:
                    if (getRegState(registers, Z)) {
                        setRegPC(registers, pcVal + 4 * offset);
                    }
                    break;

                case NE:
                    if (!getRegState(registers, Z)) {
                        setRegPC(registers, pcVal + 4 * offset);
                    }
                    break;

                case GE:
                    if (getRegState(registers, N) == getRegState(registers, V)) {
                        setRegPC(registers, pcVal + 4 * offset);
                    }
                    break;

                case LT:
                    if (getRegState(registers, N) != getRegState(registers, V)) {
                        setRegPC(registers, pcVal + 4 * offset);
                    }
                    break;

                case GT:
                    if (!getRegState(registers, Z)
                        && getRegState(registers, N) == getRegState(registers, V)) {
                        setRegPC(registers, pcVal + 4 * offset);
                    }
                    break;

                case LE:
                    if (!(!getRegState(registers, Z)
                          && getRegState(registers, N) == getRegState(registers, V))) {
                        setRegPC(registers, pcVal + 4 * offset);
                    }
                    break;

                case AL:
                    setRegPC(registers, pcVal + 4 * offset);
                    break;
            }
        }
    }
}
