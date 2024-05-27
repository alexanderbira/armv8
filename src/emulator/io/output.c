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
        fprintf(stdout, "X%02d    = %016" PRIx64 "\n", i, getReg64(reg, i));
    }
    fprintf(stdout, "PC     = %016" PRIx64 "\n", getRegPC(reg));

    char nFlag = getRegState(reg, N) ? 'N' : '-';
    char zFlag = getRegState(reg, Z) ? 'Z' : '-';
    char cFlag = getRegState(reg, C) ? 'C' : '-';
    char vFlag = getRegState(reg, V) ? 'V' : '-';

    fprintf(stdout, "PSTATE : %c%c%c%c\n", nFlag, zFlag, cFlag, vFlag);
}


void dumpMem(Memory mem) {
    fprintf(stdout, "Non-zero memory:\n");
    for (int addr = 0; addr < MEMORY_SIZE; addr += 0x4) {
        uint32_t curr = readMem32(mem, addr);
        if (curr) fprintf(stdout, "0x%08x: 0x%08x\n", addr, curr);
    }
}
