///
/// instructionHandler.c
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "instructionHandler.h"

static const IREntry instructionHandlers[] = {
        { "b", parseBranch },
        { "br", parseBranch },
        // TODO: Enumerate possible handlers.
};

static const InstructionEntry translationHandlers[] = {
        {IMMEDIATE,  translateImmediate},
        // { REGISTER, translateRegister },
        {LOAD_STORE, translateLoadStore},
        {BRANCH,     translateBranch},
};

/// Gets the corresponding [IRGenerator].
/// @param mnemonic The mnemonic to search for.
/// @return Either the corresponding [IRGenerator], or NULL if none was found.
IRGenerator getParseFunction(const char *mnemonic) {
    for (int i = 0; i < sizeof(instructionHandlers) / sizeof(IREntry); i++) {
        if (!strcmp(instructionHandlers[i].mnemonic, mnemonic)) {
            return instructionHandlers[i].handler;
        }
    }

    return NULL;
}

/// Gets the corresponding [InstructionGenerator].
/// @param type The type to search for.
/// @return Either the corresponding [InstructionGenerator], or NULL if none was found.
InstructionGenerator getTranslateFunction(IRType type) {
    for (int i = 0; i < sizeof(translationHandlers) / sizeof(InstructionEntry); i++) {
        if (translationHandlers[i].type == type) {
            return translationHandlers[i].handler;
        }
    }

    return NULL;
}
