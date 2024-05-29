///
/// branch.h
/// The intermediate representation of a Branch instruction.
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>

/// The intermediate representation of a Branch instruction.
typedef struct {

    /// The type of branch instruction
    enum {
        B,    /// Branch to the address encoded by literal (PC := PC + offset)
        BR,   /// Branch to the address in Xn (PC := Xn)
        BCOND /// Branch to literal only when PSTATE satisfies cond (if cond, PC := PC + offset)
    } branchType;

    /// The constants for the branch instruction
    union {
        /// [26b] The 26-bit signed offset to apply to the PC for the unconditional branch is encoded as simm26 * 4.
        int32_t simm26;

        /// [5b] The encoding of Xn, the register containing the address to jump to for the register branch.
        uint8_t xn;

        /// [23b] The constants for the conditional branch instruction.
        struct {
            /// [19b] The offset to apply to the PC for the conditional branch is encoded as simm19 * 4.
            int32_t simm19;

            /// [4b] The condition for the conditional branch instruction.
            enum {
                EQ = 0x0, /// equal (Z == 1)
                NE = 0x1, /// not equal (Z == 0)
                GE = 0xA, /// signed greater or equal (N == V)
                LT = 0xB, /// signed less than (N != V)
                GT = 0xC, /// signed greater than (Z == 0 && N == V)
                LE = 0xD, /// signed less than or equal (!(Z == 0 && N == V))
                AL = 0xE  /// always any (pstate flag)
            } cond;

        } conditional;

    } branch;

} Branch_IR;

#endif //BRANCH_H
