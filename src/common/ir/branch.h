///
/// branch.h
/// The intermediate representation of a Branch instruction.
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>

/// The intermediate representation of a branch instruction.
typedef struct {

    /// The type of branch instruction.
    enum {

        /// Unconditional : branch to the address encoded by literal.
        /// \code PC := PC + offset \endcode
        B,

        /// Register: branch to the address in Xn.
        /// \code PC := Xn \endcode
        BR,

        /// Conditional: branch to literal when PSTATE satisfies cond.
        /// \code if cond, PC := PC + offset \endcode
        BCOND

    } branchType;

    /// The constants for the branch instruction.
    union {

        /// [26b] Used to encode the signed offset (simm26 * 4) to apply to the PC for the unconditional branch.
        int32_t simm26;

        /// [5b] The encoding of Xn, the register containing the address to jump to for the register branch.
        uint8_t xn;

        /// [23b] The constants for the conditional branch instruction.
        struct {

            /// [19b] Used to encode the signed offset (simm19 * 4) to apply to the PC for the conditional branch.
            int32_t simm19;

            /// [4b] The condition for the conditional branch instruction.
            enum {

                /// Equal.
                /// \code Z == 1 \endcode
                EQ = 0x0,

                /// Not equal.
                /// \code Z == 0 \endcode
                NE = 0x1,

                /// Signed greater or equal.
                /// \code N == V \endcode
                GE = 0xA,

                /// Signed less than.
                /// \code N != V \endcode
                LT = 0xB,

                /// Signed greater than.
                /// \code Z == 0 && N == V \endcode
                GT = 0xC,

                /// Signed less than or equal
                /// \code !(Z == 0 && N == V) \endcode
                LE = 0xD,

                /// Always.
                /// Any PSTATE flags.
                AL = 0xE

            } cond;

        } conditional;

    } branch;

} Branch_IR;

#endif //BRANCH_H
