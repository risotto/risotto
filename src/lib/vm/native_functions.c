//
// Created by rvigee on 2018-09-24.
//

#include <stdio.h>
#include "value.h"
#include "vm.c"

#define str_t const char *

void vm_stats(Value args[], int argc, Value *ret) {
    printf("Objects count: %i\n", getVM()->numObjects);
    printf("Max Objects: %i\n", getVM()->maxObjects);
}

void run_gc(Value args[], int argc, Value *ret) {
    gc();
}

#define NATIVE_BINARY_FUNCTION_NAME(leftType, opName, rightType) binary_##leftType##_##opName##_##rightType

#define NATIVE_BINARY_FUNCTION(leftType, opName, rightType, leftCType, rightCType, leftV2, rightV2) \
void NATIVE_BINARY_FUNCTION_NAME(leftType, opName, rightType)(Value args[], int argc, Value *ret) { \
    leftCType left = leftV2(args[0]); \
    rightCType right = rightV2(args[1]);

#define NATIVE_BINARY_STRING_OPS(type, CType, V2, fmt) \
NATIVE_BINARY_FUNCTION(type, add, string, CType, str_t, V2, v2s) \
    char *result; \
    asprintf(&result, #fmt "%s", left, right); \
    ret[0] = s2v(result); \
} \
NATIVE_BINARY_FUNCTION(string, add, type, str_t, CType, v2s, V2) \
    char *result; \
    asprintf(&result, "%s" #fmt, left, right); \
    ret[0] = s2v(result); \
}

// Int

NATIVE_BINARY_STRING_OPS(int, int, v2i, %i)

void unary_prefix_int_negate(Value *args, int argc, Value *ret) {
    ret[0] = i2v(-v2i(args[0]));
}

#define NATIVE_UNARY_PREFIX_IN_PLACE(type, opName, v2t, t2v, operation) \
void unary_prefix_##type##_##opName(Value *args, int argc, Value *ret) { \
    Value v = args[0]; \
    Value ov = t2v(v2t(v) operation); \
    set(ov, &v); \
    ret[0] = v; \
}

#define NATIVE_UNARY_POSTFIX_IN_PLACE(type, opName, v2t, t2v, operation) \
void unary_postfix_##type##_##opName(Value *args, int argc, Value *ret) { \
    Value v = args[0]; \
    type tv = v2t(v); \
    Value ov = t2v(tv operation); \
    set(ov, &v); \
    ret[0] = t2v(tv); \
}

NATIVE_UNARY_PREFIX_IN_PLACE(int, decrement, v2i, i2v, -1)

NATIVE_UNARY_PREFIX_IN_PLACE(int, increment, v2i, i2v, +1)

NATIVE_UNARY_POSTFIX_IN_PLACE(int, decrement, v2i, i2v, -1)

NATIVE_UNARY_POSTFIX_IN_PLACE(int, increment, v2i, i2v, +1)

// Double

NATIVE_BINARY_STRING_OPS(double, double, v2d, %lf)

void unary_prefix_double_negate(Value *args, int argc, Value *ret) {
    ret[0] = d2v(-v2d(args[0]));
}

NATIVE_UNARY_PREFIX_IN_PLACE(double, decrement, v2d, d2v, -1.0)

NATIVE_UNARY_PREFIX_IN_PLACE(double, increment, v2d, d2v, +1.0)

NATIVE_UNARY_POSTFIX_IN_PLACE(double, decrement, v2d, d2v, -1.0)

NATIVE_UNARY_POSTFIX_IN_PLACE(double, increment, v2d, d2v, +1.0)

// Bool

void binary_bool_add_string(Value args[], int argc, Value *ret) {
    bool left = v2b(args[0]);
    str_t right = v2s(args[1]);

    str_t lefts = left ? "true" : "false";

    char *result;
    asprintf(&result, "%s%s", lefts, right);
    ret[0] = s2v(result);
}

void unary_prefix_bool_invert(Value args[], int argc, Value *ret) {
    ret[0] = b2v(!v2b(args[0]));
}

void binary_string_add_bool(Value args[], int argc, Value *ret) {
    str_t left = v2s(args[0]);
    bool right = v2b(args[1]);

    str_t rights = right ? "true" : "false";

    char *result;
    asprintf(&result, "%s%s", left, rights);

    ret[0] = s2v(result);
}

// String

NATIVE_BINARY_FUNCTION(string, add, string, str_t, str_t, v2s, v2s)
    char *result;
    asprintf(&result, "%s%s", left, right);

    ret[0] = s2v(result);
}

void println_int(Value args[], int argc, Value *ret) {
    int v = v2i(args[0]);

    vm.printf("%i\n", v);
}

void println_double(Value args[], int argc, Value *ret) {
    double v = v2d(args[0]);

    vm.printf("%f\n", v);
}

void println_string(Value args[], int argc, Value *ret) {
    str_t v = v2s(args[0]);

    vm.printf("%s\n", v);
}

void println_bool(Value args[], int argc, Value *ret) {
    bool v = v2b(args[0]);

    if (v) {
        vm.printf("true\n");
    } else {
        vm.printf("false\n");
    }
}

void array_size(Value args[], int argc, Value *ret) {
    ValueArray *array = v2a(args[0]);

    ret[0] = i2v(array->object.size);
}

void array_add(Value args[], int argc, Value *ret) {
    ValueArray *array = v2a(args[0]);

    Value value = args[1];

    writeValueArray(array, value);
}

void array_at(Value args[], int argc, Value *ret) {
    ValueArray *array = v2a(args[0]);

    unsigned index = v2i(args[1]);

    if(index < 0) {
        unsigned int size = array->object.size;

        index = size + index;
    }

    ret[0] = vp2v(&array->object.values[index]);
}
