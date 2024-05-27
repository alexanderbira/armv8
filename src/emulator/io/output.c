///
/// output.c
/// The printing of register and memory content after execution of program.
///
/// Created by Lancelot Liu on 27/05/2024.
///

#include "output.h"

void dumpRegs(Registers *reg) {
    fprintf(stdout, "Registers:\n");
    for (int i = 0; i < 31; i++) {
        fprintf(stdout, "X%02d    = %llx\n", i, getReg64(reg, i));
    }
    fprintf(stdout, "PC     = %llx\n", getRegPC(reg));

    char nFlag = getRegState(reg, N) ? 'N' : '-';
    char zFlag = getRegState(reg, Z) ? 'Z' : '-';
    char cFlag = getRegState(reg, C) ? 'C' : '-';
    char vFlag = getRegState(reg, V) ? 'V' : '-';

    fprintf(stdout, "PSTATE : %d%d%d%d\n", nFlag, zFlag, cFlag, vFlag);
}


void dumpMem(void *mem) {

}