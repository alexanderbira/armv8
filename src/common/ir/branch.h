///
/// branch.h
/// The intermediate representation of a Branch instruction.
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#ifndef IR_BRANCH_H
#define IR_BRANCH_H

#include <stdint.h>

#include "../const.h"
#include "types.h"

/// Baseline mask for a Branch (Unconditional) instruction.
#define BRANCH_UNCONDITIONAL_C        b(0001_0100_0000_0000_0000_0000_0000_0000)

/// Number of bits in [simm26] in a Branch (Unconditional) instruction.
#define BRANCH_UNCONDITIONAL_SIMM26_N 26

/// Baseline mask for a Branch (Register) instruction.
#define BRANCH_REGISTER_C             b(1101_0110_0001_1111_0000_0000_0000_0000)

/// Number of bits to shift for [xn] in a Branch (Register) instruction.
#define BRANCH_REGISTER_XN_S          5

/// Number of bits in [xn] in a Branch (Register) instruction.
#define BRANCH_REGISTER_XN_N          5

/// Baseline mask for a Branch (Conditional) instruction.
#define BRANCH_CONDITIONAL_C          b(0101_0100_0000_0000_0000_0000_0000_0000)

/// Number of bits to shift for [simm19] in a Branch (Conditional) instruction.
#define BRANCH_CONDITIONAL_SIMM19_S   5

/// Number of bits in [simm19] in a Branch (Conditional) instruction.
#define BRANCH_CONDITIONAL_SIMM19_N   19

/// Number of bits in [cond] in a Branch (Conditional) instruction.
#define BRANCH_CONDITIONAL_COND_N     4

/// Mask for a Branch (Unconditional) instruction.
#define BRANCH_UNCONDITIONAL_M        maskl(6)

/// Mask for a Branch (Register) instruction.
#define BRANCH_REGISTER_M             ((maskl(22)) | (maskr(5)))

/// Mask for a Branch (Unconditional) instruction.
#define BRANCH_CONDITIONAL_M          ((maskl(8)) | (mask(4, 4)))

/// Mask for [simm26] in a Branch (Unconditional) instruction.
#define BRANCH_UNCONDITIONAL_SIMM26_M maskr(26)

/// Mask for [xn] in a Branch (Register) instruction.
#define BRANCH_REGISTER_XN_M          mask(9, 5)

/// Mask for [simm26] in a Branch (Conditional) instruction.
#define BRANCH_CONDITIONAL_SIMM19_M   mask(23, 5)

/// Mask for [cond] in a Branch (Conditional) instruction.
#define BRANCH_CONDITIONAL_COND_M     maskr(4)

/// The intermediate representation of a branch instruction.
typedef struct {

    /// The type of branch instruction.
    enum BranchType {

        /// Unconditional: branch to the address encoded by [Literal].
        /// \code PC := PC + offset \endcode
        BRANCH_UNCONDITIONAL,

        /// Register: branch to the address in [xn].
        /// \code PC := Xn \endcode
        BRANCH_REGISTER,

        /// Conditional: branch to [Literal] when [PState] satisfies the given condition.
        /// \code (if condition) PC := PC + offset \endcode
        BRANCH_CONDITIONAL,

    } type;

    /// The constants for the branch instruction.
    union Branch {

        /// [26b] Used to encode the signed offset ([simm26] * 4) to apply to [pc] for the unconditional branch.
        Literal simm26;

        /// [5b] The encoding of the Xn register containing the address to jump to for the register branch.
        uint8_t xn;

        /// [23b] The constants for the conditional branch instruction.
        struct Conditional {

            /// [19b] Used to encode the signed offset ([simm19] * 4) to apply to [pc] for the conditional branch.
            Literal simm19;

            /// [4b] The condition for the conditional branch instruction.
            enum BranchCondition {

                /// Equal to.
                /// \code Z == 1 \endcode
                EQ = 0x0,

                /// Not equal to.
                /// \code Z == 0 \endcode
                NE = 0x1,

                /// Signed greater or equal to.
                /// \code N == V \endcode
                GE = 0xA,

                /// Signed less than.
                /// \code N != V \endcode
                LT = 0xB,

                /// Signed greater than.
                /// \code Z == 0 && N == V \endcode
                GT = 0xC,

                /// Signed less than or equal to.
                /// \code !(Z == 0 && N == V) \endcode
                LE = 0xD,

                /// Always branches (no condition).
                /// Any [PState] flags.
                AL = 0xE

            } condition;

        } conditional;

    } data;

} Branch_IR;

#endif // IR_BRANCH_H
