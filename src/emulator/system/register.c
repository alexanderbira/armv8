///
/// register.c
/// Structure, manipulation, and allocation of the virtual register.
///
/// Created by Lancelot Liu on 26/05/2024.
///

#include "register.h"

Register createRegister() {
    Register reg;
    initRegister(&reg);
    return reg;
};

void initRegister(Register* reg) {
    // All registers are initialised to zero.
    for (int i = 0; i < 31; i++) {
        reg->gprs[i] = 0;
    }
    reg->zr = 0;
    reg->pc = 0;
    reg->sp = 0;

    // All flags are cleared on init except the zero-flag.
    reg->pstate = (PState){false, true, false, false};
}
