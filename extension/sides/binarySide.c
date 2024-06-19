///
/// binarySide.c
/// The side panel for the binary representation of the assembly code.
///
/// Created by Lancelot Liu on 19/06/2024.
///


#include "binarySide.h"

static void updateBinary(unused Line *line, int index);

static void strBinRep(char *str, Instruction instruction);

void updateSide(void) {
    // Allocate memory for the lineInfo array.
    lineInfo = malloc(file->size * sizeof(LineInfo));

    AssemblerState state = createState();

    // Perform the first pass of the assembler.
    for (int i = 0; i < file->size; i++) {
        size_t irCount = state.irCount;

        LineInfo currLineInfo;
        fatalError[0] = '\0';

        setjmp(fatalBuffer);
        if (fatalError[0] != '\0') {
            // An error has occured.
            currLineInfo.lineStatus = ERRORED;
            currLineInfo.data.error = strdup(fatalError);
        } else {
            parse(getLine(file->lines[i]), &state);

            // At this point, the parsing was successful.
            currLineInfo.lineStatus = (state.irCount == irCount) ? NONE : ASSEMBLED;
            currLineInfo.data.error = NULL;
        }

        lineInfo[i] = currLineInfo;
    }

    // Keep track of which line we're doing a second pass on, and reset counter.
    int currLineNum = 0;
    state.address = 0x0;

    // Perform the second pass of the assembler.
    for (size_t i = 0; i < state.irCount; i++) {
        // Skip lines which do not require a second pass.
        while (lineInfo[currLineNum].lineStatus != ASSEMBLED) currLineNum++;

        IR ir = state.irList[i];
        fatalError[0] = '\0';

        setjmp(fatalBuffer);
        if (fatalError[0] != '\0') {
            // Fatal error was encountered during second assembly pass.
            free(lineInfo[currLineNum].data.error);
            lineInfo[currLineNum].lineStatus = ERRORED;
            lineInfo[currLineNum].data.error = strdup(fatalError);
        } else {
            Instruction instruction = getTranslator(&ir.type)(&ir, &state);

            // If this code is reached, the line was assembled.
            lineInfo[currLineNum].lineStatus = ASSEMBLED;
            lineInfo[currLineNum].data.instruction = instruction;
        }

        state.address += 0x4;
        currLineNum++;
    }

    // Print out all lines in current window.
    iterateLinesInWindow(file, &updateBinary);

    // Free the line info
    destroyState(state);
    for (int i = 0; i < file->size; i++) {
        // Free all error strings
        if (lineInfo[i].lineStatus == ERRORED) {
            free(lineInfo[i].data.error);
        }
    }
    free(lineInfo);
}

static void updateBinary(unused Line *line, int index) {
    switch (lineInfo[index].lineStatus) {
        case ERRORED:
            // Display the error
            wattron(side, COLOR_PAIR(13));
            mvwaddnstr(side, index - file->windowY, 0,
                        lineInfo[index].data.error, (cols - 1) / 2);
            wattroff(side, COLOR_PAIR(13));
            break;

        case ASSEMBLED: {
            // Convert the instruction to a string.
            char instrStr[8 * sizeof(Instruction) + 8];
            strBinRep(instrStr, lineInfo[index].data.instruction);

            // Display the binary string.
            wattron(side, COLOR_PAIR(11));
            mvwaddnstr(side, index - file->windowY, 0,
                        instrStr, (cols - 1) / 2);
            wattroff(side, COLOR_PAIR(11));
            break;
        }

        default: break;
    }
}

/// Convert an instruction to a binary string representation.
/// @param str A pointer to the char array to overwrite with the binary repr.
/// @param instruction The instruction to convert.
static void strBinRep(char *str, Instruction instruction) {
    int i = sizeof(instruction) * 8 + 7;

    str[i] = '\0';

    while (--i >= 0) {
        str[i] = '0' + (instruction & 1);
        if (i % 5 == 0) {
            str[--i] = ' ';
        }
        instruction >>= 1;
    }
}
