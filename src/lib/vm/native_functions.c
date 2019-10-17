//
// Created by rvigee on 2018-09-24.
//

#include <stdio.h>
#include <string.h>
#include "value.h"
#include "vm.c"

#define str_t const char *

#define ret0 NativeFunctionReturn __r = {.c = 0, .values = 0}; return __r;
#define ret(vc, ...) \
    Value __lvalues[] = {__VA_ARGS__}; \
    Value *__values = malloc(sizeof(Value)*vc); \
    memcpy(__values, __lvalues, sizeof(__lvalues)); \
    NativeFunctionReturn __r = {.c = vc, .values = __values}; \
    return __r;

NativeFunctionReturn vm_stats(Value args[], int argc) {
    printf("Objects count: %i\n", vm.numObjects);
    printf("Max Objects: %i\n", vm.maxObjects);

    ret0
}

NativeFunctionReturn run_gc(Value args[], int argc) {
    gc();

    ret0
}

#define NATIVE_BINARY_FUNCTION_NAME(leftType, opName, rightType) binary_##leftType##_##opName##_##rightType

#define NATIVE_BINARY_FUNCTION(leftType, opName, rightType, leftCType, rightCType, leftV2, rightV2) \
NativeFunctionReturn NATIVE_BINARY_FUNCTION_NAME(leftType, opName, rightType)(Value args[], int argc) { \
    leftCType left = leftV2(args[0]); \
    rightCType right = rightV2(args[1]);

#define NATIVE_BINARY_MATH_OP(leftType, opName, rightType, leftV2, rightV2, op, outV) \
NATIVE_BINARY_FUNCTION(leftType, opName, rightType, leftType, rightType, leftV2, rightV2) \
    ret(1, outV(left op right)); \
}

#define NATIVE_BINARY_SHORTHAND_MATH_OP(leftType, opName, rightType, leftV2, rightV2, op, outV) \
NATIVE_BINARY_FUNCTION(leftType, opName##_equal, rightType, leftType, rightType, leftV2, rightV2) \
    NativeFunctionReturn r = NATIVE_BINARY_FUNCTION_NAME(leftType, opName, rightType)(args, argc); \
    set(r.values[0], &args[0]); \
    ret(1, r.values[0]); \
}

#define NATIVE_BINARY_STRING_OPS(type, CType, V2, fmt) \
NATIVE_BINARY_FUNCTION(type, add, string, CType, str_t, V2, v2s) \
    char *result; \
    asprintf(&result, #fmt "%s", left, right); \
    ret(1, s2v(result)); \
} \
NATIVE_BINARY_FUNCTION(string, add, type, str_t, CType, v2s, V2) \
    char *result; \
    asprintf(&result, "%s" #fmt, left, right); \
    ret(1, s2v(result)); \
} \
NATIVE_BINARY_FUNCTION(string, add_equal, type, str_t, CType, v2s, V2) \
    NativeFunctionReturn r = NATIVE_BINARY_FUNCTION_NAME(string, add, type)(args, argc); \
    set(r.values[0], &args[0]); \
    ret(1, r.values[0]); \
}

#define NATIVE_BINARY_EQ_OPS(leftType, rightType, leftV2, rightV2) \
NATIVE_BINARY_FUNCTION(leftType, eq, rightType, leftType, rightType, leftV2, rightV2) \
    if (TYPECHECK(args[0], T_NIL) || TYPECHECK(args[1], T_NIL)) { \
        if (TYPECHECK(args[0], T_NIL) && TYPECHECK(args[1], T_NIL)) { \
            ret(1, b2v(true)); \
        } \
        ret(1, b2v(false)); \
    } \
    ret(1, b2v(left == right)); \
} \
NATIVE_BINARY_FUNCTION(leftType, neq, rightType, leftType, rightType, leftV2, rightV2) \
    if (TYPECHECK(args[0], T_NIL) || TYPECHECK(args[1], T_NIL)) { \
        if (TYPECHECK(args[0], T_NIL) && TYPECHECK(args[1], T_NIL)) { \
            ret(1, b2v(false)); \
        } \
        ret(1, b2v(true)); \
    } \
    ret(1, b2v(left != right)); \
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

NativeFunctionReturn unary_prefix_int_negate(Value *args, int argc) {
    ret(1, i2v(-v2i(args[0])));
}

#define NATIVE_UNARY_PREFIX_IN_PLACE(type, opName, v2t, t2v, operation) \
NativeFunctionReturn unary_prefix_##type##_##opName(Value *args, int argc) { \
    Value v = args[0]; \
    Value ov = t2v(v2t(v) operation); \
    set(ov, &v); \
    ret(1, v); \
}

#define NATIVE_UNARY_POSTFIX_IN_PLACE(type, opName, v2t, t2v, operation) \
NativeFunctionReturn unary_postfix_##type##_##opName(Value *args, int argc) { \
    Value v = args[0]; \
    type tv = v2t(v); \
    Value ov = t2v(tv operation); \
    set(ov, &v); \
    ret(1, t2v(tv)); \
}

NATIVE_UNARY_PREFIX_IN_PLACE(int, decrement, v2i, i2v, -1)

NATIVE_UNARY_PREFIX_IN_PLACE(int, increment, v2i, i2v, +1)

NATIVE_UNARY_POSTFIX_IN_PLACE(int, decrement, v2i, i2v, -1)

NATIVE_UNARY_POSTFIX_IN_PLACE(int, increment, v2i, i2v, +1)

// Double

NATIVE_BINARY_MATH_OPS(double, int, v2d, v2i, d2v)

NATIVE_BINARY_MATH_OPS(double, double, v2d, v2d, d2v)

NATIVE_BINARY_STRING_OPS(double, double, v2d, %lf)

NativeFunctionReturn unary_prefix_double_negate(Value *args, int argc) {
    ret(1, d2v(-v2d(args[0])));
}

NATIVE_UNARY_PREFIX_IN_PLACE(double, decrement, v2d, d2v, -1.0)

NATIVE_UNARY_PREFIX_IN_PLACE(double, increment, v2d, d2v, +1.0)

NATIVE_UNARY_POSTFIX_IN_PLACE(double, decrement, v2d, d2v, -1.0)

NATIVE_UNARY_POSTFIX_IN_PLACE(double, increment, v2d, d2v, +1.0)

// Bool

NativeFunctionReturn binary_bool_add_string(Value args[], int argc) {
    bool left = v2b(args[0]);
    str_t right = v2s(args[1]);

    str_t lefts = left ? "true" : "false";

    char *result;
    asprintf(&result, "%s%s", lefts, right);
    ret(1, s2v(result));
}

NativeFunctionReturn unary_prefix_bool_invert(Value args[], int argc) {
    ret(1, b2v(!v2b(args[0])));
}

NativeFunctionReturn binary_string_add_bool(Value args[], int argc) {
    str_t left = v2s(args[0]);
    bool right = v2b(args[1]);

    str_t rights = right ? "true" : "false";

    char *result;
    asprintf(&result, "%s%s", left, rights);
    ret(1, s2v(result));
}

NativeFunctionReturn binary_string_add_equal_bool(Value args[], int argc) {
    NativeFunctionReturn v = binary_string_add_bool(args, argc);

    set(v.values[0], &args[0]);

    ret(1, v.values[0]);
}

// String

NATIVE_BINARY_FUNCTION(string, add, string, str_t, str_t, v2s, v2s)
    char *result;
    asprintf(&result, "%s%s", left, right);
    ret(1, s2v(result));
}

NativeFunctionReturn binary_string_add_equal_string(Value args[], int argc) {
    NativeFunctionReturn v = binary_string_add_string(args, argc);

    set(v.values[0], &args[0]);

    ret(1, v.values[0])
}

NativeFunctionReturn println_int(Value args[], int argc) {
    int v = v2i(args[0]);

    printf("%i\n", v);

    ret0
}

NativeFunctionReturn println_double(Value args[], int argc) {
    double v = v2d(args[0]);

    printf("%f\n", v);

    ret0
}

NativeFunctionReturn println_string(Value args[], int argc) {
    str_t v = v2s(args[0]);

    printf("%s\n", v);

    ret0
}

NativeFunctionReturn println_bool(Value args[], int argc) {
    bool v = v2b(args[0]);

    if (v) {
        printf("true\n");
    } else {
        printf("false\n");
    }

    ret0
}
