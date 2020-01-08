//
// Created by raphael on 10/04/18.
//

#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include "memory.h"
#include "value.h"
#include "vm.h"

void initValueArray(ValueArray *array) {
    array->object.values = NULL;
    array->capacity = 0;
    array->object.size = 0;
    array->object.marked = 0;
}

void writeValueArray(ValueArray *array, Value value) {
    if (array->capacity < array->object.size + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->object.values = GROW_ARRAY(array->object.values, Value,
                                          oldCapacity, array->capacity);
    }

    array->object.values[array->object.size] = value;
    array->object.size++;
}

void freeValueArray(ValueArray *array) {
    FREE_ARRAY(Value, array->object.values, array->capacity);
    initValueArray(array);
}


Value n2v() {
    NEW_VALUE(value);
    TGET(value) = T_NIL;

    return value;
}

Value i2v(int v) {
    NEW_VALUE(value);
    DGET(value, int) = v;
    TGET(value) = T_INT;

    return value;
}

Value ui2v(unsigned int v) {
    NEW_VALUE(value);
    DGET(value, uint) = v;
    TGET(value) = T_UINT;

    return value;
}

Value b2v(bool v) {
    NEW_VALUE(value);
    DGET(value, bool) = v;
    TGET(value) = T_BOOL;

    return value;
}

Value a2v(ValueArray *v) {
    NEW_VALUE(value);
    DGET(value, p) = v;
    TGET(value) = T_ARRAY;

    return value;
}

Value vp2v(Value *v) {
    if (IS_VALUE_REF(*v)) {
        return *v;
    }

    NEW_VALUE(value);
    DGET(value, p) = v;
    TGET(value) = T_VALUE_REF;

    return value;
}

Value p2v(void *v) {
    NEW_VALUE(value);
    DGET(value, p) = v;
    TGET(value) = T_P;

    return value;
}

Value d2v(double v) {
    NEW_VALUE(value);
    DGET(value, double) = v;
    TGET(value) = T_DOUBLE;

    return value;
}

Value s2v(const char *v) {
    NEW_VALUE(value);
    DGET(value, str) = v;
    TGET(value) = T_STR;

    return value;
}

Value o2v(Object *v) {
    NEW_VALUE(value);
    DGET(value, p) = v;
    TGET(value) = T_OBJECT;

    return value;
}

int v2i(Value v) {
    V2_ACCESS_REF(v, v2i);

    assert(TGET(v) == T_INT);

    return DGET(v, int);
}

unsigned int v2ui(Value v) {
    V2_ACCESS_REF(v, v2ui);

    assert(TGET(v) == T_UINT);

    return DGET(v, uint);
}

void *v2p(Value v) {
    V2_ACCESS_REF(v, v2p);

    assert(TGET(v) == T_P);

    return DGET(v, p);
}

double v2d(Value v) {
    V2_ACCESS_REF(v, v2d);

    assert(TGET(v) == T_DOUBLE);

    return DGET(v, double);
}

const char *v2s(Value v) {
    V2_ACCESS_REF(v, v2s);

    switch (TGET(v)) {
        case T_NIL:
            return "nil";
        case T_UINT: {
            char *o = malloc(12 * sizeof(char));
            sprintf(o, "%d", v2ui(v));
            return o;
        }
        case T_INT: {
            char *o = malloc(12 * sizeof(char));
            sprintf(o, "%d", v2i(v));
            return o;
        }
        case T_DOUBLE: {
            double d = v2d(v);
            char *o = malloc(18 * sizeof(char));
            sprintf(o, "%lf", d);
            return o;
        }
        case T_OBJECT: {
            char *o = malloc(20 * sizeof(char));
            sprintf(o, "%p", v2o(v));
            return o;
        }
        case T_P: {
            char *o = malloc(20 * sizeof(char));
            sprintf(o, "%p", v2p(v));
            return o;
        }
        case T_STR:
            return DGET(v, str);
        case T_BOOL:
            return v2b(v) ? "true" : "false";
        case T_ARRAY: {
            char *o = calloc(1000, sizeof(char));
            ValueArray *array = v2a(v);
            sprintf(o, "[");
            for (int i = 0; i < array->object.size; ++i) {
                sprintf(o + strlen(o), v2s(*(array->object.values + i)));

                if (i != array->object.size - 1) {
                    sprintf(o + strlen(o), ", ");
                }
            }
            sprintf(o + strlen(o), "]");
            return o;
        }
    }

    ERROR("Unhandled value for v2s");
}

Object *v2o(Value v) {
    V2_ACCESS_REF(v, v2o);

    assert(TGET(v) == T_OBJECT || TGET(v) == T_ARRAY);

    return DGET(v, p);
}

bool v2b(Value v) {
    V2_ACCESS_REF(v, v2b);

    assert(TGET(v) == T_BOOL);

    return DGET(v, bool);
}

ValueArray *v2a(Value v) {
    V2_ACCESS_REF(v, v2a);

    assert(TGET(v) == T_ARRAY);

    return DGET(v, p);
}

Value copy(Value v) {
    v = followRefV(&v);
    NEW_VALUE(nv);

    nv.data = v.data;
    nv.type = v.type;
    nv.vtable = v.vtable;

    if (TYPECHECK(nv, T_ARRAY) || TYPECHECK(nv, T_OBJECT)) {
        Object *object = v2o(nv);
        for (int i = 0; i < object->size; ++i) {
            object->values[i] = followRefV(&object->values[i]);
        }
    }

    return nv;
}

bool typecheck(Value value, ValueType type) {
    Value *v = followRef(&value);

    return v->type == type;
}

Value *followRef(Value *value) {
    Value *v = value;
    while (v->type == T_VALUE_REF) {
        v = v->data._p;
    }

    return v;
}

Value followRefV(Value *value) {
    return *followRef(value);
}
