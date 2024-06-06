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

/// Baseline mask for a Load Store (Literal) instruction.
#define LOAD_STORE_DATA                   b(1011_1000_0000_0000_0000_0000_0000_0000)

/// Baseline mask for a Register Offset-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_OFFSET_REGISTER   b(0000_0000_0010_0000_0110_1000_0000_0000)

/// Baseline mask for a Register Offset-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_PRE_POST_INDEX    b(0000_0000_0000_0000_0000_0100_0000_0000)

/// Mask for a Load Store (Literal) instruction.
#define LOAD_STORE_DATA_M                 ((maskl(1)) | (mask(29, 25)) | (mask(23, 23)))

/// Number of bits to shift for [U] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_U_S               24

/// Mask for [U] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_U_M               mask(24, 24)

/// Number of bits to shift for [L] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_L_S               22

/// Mask for [L] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_L_M               mask(22, 22)

/// Number of bits to shift for [offset] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_OFFSET_S          10

/// Number of bits in [offset] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_OFFSET_N          12

/// Mask for [offset] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_OFFSET_M          mask(21, 10)

/// Mask for [offset] in a Register Offset-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_OFFSET_REGISTER_M b(1000_0011_1111)

/// Code for [offset] in a Register Offset-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_OFFSET_REGISTER_C b(1000_0001_1010)

/// Number of bits to shift for [xm] in a Register Offset-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_XM_REGISTER_S     16

/// Mask for [xm] in a Register Offset-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_XM_REGISTER_M     mask(20, 16)

/// Number of bits in [simm9] in a Pre/Post-Index-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_SIMM9_INDEXED_N   9

/// Number of bits to shift for [simm9] in a Pre/Post-Index-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_SIMM9_INDEXED_S   12

/// Mask for [simm9] in a Pre/Post-Index-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_SIMM9_INDEXED_M   mask(20, 12)

/// Number of bits to shift for [i] in a Pre/Post-Index-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_I_INDEXED_S       11

/// Mask for [I] in a Pre/Post-Index-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_I_INDEXED_M       mask(11, 11)

/// Mask for [offset] in a Pre/Post-Index-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_OFFSET_INDEXED_M  b(1000_0000_0001)

/// Code for [offset] in a Pre/Post-Index-ed Load Store (Single Data Transfer) instruction.
#define LOAD_STORE_DATA_OFFSET_INDEXED_C  b(0000_0000_0001)

/// Number of bits to shift for [xn] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_XN_S              5

/// Number of bits in [xn] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_XN_N              5

/// Mask for [xn] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_DATA_XN_M              mask(9, 5)

/// Baseline mask for a Single Data Transfer (Literal) instruction.
#define LOAD_STORE_LITERAL                b(0001_1000_0000_0000_0000_0000_0000_0000)

/// Mask for a Single Data Transfer (Literal) instruction
#define LOAD_STORE_LITERAL_M              ((maskl(1)) | (mask(29, 24)))

/// Number of bits to shift for [simm19] in a Single Data Transfer (Literal) instruction.
#define LOAD_STORE_LITERAL_SIMM19_S       5

/// Number of bits in [simm19] in a Single Data Transfer (Literal) instruction.
#define LOAD_STORE_LITERAL_SIMM19_N       19

/// Mask for [simm19] in a Load Store (Literal) instruction.
#define LOAD_STORE_LITERAL_SIMM19_M       mask(23, 5)

/// Number of bits to shift for [sf] in a Single Data Transfer (Load / Literal) instruction.
#define LOAD_STORE_SF_S                   30

/// Mask for [sf] in a Load-Store instruction.
#define LOAD_STORE_SF_M                   mask(30, 30)

/// Number of bits in [rt] in a Single Data Transfer (Load) instruction.
#define LOAD_STORE_RT_N                   5

/// Mask for [rt] (target register) in a Load-Store instruction.
#define LOAD_STORE_RT_M                   mask(4, 0)

/// The intermediate representation of a load and store instruction.
typedef struct {

    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// The type of the load and store instruction.
    enum LoadStoreType {

        /// The single data transfer instruction type.
        SINGLE_DATA_TRANSFER,

        /// The load literal instruction type.
        LOAD_LITERAL,

    } type;

    /// [19b] The constants for the load/store instruction group.
    union LoadStoreConstants {

        /// [19b] Single data transfer interpretation.
        struct SingleDataTransfer {

            /// [1b] Determines whether the addressing mode is unsigned offset: 0 for false, 1 for true.
            bool u;

            /// [1b] Determines the type of data transfer: 0 for store, 1 for load.
            bool l;

            /// The addressing mode.
            enum AddressingMode {

                /// Form: \code [xn, #<imm>] \endcode
                /// Transfer address: \code Xn + imm \endcode
                UNSIGNED_OFFSET,

                /// Form: \code [xn, #<simm>]! \endcode
                /// Transfer address: \code Xn + simm \endcode
                /// Write-back: \code Xn := Xn + simm \endcode
                PRE_INDEXED,

                /// Form: \code [xn], #<simm> \encode
                /// Transfer address: \code Xn \endcode
                /// Write-back: \code Xn + simm \endcode
                POST_INDEXED,

                /// In the form of \code [xn, xm] \endcode
                /// Transfer address: \code Xn + Xm \endcode
                REGISTER_OFFSET,

            } addressingMode;

            /// [12b] Offset interpretation (determined by addressing mode).
            union Offset {

                /// [5b] The encoding of the Xm register, used for the register offset addressing mode.
                uint8_t xm;

                /// [10b] The parameters for the pre/post-indexed addressing mode.
                struct PrePostIndex {

                    /// [9b] The signed value used for the pre/post-indexed addressing mode.
                    int16_t simm9;

                    /// [1b] Determines whether to use pre/post-indexing: 0 for post-indexed, 1 for pre-indexed.
                    bool i;

                } prePostIndex;

                /// [12b] 12-bit unsigned immediate constant for the unsigned offset addressing mode.
                uint16_t uoffset;

            } offset;

            /// [5b] The encoding of the Xn register.
            uint8_t xn;

        } sdt;

        /// [19b] Load literal interpretation (signed immediate value).
        Literal simm19;

    } data;

    /// [5b] The encoding of the Rt register.
    uint8_t rt;

} LoadStore_IR;

#endif // IR_LOAD_STORE_H
