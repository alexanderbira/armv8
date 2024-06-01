///
/// output.c
/// The printing of register and memory content after execution of program.
///
/// Created by Lancelot Liu on 27/05/2024.
///

#include "output.h"

void dumpRegs(Registers regs, FILE *fileOut) {
    fprintf(fileOut, "Registers:\n");
    for (int i = 0; i < NUM_GPRS; i++) {
        fprintf(fileOut, "X%02d    = %016" PRIx64 "\n", i, getReg(regs, i));
    }
    fprintf(fileOut, "PC     = %016" PRIx64 "\n", getRegPC(regs));

    char nFlag = getRegState(regs, N) ? 'N' : '-';
    char zFlag = getRegState(regs, Z) ? 'Z' : '-';
    char cFlag = getRegState(regs, C) ? 'C' : '-';
    char vFlag = getRegState(regs, V) ? 'V' : '-';

    fprintf(fileOut, "PSTATE : %c%c%c%c\n", nFlag, zFlag, cFlag, vFlag);
}


void dumpMem(Memory mem, FILE *fileOut) {
    fprintf(fileOut, "Non-zero memory:\n");
    for (int addr = 0; addr < MEMORY_SIZE; addr += 0x4) {
        uint32_t curr = readMem(mem, false, addr);
        if (curr) fprintf(fileOut, "0x%08x: 0x%08x\n", addr, curr);
    }
}
