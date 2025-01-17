///
/// adecl.h
/// Translates an assembly instructoin into a human readable description
///
/// Created by Jack Wong on 19/06/24.
///

#ifndef EXTENSION_ADECL_H
#define EXTENSION_ADECL_H

#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ir.h"

char *adecl(IR *irObject);

#endif // EXTENSION_ADECL_H
