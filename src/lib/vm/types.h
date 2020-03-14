//
// Created by rvigee on 3/14/20.
//

#ifndef RISOTTOV2_TYPES_H
#define RISOTTOV2_TYPES_H

#include "valuetype.h"
#include "vtable.h"

struct ValueTypeContainer {
    ValueType type;
    struct vtable *vtable;
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

typedef PRIMITIVES_STRUCT(ValueTypeContainer) PrimitiveTypes;

void initPrimitives();

const PrimitiveTypes *getPrimitives();

#endif //RISOTTOV2_TYPES_H
