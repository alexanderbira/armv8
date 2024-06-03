//
// Created by Jack on 6/3/2024.
//

#ifndef DATAPROCESSINGHANDLER_H
#define DATAPROCESSINGHANDLER_H

#include <string.h>

#include "../../common/ir/ir.h"
#include "../helpers.h"
#include "../state.h"

IR dataProcessingHandler(TokenisedLine *line, AssemblerState *state);

#endif //DATAPROCESSINGHANDLER_H
