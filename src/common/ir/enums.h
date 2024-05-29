///
/// enums.h
/// Enums common to multiple IR subtypes
///
/// Created by Alexander Biraben-Renard and Billy Highley on 29/05/2024.
///

#ifndef ENUMS_H
#define ENUMS_H

/// The opcode for arithmetic operations.
enum ArithType {

    /// The operation code for add.
    /// \code Rd := Rn + Op2 \endcode
    ADD,

    /// The operation code for add, setting flags.
    /// \code Rd := Rn + Op2 \endcode
    ADDS,

    /// The operation code for subtract.
    /// \code Rd := Rn - Op2 \endcode
    SUB,

    /// The operation code for subtract, setting flags.
    /// \code Rd := Rn - Op2 \endcode
    SUBS

};

#endif //ENUMS_H
