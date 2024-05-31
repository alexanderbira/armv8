///
/// instructionHandler.h
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef ASSEMBLER_INSTRUCTION_HANDLER_H
#define ASSEMBLER_INSTRUCTION_HANDLER_H

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "../../common/const.h"
#include "../../common/ir/ir.h"
#include "../state.h"
#include "branchHandler.h"
// #include "registerHandler.h"
#include "immediateHandler.h"
#include "loadStoreHandler.h"

/// A function which processes a tokenised assembly instruction into its intermediate representation.
typedef IR (*IRGenerator)(TokenisedLine *line, AssemblerState *state);

/// An entry in an [IRGenerator] table.
typedef struct {
    char *mnemonic;
    IRGenerator handler;
} IREntry;

/// A function which produces a binary word instruction given its intermediate representation.
typedef Instruction (*InstructionGenerator)(IR *irObject, AssemblerState *state);

/// An entry in an [InstructionGenerator] table.
typedef struct {
    IRType type;
    InstructionGenerator handler;
} InstructionEntry;

IRGenerator getParseFunction(const char *mnemonic);

InstructionGenerator getTranslateFunction(IRType type);

#endif // ASSEMBLER_INSTRUCTION_HANDLER_H
