//
// Created by rvigee on 2018-09-24.
//

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include "value.h"
#include "vm.c"

Value vm_stats(Value args[], int argc) {
    printf("Objects count: %i\n", vm.numObjects);
    printf("Max Objects: %i\n", vm.maxObjects);

    return n2v();
}

Value run_gc(Value args[], int argc) {
    gc();

    return n2v();
}

#define str_t const char *

#define NATIVE_BINARY_FUNCTION_NAME(leftType, opName, rightType) binary_##leftType##_##opName##_##rightType

#define NATIVE_BINARY_FUNCTION(leftType, opName, rightType, leftCType, rightCType, leftV2, rightV2) \
Value NATIVE_BINARY_FUNCTION_NAME(leftType, opName, rightType)(Value args[], int argc) { \
    leftCType left = leftV2(args[0]); \
    rightCType right = rightV2(args[1]);

#define NATIVE_BINARY_MATH_OP(leftType, opName, rightType, leftV2, rightV2, op, outV) \
NATIVE_BINARY_FUNCTION(leftType, opName, rightType, leftType, rightType, leftV2, rightV2) \
    return outV(left op right); \
}

#define NATIVE_BINARY_SHORTHAND_MATH_OP(leftType, opName, rightType, leftV2, rightV2, op, outV) \
NATIVE_BINARY_FUNCTION(leftType, opName##_equal, rightType, leftType, rightType, leftV2, rightV2) \
    Value r = NATIVE_BINARY_FUNCTION_NAME(leftType, opName, rightType)(args, argc); \
    set(r, &args[0]); \
    return r; \
}

#define NATIVE_BINARY_STRING_OPS(type, CType, V2, fmt) \
NATIVE_BINARY_FUNCTION(type, add, string, CType, str_t, V2, v2s) \
    char *result; \
    asprintf(&result, #fmt "%s", left, right); \
    return s2v(result); \
} \
NATIVE_BINARY_FUNCTION(string, add, type, str_t, CType, v2s, V2) \
    char *result; \
    asprintf(&result, "%s" #fmt, left, right); \
    return s2v(result); \
} \
NATIVE_BINARY_FUNCTION(string, add_equal, type, str_t, CType, v2s, V2) \
    Value r = NATIVE_BINARY_FUNCTION_NAME(string, add, type)(args, argc); \
    set(r, &args[0]); \
    return r; \
}

#define NATIVE_BINARY_EQ_OPS(leftType, rightType, leftV2, rightV2) \
NATIVE_BINARY_FUNCTION(leftType, eq, rightType, leftType, rightType, leftV2, rightV2) \
    if (TYPECHECK(args[0], T_NIL) || TYPECHECK(args[1], T_NIL)) { \
        if (TYPECHECK(args[0], T_NIL) && TYPECHECK(args[1], T_NIL)) { \
            return b2v(true); \
        } \
        return b2v(false); \
    } \
    return b2v(left == right); \
} \
NATIVE_BINARY_FUNCTION(leftType, neq, rightType, leftType, rightType, leftV2, rightV2) \
    if (TYPECHECK(args[0], T_NIL) || TYPECHECK(args[1], T_NIL)) { \
        if (TYPECHECK(args[0], T_NIL) && TYPECHECK(args[1], T_NIL)) { \
            return b2v(false); \
        } \
        return b2v(true); \
    } \
    return b2v(left != right); \
}

#define NATIVE_BINARY_MATH_OPS(leftType, rightType, leftV2, rightV2, outV) \
NATIVE_BINARY_MATH_OP(leftType, add,            rightType, leftV2, rightV2, +,  outV) \
NATIVE_BINARY_MATH_OP(leftType, sub,            rightType, leftV2, rightV2, -,  outV) \
NATIVE_BINARY_MATH_OP(leftType, mul,            rightType, leftV2, rightV2, *,  outV) \
NATIVE_BINARY_MATH_OP(leftType, div,            rightType, leftV2, rightV2, /,  outV) \
NATIVE_BINARY_MATH_OP(leftType, lower,          rightType, leftV2, rightV2, <,  b2v) \
NATIVE_BINARY_MATH_OP(leftType, greater,        rightType, leftV2, rightV2, >,  b2v) \
NATIVE_BINARY_MATH_OP(leftType, lower_equal,    rightType, leftV2, rightV2, <=, b2v) \
NATIVE_BINARY_MATH_OP(leftType, greater_equal,  rightType, leftV2, rightV2, >=, b2v) \
NATIVE_BINARY_SHORTHAND_MATH_OP(leftType, add, rightType, leftV2, rightV2, +, outV) \
NATIVE_BINARY_SHORTHAND_MATH_OP(leftType, sub, rightType, leftV2, rightV2, -, outV) \
NATIVE_BINARY_SHORTHAND_MATH_OP(leftType, mul, rightType, leftV2, rightV2, *, outV) \
NATIVE_BINARY_SHORTHAND_MATH_OP(leftType, div, rightType, leftV2, rightV2, /, outV) \
NATIVE_BINARY_EQ_OPS(leftType, rightType, leftV2, rightV2)

// Int

NATIVE_BINARY_MATH_OPS(int, int, v2i, v2i, i2v)

NATIVE_BINARY_MATH_OP(int, mod, int, v2i, v2i, %, i2v)

NATIVE_BINARY_MATH_OPS(int, double, v2i, v2d, d2v)

NATIVE_BINARY_STRING_OPS(int, int, v2i, %i)

Value unary_prefix_int_negate(Value *args, int argc) {
    return i2v(-v2i(args[0]));
}

#define NATIVE_UNARY_PREFIX_IN_PLACE(type, opName, v2t, t2v, operation) \
Value unary_prefix_##type##_##opName(Value *args, int argc) { \
    Value v = args[0]; \
    Value ov = t2v(v2t(v) operation); \
    set(ov, &v); \
    return v; \
}

#define NATIVE_UNARY_POSTFIX_IN_PLACE(type, opName, v2t, t2v, operation) \
Value unary_postfix_##type##_##opName(Value *args, int argc) { \
    Value v = args[0]; \
    type tv = v2t(v); \
    Value ov = t2v(tv operation); \
    set(ov, &v); \
    return t2v(tv); \
}

NATIVE_UNARY_PREFIX_IN_PLACE(int, decrement, v2i, i2v, -1)

NATIVE_UNARY_PREFIX_IN_PLACE(int, increment, v2i, i2v, +1)

NATIVE_UNARY_POSTFIX_IN_PLACE(int, decrement, v2i, i2v, -1)

NATIVE_UNARY_POSTFIX_IN_PLACE(int, increment, v2i, i2v, +1)

// Double

NATIVE_BINARY_MATH_OPS(double, int, v2d, v2i, d2v)

NATIVE_BINARY_MATH_OPS(double, double, v2d, v2d, d2v)

NATIVE_BINARY_STRING_OPS(double, double, v2d, %lf)

Value unary_prefix_double_negate(Value *args, int argc) {
    return d2v(-v2d(args[0]));
}

NATIVE_UNARY_PREFIX_IN_PLACE(double, decrement, v2d, d2v, -1.0)

NATIVE_UNARY_PREFIX_IN_PLACE(double, increment, v2d, d2v, +1.0)

NATIVE_UNARY_POSTFIX_IN_PLACE(double, decrement, v2d, d2v, -1.0)

NATIVE_UNARY_POSTFIX_IN_PLACE(double, increment, v2d, d2v, +1.0)

// Bool

Value binary_bool_add_string(Value args[], int argc) {
    bool left = v2b(args[0]);
    str_t right = v2s(args[1]);

    str_t lefts = left ? "true" : "false";

    char *result;
    asprintf(&result, "%s%s", lefts, right);
    return s2v(result);
}

Value unary_prefix_bool_invert(Value args[], int argc) {
    return b2v(!v2b(args[0]));
}

Value binary_string_add_bool(Value args[], int argc) {
    str_t left = v2s(args[0]);
    bool right = v2b(args[1]);

    str_t rights = right ? "true" : "false";

    char *result;
    asprintf(&result, "%s%s", left, rights);
    return s2v(result);
}

Value binary_string_add_equal_bool(Value args[], int argc) {
    Value v = binary_string_add_bool(args, argc);

    set(v, &args[0]);

    return v;
}

// String

NATIVE_BINARY_FUNCTION(string, add, string, str_t, str_t, v2s, v2s)
    char *result;
    asprintf(&result, "%s%s", left, right);
    return s2v(result);
}

Value binary_string_add_equal_string(Value args[], int argc) {
    Value v = binary_string_add_string(args, argc);

    set(v, &args[0]);

    return v;
}

Value println_int(Value args[], int argc) {
    int v = v2i(args[0]);

    printf("%i\n", v);

    return n2v();
}

Value println_double(Value args[], int argc) {
    double v = v2d(args[0]);

    printf("%f\n", v);

    return n2v();
}

Value println_string(Value args[], int argc) {
    str_t v = v2s(args[0]);

    printf("%s\n", v);

    return n2v();
}

Value println_bool(Value args[], int argc) {
    bool v = v2b(args[0]);

    if (v) {
        printf("true\n");
    } else {
        printf("false\n");
    }

    return n2v();
}