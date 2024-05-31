///
/// branchExecute.c
/// Execute a branch instruction from its intermediate representation (IR)
///
/// Created by Alexander Biraben-Renard on 31/05/2024.
///

#include "branchExecute.h"

/// Execute a branch instruction from its intermediate representation
/// @param ir The IR of the instruction to execute
/// @param regs A pointer to the registers
/// @param mem A pointer to the memory
void executeBranch(Branch_IR branchIr, Registers regs, Memory mem) {

    switch (branchIr.branchType) {

        case B:
            // Unconditional branch
            // code block to avoid a label followed by a declaration
            {
                // Get the address offset
                uint64_t simm26 = branchIr.branch.simm26;
                // Sign-extend the address offset (64 - 32 = 32)
                int64_t offset = (simm26 << 32) >> 32;

                int64_t pcVal = getRegPC(regs);

                // Perform the jump
                setRegPC(regs, pcVal + 4 * offset);
            }
            break;

        case BR:
            // Register branch
            // code block to avoid a label followed by a declaration
            {
                // Jump to the address stored in Xn
                BitData jumpAddress = getReg(regs, branchIr.branch.xn);
                setRegPC(regs, jumpAddress);
            }
            break;

        case BCOND:
            // Conditional branch
            // code block to avoid a label followed by a declaration
            {
                // Get the address offset
                uint64_t simm19 = branchIr.branch.conditional.simm19;
                // Sign-extend the address offset (64 - 32 = 32)
                int64_t offset = (simm19 << 32) >> 32;

                int64_t pcVal = getRegPC(regs);

                // Determine the condition and perform the jump if the condition
                // is satisfied
                switch (branchIr.branch.conditional.cond) {
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
