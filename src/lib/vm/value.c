//
// Created by raphael on 10/04/18.
//

#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include "memory.h"
#include "value.h"
#include "types.h"

void initValueArray(ValueArray *array) {
    array->object.values = NULL;
    array->capacity = 0;
    array->object.size = 0;
    array->object.marked = 0;
}

void writeValueArray(ValueArray *array, Value value) {
    if (array->capacity < array->object.size + 1) {
        unsigned int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->object.values = GROW_ARRAY(array->object.values, Value, oldCapacity, array->capacity);
    }

    array->object.values[array->object.size] = value;
    array->object.size++;
}

void freeValueArray(ValueArray *array) {
    FREE_ARRAY(Value, array->object.values, array->capacity);
    initValueArray(array);
}

Value n2v() {
    return NEW_VALUE(&primitives._nil, p, 0);
}

Value i2v(int v) {
    return NEW_VALUE(&primitives._int, int, v);
}

Value ui2v(unsigned int v) {
    return NEW_VALUE(&primitives._uint, uint, v);
}

Value b2v(bool v) {
    return NEW_VALUE(&primitives._bool, bool, v);
}

Value a2v(ValueArray *v) {
    return NEW_VALUE(T_ARRAY, p, v);
}

Value vp2v(Value *v) {
    Value vl = *v;
    if (TGET(vl) == T_VALUE_P) {
        return vl;
    }

    return NEW_VALUE(&primitives._value_p, p, v);
}

Value p2v(void *v) {
    return NEW_VALUE(&primitives._p, p, v);
}

Value d2v(double v) {
    return NEW_VALUE(&primitives._double, double, v);
}

Value s2v(const char *v) {
    return NEW_VALUE(&primitives._string, str, v);
}

Value o2v(Object *v, ValueTypeContainer *tc) {
    return NEW_VALUE(tc, p, v);
}

int v2i(Value v) {
    ACCESS_REF(v);

    assert(TGET(v) == T_INT);

    return DGET(v, int);
}

unsigned int v2ui(Value v) {
    ACCESS_REF(v);

    assert(TGET(v) == T_UINT);

    return DGET(v, uint);
}

void *v2p(Value v) {
    ACCESS_REF(v);

    assert(TGET(v) == T_P);

    return DGET(v, p);
}

double v2d(Value v) {
    ACCESS_REF(v);

    assert(TGET(v) == T_DOUBLE);

    return DGET(v, double);
}

const char *v2s(Value v) {
    ACCESS_REF(v);

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
                sprintf(o + strlen(o), "%s", v2s(*(array->object.values + i)));

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
    ACCESS_REF(v);

    assert(TGET(v) == T_OBJECT || TGET(v) == T_ARRAY);

    return DGET(v, p);
}

bool v2b(Value v) {
    ACCESS_REF(v);

    assert(TGET(v) == T_BOOL);

    return DGET(v, bool);
}

ValueArray *v2a(Value v) {
    ACCESS_REF(v);

    assert(TGET(v) == T_ARRAY);

    return DGET(v, p);
}

Value copy(Value v) {
    if (TGET(v) != T_VALUE_P) {
        return v;
    }

    Value nv = NEW_VALUE(T_NIL, p, 0);
    set(v, &nv);

    if (typecheck(nv, T_ARRAY) || typecheck(nv, T_OBJECT)) {
        Object *object = v2o(nv);
        for (int i = 0; i < object->size; ++i) {
            object->values[i] = copy(object->values[i]);
        }
    }

    return nv;
}

bool typecheck(Value value, ValueType type) {
    ACCESS_REF(value);

    return TGET(value) == type;
}

Value accessRef(Value v) {
    return *accessRefp(&v);
}

Value *accessRefp(Value *value) {
    Value *v = value;
    while (TGETP(v) == T_VALUE_P) {
        v = v->data._p;
    }

    return v;
}

bool veq(Value l, Value r) {
    l = accessRef(l);
    r = accessRef(r);

    if (TGET(l) != TGET(r)) {
        return false;
    }

    switch (TGET(l)) {
        case T_NIL:
            return true;
        case T_UINT:
            return DGET(l, uint) == DGET(r, uint);
        case T_INT:
            return DGET(l, int) == DGET(r, int);
        case T_DOUBLE:
            return DGET(l, double) == DGET(r, double);
        case T_P:
        case T_OBJECT:
        case T_ARRAY:
            return DGET(l, p) == DGET(r, p);
        case T_STR: {
            const char *ls = v2s(l);
            const char *rs = v2s(r);

            bool res = strcmp(ls, rs);
            free((void *) ls);
            free((void *) rs);

            return res;
        }
        case T_BOOL:
            return DGET(l, bool) == DGET(r, bool);
    }
}

void set(Value origin, Value *target) {
    target = accessRefp(target);
    origin = accessRef(origin);

    target->data = origin.data;
    target->tc = origin.tc;
}
