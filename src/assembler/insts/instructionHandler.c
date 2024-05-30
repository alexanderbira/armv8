///
/// instructionHandler.c
/// Functions to categorise assembly instructions.
///
/// Created by Lancelot Liu on 30/05/2024.
///

#include "instructionHandler.h"

static const HandlerEntry instructionHandlers[] = {
        {"b", parseBranch}
};

Handler getParseFunction(const char *mnemonic) {
    for (int i = 0; i < sizeof(instructionHandlers) / sizeof(HandlerEntry); i++) {
        if (!strcmp(instructionHandlers[i].mnemonic, mnemonic)) {
            return instructionHandlers[i].handler;
        }
    }

    return NULL;
}
