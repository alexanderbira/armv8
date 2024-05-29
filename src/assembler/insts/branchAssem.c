///
/// branchAssem.h
/// Functions to parse from assembly and write as binary a Branch instruction.
///
/// Created by Lancelot Liu on 29/05/2024.
///

#include "branchAssem.h"

static void genBranch(Branch_IR *ir) {
    ir->branchType = B;
}

static void genBranchReg(Branch_IR *ir) {
    ir->branchType = BR;
}

static void genBranchCond(Branch_IR *ir, char *condStr) {
    ir->branchType = BCOND;
}

Branch_IR asmToImm(char *line) {
    char *mne = strtok(line, " ");
    Branch_IR ir;

    if (!strcasecmp(mne, "b")) {
        genBranch(&ir);
    } else if (!strcasecmp(mne, "br")) {
        genBranchReg(&ir);
    } else {
        char *cond = strtok(mne, ".");
        if (!strcasecmp(cond, "b")) {
            strtok(NULL, ".");
            genBranchCond(&ir, cond);
        }

        // TODO: Throw error
    }

    return ir;
}

BitInst immToInst(Branch_IR ir) {

}
