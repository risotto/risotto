//
// Created by raphael on 10/04/18.
//

#ifndef RISOTTOVM_VALUE_H
#define RISOTTOVM_VALUE_H

#include "common.h"
#include "../lib/vec/src/vec.h"
#include "valuetype.h"

#define DGET(value, type) (value).data._##type
#define TGET(value) (value).tc->type
#define VTGET(value) (value).tc->vtable
#define TGETP(value) (value)->tc->type
#define NEW_VALUE(_tc, tu, value) (Value) {.tc = _tc, .data={._##tu = value}}
#define ACCESS_REF(value) value = accessRef(value)

typedef union {
    unsigned int _uint;
    int _int;
    double _double;
    void *_p;
    const char *_str;
    bool _bool;
} ValueData;

typedef struct ValueTypeContainer ValueTypeContainer;

typedef struct {
    ValueData data;
    const ValueTypeContainer *tc;
} Value;

// This is super confusing syntax:
// Creates a type `NativeFunction` which is a function `void (Value[], int, Value[])`
typedef void (*NativeFunction)(Value args[], int argsc, Value ret[]);

typedef struct Object Object;

struct Object {
    Object *next;
    unsigned char marked;
    Value *values;
    unsigned int size;
};

typedef struct {
    Object object;
    unsigned int capacity;
} ValueArray;

void initValueArray(ValueArray *array);

void writeValueArray(ValueArray *array, Value value);

void freeValueArray(ValueArray *array);

Value n2v();

Value p2v(void *v);

Value ui2v(unsigned int v);

Value i2v(int v);

Value d2v(double v);

Value s2v(const char *v);

Value o2v(Object *v, struct ValueTypeContainer *tc);

Value b2v(bool b);

Value vp2v(Value *);

Value a2v(ValueArray *, ValueTypeContainer *tc);

void *v2p(Value v);

unsigned int v2ui(Value v);

int v2i(Value v);

double v2d(Value v);

const char *v2s(Value v);

Object *v2o(Value v);

bool v2b(Value v);

ValueArray *v2a(Value v);

Value copy(Value v);

bool typecheck(Value value, ValueType type);

Value accessRef(Value v);

Value *accessRefp(Value *v);

bool veq(Value l, Value r);

void set(Value origin, Value *target);

#endif //RISOTTOVM_VALUE_H
