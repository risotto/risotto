//
// Created by rvigee on 3/14/20.
//

#include "types.h"

PrimitiveTypes primitives = {};

void initPrimitives() {
    PrimitiveTypes p = (PrimitiveTypes) {
            ._nil = {.type = T_NIL, .vtable = MALLOC_VTABLE},
            ._int = {.type = T_INT, .vtable = MALLOC_VTABLE},
            ._uint = {.type = T_UINT, .vtable = MALLOC_VTABLE},
            ._double = {.type = T_DOUBLE, .vtable = MALLOC_VTABLE},
            ._bool = {.type = T_BOOL, .vtable = MALLOC_VTABLE},
            ._string = {.type = T_STR, .vtable = MALLOC_VTABLE},

            ._value_p = {.type = T_VALUE_P, .vtable = NULL},
            ._p = {.type = T_P, .vtable = NULL},
    };

    primitives = p;
}
