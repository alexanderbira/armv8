///
/// branchParser.h
/// Transform a [TokenisedLine] to an [IR] of a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#ifndef ASSEMBLER_BRANCH_PARSER_H
#define ASSEMBLER_BRANCH_PARSER_H

#include "../../helpers.h"
#include "../../state.h"

/// An entry in an [enum BranchCondition] table.
typedef struct {
    char *subMnemonic;
    enum BranchCondition code;
} BranchEntry;

IR parseBranch(TokenisedLine *line, unused AssemblerState *state);

#endif // ASSEMBLER_BRANCH_PARSER_H
