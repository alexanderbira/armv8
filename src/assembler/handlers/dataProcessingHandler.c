//
// Created by Jack on 6/3/2024.
//

#include "dataProcessingHandler.h"

/// Converts the assembly form of an Data Processing instruction to IR form.
/// @param line The [TokenisedLine] of the instruction.
/// @param state The current state of the assembler.
/// @return The [IR] struct representing the instruction.
/// @pre The incoming [line] is a Data Processing (Immediate) assembly instruction.
IR dataProcessingHandler(TokenisedLine *line, AssemblerState *state) {

}

/// Converts an assembly form instruction to its alias (if applicable).
/// @param line The [TokenisedLine] of the instruction.
/// @return The [TokenisedLine] of the input's alias (or itself, if no alias).
TokenisedLine convertAlias(TokenisedLine *line) {
}