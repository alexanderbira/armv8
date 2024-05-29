//
// Created by Alexander Biraben-Renard on 29/05/2024.
//

#ifndef SDT_H
#define SDT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    /// [1b] The bit-width of all the registers in the instruction: 0 for 32-bit, 1 for 64-bit.
    bool sf;

    /// [1b] Unsigned offset flag. When this bit is set, the addressing mode is an Unsigned Offset.
    bool u;

    /// [1b] The type of data transfer. When 1, this is a load operation. Otherwise it is a store.
    bool l;

    /// The addressing mode
    enum {
        REGISTER_OFFSET, /// Transfer address = Xn + uoffset
        PRE_INDEXED,     /// Transfer address = Xn + simm9, Write-back = Xn + simm9
        POST_INDEXED,    /// Transfer address = Xn, Write-back = Xn + simm9
        UNSIGNED_OFFSET  /// Transfer address = Xn + Xm
    } addressingMode;

    /// [12b] The offset. The meaning of the offset depends on the addressing mode.
    union {

        /// [5b] The code for register Xm. Used for the register offset addressing mode.
        uint8_t xm;

        /// [10b] The parameters for the pre/post indexed addressing mode.
        struct {
            /// [9b] The signed value used for the pre/post-index addressing mode.
            int16_t simm9;

            /// [1b] Determines which of pre/post-indexing the addressing mode is using: 0 for post-indexed, 1 for pre-indexed.
            bool i;
        } prePostIndex;

        /// [12b] The immediate constant for the unsigned offset addressing mode.
        uint16_t uoffset;

    } offset;

    /// [5b] The base register.
    uint8_t xn;

    /// [5b] The target register.
    uint8_t rt;

} SDT_IR;

#endif //SDT_H
