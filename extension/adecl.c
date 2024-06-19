///
/// adecl.c
/// Translates an assembly instructoin into a human readable description
///
/// Created by Jack Wong on 19/06/24.
///

#include "adecl.h"

/// Translates [irObject] to its human readable description.
/// @param irObject The instruction to interpret.
/// @returns The human readable description.
char *adecl(IR *irObject) {

    switch (irObject->type) {
        case IMMEDIATE: {
            // TODO
        }
        case REGISTER: {
            // TODO
        }
        case LOAD_STORE: {
            // TODO
        }
        case BRANCH: {
            // TODO
        }
        case DIRECTIVE: {
            // TODO
        }
    }
}
