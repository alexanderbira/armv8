///
/// loadStore.h
/// The intermediate representation of a Load and Store instruction.
///
/// Created by Alexander Biraben-Renard on 29/05/2024.
///

#ifndef IR_LOAD_STORE_H
#define IR_LOAD_STORE_H

#include <stdbool.h>
#include <stdint.h>

/// Baseline mask for a Single Data Transfer (Load) instruction.
#define SINGLE_DATA_TRANSFER_LOAD             b(1011_1000_0000_0000_0000_0000_0000_0000)

/// Number of bits to shift for [U] in a Single Data Transfer (Load) instruction.
#define SINGLE_DATA_TRANSFER_LOAD_U_S         24

/// Number of bits to shift for [L] in a Single Data Transfer (Load) instruction.
#define SINGLE_DATA_TRANSFER_LOAD_L_S         22

/// Number of bits to shift for [offset] in a Single Data Transfer (Load) instruction.
#define SINGLE_DATA_TRANSFER_LOAD_OFFSET_S    10

/// Number of bits in [offset] in a Single Data Transfer (Load) instruction.
#define SINGLE_DATA_TRANSFER_LOAD_OFFSET_N    12

/// Number of bits to shift for [xn] in a Single Data Transfer (Load) instruction.
#define SINGLE_DATA_TRANSFER_LOAD_XN_S        5

/// Number of bits in [xn] in a Single Data Transfer (Load) instruction.
#define SINGLE_DATA_TRANSFER_LOAD_XN_N        5

/// Baseline mask for a Single Data Transfer (Literal) instruction.
#define SINGLE_DATA_TRANSFER_LITERAL          b(1101_0110_0001_1111_0000_0000_0000_0000)

/// Number of bits to shift for [simm19] in a Single Data Transfer (Literal) instruction.
#define SINGLE_DATA_TRANSFER_LITERAL_SIMM19_S 5

/// Number of bits in [simm19] in a Single Data Transfer (Literal) instruction.
#define SINGLE_DATA_TRANSFER_LITERAL_SIMM19_N 19

/// Number of bits to shift for [sf] in a Single Data Transfer (Load / Literal) instruction.
#define SINGLE_DATA_TRANSFER_SF_S             30

/// Number of bits in [rt] in a Single Data Transfer (Load) instruction.
#define SINGLE_DATA_TRANSFER_RT_N             5

/// The intermediate representation of a load and store instruction.
typedef struct {

    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// The type of the load and store instruction.
    enum LoadStoreType {
        SINGLE_DATA_TRANSFER, ///< The single data transfer instruction type.
        LOAD_LITERAL,         ///< The load literal instruction type.
    } type;

    /// [19b] The constants for the load and store instruction group.
    union LoadStoreConstants {

        /// [19b] Single data transfer interpretation: constants struct.
        struct SingleDataTransfer {

            /// [1b] Determines whether addressing mode is unsigned offset (1 = unsigned offset).
            bool u;

            /// [1b] Determines the type of data transfer (0 = store, 1 = load).
            bool l;

            /// The addressing mode.
            enum AddressingMode {
                /// In the form of \code [xn] \endcode
                /// Transfer address: \code Xn \endcode
                ZERO_UNSIGNED_OFFSET,

                /// In the form of \code [xn, #<imm>] \endcode
                /// Transfer address: \code Xn + imm \endcode
                UNSIGNED_OFFSET,

                /// In the form of \code [xn, #<simm>]! \endcode
                /// Transfer address: \code Xn + simm \endcode
                /// Write-back: \code Xn := Xn + simm \endcode
                PRE_INDEXED,

                /// In the form of \code [xn], #<simm> \encode
                /// Transfer address: \code Xn \endcode
                /// Write-back: \code Xn + simm \endcode
                POST_INDEXED,

                /// In the form of \code [xn, xm] \endcode
                /// Transfer address: \code Xn + Xm \endcode
                REGISTER_OFFSET,

                /// In the form of \code <literal> \endcode
                LITERAL,
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
        Literal simm19;

    } data;

    /// [5b] The encoding of the target register.
    uint8_t rt;

} LoadStore_IR;

#endif // IR_LOAD_STORE_H
