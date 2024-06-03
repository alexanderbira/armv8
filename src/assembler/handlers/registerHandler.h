//
// Created by Jack on 6/1/2024.
//

#ifndef ASSEMBLER_REGISTER_H
#define ASSEMBLER_REGISTER_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../common/const.h"
#include "../../common/error.h"
#include "../../common/ir/ir.h"
#include "../helpers.h"
#include "../state.h"

IR parseRegister(TokenisedLine *line, unused AssemblerState *state);

Instruction translateRegister(IR *irObject, unused AssemblerState *state);

#endif // ASSEMBLER_REGISTER_H
