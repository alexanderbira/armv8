///
/// assemblerDelegate.h
/// Delegation logic for the assembler.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#ifndef ASSEMBLER_DELEGATE_H
#define ASSEMBLER_DELEGATE_H

#include <stdlib.h>
#include <string.h>

#include "../common/ir/ir.h"
#include "state.h"
#include "parsers/branch/branchParser.h"
#include "parsers/dataProcessing/dataProcessingParser.h"
#include "parsers/dataProcessing/immediateParser.h"
#include "parsers/dataProcessing/registerParser.h"
#include "parsers/directive/directiveParser.h"
#include "parsers/loadStore/loadStoreParser.h"
#include "translators/branch/branchTranslator.h"
#include "translators/dataProcessing/immediateTranslator.h"
#include "translators/dataProcessing/registerTranslator.h"
#include "translators/directive/directiveTranslator.h"
#include "translators/loadStore/loadStoreTranslator.h"

/// A function which processes a tokenised assembly instruction into its intermediate representation.
typedef IR (*Parser)(TokenisedLine *line, AssemblerState *state);

/// An entry in an [Parser] table.
typedef struct {

    const char *mnemonic;

    const Parser handler;

} ParserEntry;

/// A function which produces a binary word instruction given its intermediate representation.
typedef Instruction (*Translator)(IR *irObject, AssemblerState *state);

/// An entry in an [Translator] table.
typedef struct {

    const IRType type;

    const Translator handler;

} TranslatorEntry;

Parser getParser(const char *mnemonic);

Translator getTranslator(const IRType *type);

#endif // ASSEMBLER_DELEGATE_H
