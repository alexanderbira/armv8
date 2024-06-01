///
/// branchExecuter.c
/// Execute a branch instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 31/05/2024.
///

#include "branchExecuter.h"

/// Execute a branch instruction from its intermediate representation
/// @param ir The IR of the instruction to execute
/// @param regs A pointer to the registers
/// @param mem A pointer to the memory
void executeBranch(IR *irObject, Registers regs, Memory mem) {
    assertFatal(irObject->type == IMMEDIATE,
                "[executeImmediate] Received non-immediate instruction!");
    Branch_IR *branchIR = &irObject->ir.branchIR;

    switch (branchIR->type) {
        case BRANCH_UNCONDITIONAL:
            {
                // Get the address offset
                uint64_t simm26 = branchIR->data.simm26.data.immediate;
                int64_t offset = (simm26 << 32) >> 32; // Sign-extend the address offset (64 - 32 = 32)

                int64_t pcVal = getRegPC(regs);

                // Perform the jump
                setRegPC(regs, pcVal + 4 * offset);
            }
            break;
        case BRANCH_REGISTER:
            {
                // Jump to the address stored in Xn
                BitData jumpAddress = getReg(regs, branchIR->data.xn);
                setRegPC(regs, jumpAddress);
            }
            break;
        case BRANCH_CONDITIONAL:
            {
                // Get the address offset
                uint64_t simm19 = branchIR->data.conditional.simm19.data.immediate;
                int64_t offset = (simm19 << 32) >> 32; // Sign-extend the address offset (64 - 32 = 32)

                int64_t pcVal = getRegPC(regs);

                switch (branchIR->data.conditional.condition) {
                    case EQ:
                        if (getRegState(regs, Z)) {
                            setRegPC(regs, pcVal + 4 * offset);
                        }
                        break;
                    case NE:
                        if (!getRegState(regs, Z)) {
                            setRegPC(regs, pcVal + 4 * offset);
                        }
                        break;
                    case GE:
                        if (getRegState(regs, N) == getRegState(regs, V)) {
                            setRegPC(regs, pcVal + 4 * offset);
                        }
                        break;
                    case LT:
                        if (getRegState(regs, N) != getRegState(regs, V)) {
                            setRegPC(regs, pcVal + 4 * offset);
                        }
                        break;
                    case GT:
                        if (
                            !getRegState(regs, Z) &&
                            getRegState(regs, N) == getRegState(regs, V)
                        ) {
                            setRegPC(regs, pcVal + 4 * offset);
                        }
                        break;
                    case LE:
                        if (
                            !(
                                !getRegState(regs, Z) &&
                                getRegState(regs, N) == getRegState(regs, V)
                            )
                        ) {
                            setRegPC(regs, pcVal + 4 * offset);
                        }
                        break;
                    case AL:
                        setRegPC(regs, pcVal + 4 * offset);
                }
            }
            break;
    }
}
