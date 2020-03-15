//
// Created by rvigee on 3/14/20.
//

#include "types.h"

PrimitiveTypes primitives = {};

void initPrimitives() {
    PrimitiveTypes p = (PrimitiveTypes) {
            ._nil = {.type = T_NIL, .vtable = malloc(sizeof(struct vtable))},
            ._int = {.type = T_INT, .vtable = malloc(sizeof(struct vtable))},
            ._uint = {.type = T_UINT, .vtable = malloc(sizeof(struct vtable))},
            ._double = {.type = T_DOUBLE, .vtable = malloc(sizeof(struct vtable))},
            ._bool = {.type = T_BOOL, .vtable = malloc(sizeof(struct vtable))},
            ._string = {.type = T_STR, .vtable = malloc(sizeof(struct vtable))},
            ._value_p = {.type = T_VALUE_P, .vtable = malloc(sizeof(struct vtable))},
            ._p = {.type = T_P, .vtable = malloc(sizeof(struct vtable))},
    };

    primitives = p;
}
