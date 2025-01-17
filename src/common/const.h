///
/// const.h
/// Constants and helpers for the rest of the program.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#ifndef COMMON_CONST_H
#define COMMON_CONST_H

#include <limits.h>
#include <math.h>
#include <stdint.h>

/// The virtual memory size of the emulated machine.
#define MEMORY_SIZE      (2 << 20)

/// All considered whitespace characters.
#define WHITESPACE       " \n\t\r"

/// The number of general purpose registers in the virtual machine.
#define NO_GPRS          31

/// The encoding of the zero register.
#define ZERO_REGISTER    31

/// ID of the colour scheme for the menu window.
#define MENU_SCHEME      7

/// ID of the default colour scheme.
#define DEFAULT_SCHEME   8

/// ID of the inverted default colour scheme.
#define I_DEFAULT_SCHEME 9

/// ID of the colour scheme for selected content.
#define SELECTED_SCHEME  10

/// ID of the colour scheme for errored content.
#define ERROR_SCHEME     11

/// ID of the colour scheme for inverted errored content.
#define I_ERROR_SCHEME   12

/// The height (in characters) of GRIM's title.
#define TITLE_HEIGHT      1

/// The height (in characters) of GRIM's help menu.
#define MENU_HEIGHT       1

/// The height (in characters) of the main content.
#define CONTENT_HEIGHT    ((int) rows - TITLE_HEIGHT - MENU_HEIGHT)

/// Alias for a chunk of data passed to and from the virtual registers or memory.
typedef uint64_t BitData;

/// Alias for an instruction as a binary word.
typedef uint32_t Instruction;

/// Alias for a bit-mask.
typedef uint32_t Mask;

/// Alias for a sub-component of an [Instruction].
typedef uint32_t Component;

/// The mode that GRIM is in.
typedef enum {
    EDIT,   ///< Standard editing mode.
    DEBUG,  ///< Read-only, debugging.
    BINARY, ///< Read-only view of compiled binary.
} EditorMode;

/// The status of the editor.
typedef enum {
    READ_ONLY, ///< File is read-only.
    UNSAVED,   ///< File has unsaved changes.
    SAVED      ///< File has no pending changes.
} EditorStatus;

/// The status of the line after going through the assembler.
typedef enum {
    /// If the line was successfully assembled.
    ASSEMBLED,

    /// If an error was encountered during the line assembly.
    ERRORED,

    /// If the line didn't need to be assembled (e.g. comments, labels, ...).
    NONE,
} LineStatus;

/// Contains data about the line after it goes through the assembler.
typedef struct {
    /// The status of the line after going through the assembler.
    LineStatus lineStatus;

    /// Data about the line after it goes through the assembler.
    union {
        /// The assembled instruction.
        Instruction instruction;

        /// A string containing the error message.
        char *error;
    } data;
} LineInfo;

/// Shorthand to mark an input parameter is unused. This is usually because a type signature
/// has to be followed for a group of functions.
#define unused __attribute__((unused))

/// Shorthand for binary literals. Ignores '_'s.
/// @param __LITERAL__ The literal to interpret as binary.
/// @returns The value of the literal, implicitly casted.
/// @example \code b(1010_0101) = 0xA5 \endcode
/// @warning May not be optimised at compile - do not abuse!
#define b(__LITERAL__) toBinary(#__LITERAL__)

static inline uint64_t toBinary(const char *str) {
    uint64_t result = 0;
    while (*str) {
        if (*str == '_') {
            str++;
            continue;
        }
        result <<= 1;
        result += *str++ - '0';
    }
    return result;
}

/// Shorthand for simple bitmasks on the "right hand" / least significant side. One-indexed.
/// Significant inspiration taken from @see https://stackoverflow.com/a/28703383/
/// @param __ONE_COUNT__ The number of bits to set.
/// @returns The mask.
/// @example \code maskr(uint8_t, 3) = 0x07 \endcode
/// @warning Produces [uint32_t]!
/// @pre 0 <= __ONE_COUNT__ <= 32
#define maskr(__ONE_COUNT__) \
    ((__ONE_COUNT__) == 0 ? 0 : (((uint32_t)-1) >> (sizeof(uint32_t) * CHAR_BIT - (__ONE_COUNT__))))

/// Shorthand for simple bitmasks on the "left hand" / most significant side. One-indexed.
/// @param __ONE_COUNT__ The number of bits to set.
/// @returns The mask.
/// @example \code maskl(4) = 0xF0000000 \endcode
/// @warning Produces [uint32_t]!
/// @pre 0 <= __ONE_COUNT__ <= 32
#define maskl(__ONE_COUNT__) \
    ((__ONE_COUNT__) == 0 ? 0 : (((uint32_t)-1) << (sizeof(uint32_t) * CHAR_BIT - (__ONE_COUNT__))))

/// Shorthand for simple bitmasks in the "center" / delimited by MSB and LSB, inclusive. 0-indexed.
/// Based on original by Billy Highley, \code ((1 << (msb - lsb + 1)) - 1) << lsb; \endcode
/// @param __MSB__ The most significant bit to start the mask from.
/// @param __LSB__ The least significant bit to end the mask on.
/// @returns The mask.
/// @example \code mask(3, 1) = 0xE \endcode \code mask(31, 29) = 0xe0000000 \endcode
/// @warning Produces [uint32_t]!
/// @pre 31 >= __MSB__, __LSB__ >= 0
#define mask(__MSB__, __LSB__) (maskl(8 * sizeof(uint32_t) - (__LSB__)) & maskr((__MSB__) + 1))

/// Shorthand for truncating a value to just [__BIT_COUNT__] of its least significant bits.
/// @param __VALUE__ The value to truncate.
/// @param __BIT_COUNT__ The number of bits to preserve.
/// @returns The truncated value.
/// @example \code truncater(0xF, 3) = 0x7 \endcode
#define truncater(__VALUE__, __BIT_COUNT__) ((__VALUE__) & maskr(__BIT_COUNT__))

/// Applies the given mask to an instruction and returns the bits
/// shifted so that the LSB is right-aligned to yield the component.
/// @param __WORD__ The instruction to mask.
/// @param __MASK__ The mask to use on the instruction.
/// @returns The shifted bit pattern extracted by the mask.
/// @example \code decompose(10111, 11100) == 00101 \endcode
/// @authors Billy Highley and Alexander Biraben-Renard
#define decompose(__WORD__, __MASK__) decompose(__WORD__, __MASK__)

static inline Component decompose(Instruction word, Mask mask) {
    uint32_t bits = word & mask;
    while (!(mask & 1) && (mask >>= 1)) bits >>= 1;
    return bits;
}

/// Sign extends the given [__VALUE__] given that only [__DESIRED_WIDTH__].
/// bits are wanted.
/// @param __VALUE__ The value to sign extend.
/// @param __ACTUAL_WIDTH__ The number of bits that need to be preserved.
/// @returns The sign extended value.
#define signExtend(__VALUE__, __ACTUAL_WIDTH__) \
    ((__typeof__(__VALUE__))((__VALUE__) << (8 * sizeof(__VALUE__) - (__ACTUAL_WIDTH__))) >> \
    (8 * sizeof(__VALUE__) - (__ACTUAL_WIDTH__)))

/// Counts the number of digits in [__VALUE__] if it were printed in decimal.
/// @param __VALUE__ The value to count digits for.
/// @returns The number of digits in [__VALUE__].
#define countDigits(__VALUE__) \
    ((__VALUE__ == 0) ? 1 : (int) floor(log10((double) __VALUE__)) + 1)

#endif // COMMON_CONST_H
