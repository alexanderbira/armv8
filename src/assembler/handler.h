///
/// classify.h
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 28/05/2024.
///

#ifndef ASSEMBLER_HANDLER_H
#define ASSEMBLER_HANDLER_H

#include <stdlib.h>

#include "../common/ir/ir.h"

/// Function which takes a tokenised assembly instruction and returns its IR.
typedef IR (*Handler)(char **);

/// A mapping between an instruction mnemonic and its [Handler].
typedef struct {
    const char *mnemonic;
    Handler handler;
} HandlerEntry;

#endif //ASSEMBLER_HANDLER_H
