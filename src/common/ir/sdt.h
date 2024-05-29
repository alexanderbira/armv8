///
/// sdt.h
/// The intermediate representation of a Single Data Transfer instruction.
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#ifndef COMMON_SDT_H
#define COMMON_SDT_H

#include <stdbool.h>
#include <stdint.h>

/// The intermediate representation of a single data transfer instruction.
typedef struct {

    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// The type of the single data transfer instruction group instruction.
    enum {

        SDT, ///< The single data transfer instruction type.
        LL   ///< The load literal instruction type.

    } sdtType;

    /// [19b] The parameters for the single data transfer instruction group.
    union {

        /// [19b] Single data transfer interpretation: constants struct.
        struct {

            /// [1b] Determines whether addressing mode is unsigned offset (1 = unsigned offset).
            bool u;

            /// [1b] Determines the type of data transfer (0 = store, 1 = load).
            bool l;

            /// The addressing mode.
            enum {

                /// Transfer address: \code Xn + uoffset \endcode
                REGISTER_OFFSET,

                /// Transfer address: \code Xn + simm9 \endcode
                /// Write-back: \code Xn + simm9 \endcode
                PRE_INDEXED,

                /// Transfer address: \code Xn \endcode
                /// Write-back: \code Xn + simm9 \endcode
                POST_INDEXED,

                /// Transfer address: \code Xn + Xm \endcode
                UNSIGNED_OFFSET

            } addressingMode;

            /// [12b] Interpretation of offset (depending on addressing mode).
            union {

                /// [5b] The code for register Xm, used for the register offset addressing mode.
                uint8_t xm;

                /// [10b] The parameters for the pre/post indexed addressing mode.
                struct {

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

        } sdtType;

        /// [19b] Load literal interpretation: signed immediate value.
        int32_t simm19;

    } sdtGroup;

    /// [5b] The encoding of the target register.
    uint8_t rt;

} SDT_IR;

#endif //COMMON_SDT_H
