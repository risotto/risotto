//
// Created by rvigee on 3/14/20.
//

#include "types.h"

#define PR_VT(t) struct vtable PR_VT_NAME(t)

#define PR_VT_NAME(name) __vtable_##name

PR_VT(T_NIL);
PR_VT(T_INT);
PR_VT(T_UINT);
PR_VT(T_DOUBLE);
PR_VT(T_BOOL);
PR_VT(T_STR);

const PrimitiveTypes primitives = {
    ._nil = {.type = T_NIL, .vtable = &PR_VT_NAME(T_NIL)},
    ._int = {.type = T_INT, .vtable = &PR_VT_NAME(T_INT)},
    ._uint = {.type = T_UINT, .vtable = &PR_VT_NAME(T_UINT)},
    ._double = {.type = T_DOUBLE, .vtable = &PR_VT_NAME(T_DOUBLE)},
    ._bool = {.type = T_BOOL, .vtable = &PR_VT_NAME(T_BOOL)},
    ._string = {.type = T_STR, .vtable = &PR_VT_NAME(T_STR)},

    ._value_p = {.type = T_VALUE_P, .vtable = NULL},
    ._p = {.type = T_P, .vtable = NULL},
};
