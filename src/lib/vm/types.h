//
// Created by rvigee on 3/14/20.
//

#ifndef RISOTTOV2_TYPES_H
#define RISOTTOV2_TYPES_H

#include "valuetype.h"
#include "vtable.h"

struct ValueTypeContainer {
    const ValueType type;
    struct vtable * const vtable;
};

#define PRIMITIVES_STRUCT(T) \
    struct { \
        T _nil; \
        T _int; \
        T _uint; \
        T _double; \
        T _bool; \
        T _string; \
        T _value_p; \
        T _p; \
    }

typedef PRIMITIVES_STRUCT(const ValueTypeContainer) PrimitiveTypes;

extern const PrimitiveTypes primitives;

#endif //RISOTTOV2_TYPES_H
