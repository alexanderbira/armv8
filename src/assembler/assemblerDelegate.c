///
/// instructionHandler.c
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "assemblerDelegate.h"

static const ParserEntry parsers[] = {
    { "",     parseDirective },
    { "add",  parseDataProcessing },
    { "adds", parseDataProcessing },
    { "and",  parseRegister },
    { "ands", parseRegister },
    { "b",    parseBranch },
    { "bic",  parseRegister },
    { "bics", parseRegister },
    { "br",   parseBranch },
    { "cmn",  parseDataProcessing },
    { "cmp",  parseDataProcessing },
    { "eon",  parseRegister },
    { "eor",  parseRegister },
    { "ldr",  parseLoadStore },
    { "madd", parseRegister },
    { "mneg", parseDataProcessing },
    { "mov",  parseDataProcessing },
    { "movk", parseImmediate },
    { "movn", parseImmediate },
    { "movz", parseImmediate },
    { "msub", parseRegister },
    { "mul",  parseDataProcessing },
    { "mvn",  parseRegister },
    { "neg",  parseDataProcessing },
    { "negs", parseDataProcessing },
    { "orn",  parseRegister },
    { "orr",  parseRegister },
    { "str",  parseLoadStore },
    { "sub",  parseDataProcessing },
    { "subs", parseDataProcessing },
    { "tst",  parseDataProcessing },
};

static const TranslatorEntry translators[] = {
    { IMMEDIATE,  translateImmediate },
    { REGISTER,   translateRegister },
    { LOAD_STORE, translateLoadStore },
    { BRANCH,     translateBranch },
    { DIRECTIVE,  translateDirective },
};

/// Performs [strcmp] on the [mnemonic]s of [ParserEntry]s, but takes in [void *]s.
/// @param v1 The first item.
/// @param v2 The second item.
/// @returns [int] of comparison.
static int parserCmp(const void *v1, const void *v2) {
    const ParserEntry *p1 = (const ParserEntry *) v1;
    const ParserEntry *p2 = (const ParserEntry *) v2;
    return strcmp(p1->mnemonic, p2->mnemonic);
}

/// Gets the corresponding [Parser] for [mnemonic].
/// @param mnemonic The mnemonic to search for.
/// @returns The corresponding [Parser]
Parser getParser(const char *mnemonic) {
    ParserEntry target = (ParserEntry) { mnemonic, NULL };
    ParserEntry *entry = bsearch(&target, parsers, sizeof(parsers) / sizeof(ParserEntry),
                                 sizeof(ParserEntry), parserCmp);
    assertFatalNotNullWithArgs(entry, "No Parser found for mnemonic <%s>!", mnemonic);
    return entry->handler;
}

/// Performs comparison on the [type]s of [TranslatorEntry]s, but takes in [void *]s.
/// @param v1 The first item.
/// @param v2 The second item.
/// @returns [int] of comparison.
static int translatorCmp(const void *v1, const void *v2) {
    const TranslatorEntry *p1 = (const TranslatorEntry *) v1;
    const TranslatorEntry *p2 = (const TranslatorEntry *) v2;
    return p1->type - p2->type;
}

/// Gets the corresponding [Parser] for [mnemonic].
/// @param mnemonic The mnemonic to search for.
/// @returns The corresponding [Parser]
Translator getTranslator(const IRType *type) {
    TranslatorEntry target = (TranslatorEntry) { *type, NULL };
    TranslatorEntry *entry = bsearch(&target, translators, sizeof(translators) / sizeof(TranslatorEntry),
                                     sizeof(TranslatorEntry), translatorCmp);
    assertFatalNotNullWithArgs(entry, "No Translator found for type <%s>!", type);
    return entry->handler;
}

/// Parses [line] into its IR form, and adds it to [state].
/// @param line The assembly instruction to parse.
/// @param state The [AssemblerState] to modify.
void parse(char *line, AssemblerState *state) {
    char *trimmedLine = trim(line, WHITESPACE);

    // Check if line is blank.
    if (strlen(trimmedLine) == 0) return;

    // End the line where a comment starts.
    char *commentStart = strstr(trimmedLine, "//");
    if (commentStart != NULL) {
        *commentStart = '\0';
    }

    // Check if line is a label.
    char *colon = strchr(line, ':');
    if (colon != NULL) {
        // Process as label if first character is valid.
        if (isalpha(line[0]) || line[0] == '_' || line[0] == '.') {
            char *label = strdup(line);
            assertFatalNotNull(label, "<Memory> Unable to duplicate [char *]!");
            *strchr(label, ':') = '\0';

            addMapping(state, label, state->address);
            return;
        };

        throwFatalWithArgs("Invalid label named <%s>!", line);
    }

    // By default, handle either directive or instructions.
    TokenisedLine tokenisedLine = tokenise(line);
    IR ir = getParser(tokenisedLine.mnemonic)(&tokenisedLine, state);

    state->address += 0x4;
    destroyTokenisedLine(&tokenisedLine);
    addIR(state, ir);
}
