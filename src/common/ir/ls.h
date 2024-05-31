///
/// ls.h
/// The intermediate representation of a Load and Store instruction.
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#ifndef COMMON_LS_H
#define COMMON_LS_H

#include <stdbool.h>
#include <stdint.h>

/// The intermediate representation of a load and store instruction.
typedef struct {

    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// The type of the load and store instruction.
    enum Ls {

        SDT, ///< The single data transfer instruction type.
        LL   ///< The load literal instruction type.

    } ls;

    /// [19b] The constants for the load and store instruction group.
    union LsConsts {

        /// [19b] Single data transfer interpretation: constants struct.
        struct Sdt {

            /// [1b] Determines whether addressing mode is unsigned offset (1 = unsigned offset).
            bool u;

            /// [1b] Determines the type of data transfer (0 = store, 1 = load).
            bool l;

            /// The addressing mode.
            enum AddressingMode {

                /// Transfer address: \code Xn + uoffset \endcode
                UNSIGNED_OFFSET,

                /// Transfer address: \code Xn + simm9 \endcode
                /// Write-back: \code Xn + simm9 \endcode
                PRE_INDEXED,

                /// Transfer address: \code Xn \endcode
                /// Write-back: \code Xn + simm9 \endcode
                POST_INDEXED,

                /// Transfer address: \code Xn + Xm \endcode
                REGISTER_OFFSET

            } addressingMode;

            /// [12b] Interpretation of offset (depending on addressing mode).
            union Offset {

                /// [5b] The code for register Xm, used for the register offset addressing mode.
                uint8_t xm;

                /// [10b] The parameters for the pre/post indexed addressing mode.
                struct PrePostIndex {

                    /// [9b] The signed value used for the pre/post-indexed addressing mode.
                    int16_t simm9;

                    /// [1b] Determines whether to use pre/post-indexing (0 = post-indexed, 1 = pre-indexed).
                    bool i;

                } prePostIndex;

                /// [12b] 12-bit unsigned immediate constant for the unsigned offset addressing mode.
                uint16_t uoffset;

            } offset;

            /// [5b] The encoding of the base register.
            uint8_t xn;

        } sdt;

        /// [19b] Load literal interpretation: signed immediate value.
        int32_t simm19;

    } lsConsts;

    /// [5b] The encoding of the target register.
    uint8_t rt;

} LS_IR;

#endif //COMMON_LS_H