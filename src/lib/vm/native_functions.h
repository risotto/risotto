//
// Created by rvigee on 2018-09-24.
//

#ifndef RISOTTOPROJECT_NATIVE_FUNCTIONS_H
#define RISOTTOPROJECT_NATIVE_FUNCTIONS_H

#include "value.h"

#define NATIVE_FUNCTION(name) void name(Value args[], int argc, Value ret[])

NATIVE_FUNCTION(vm_stats);
NATIVE_FUNCTION(run_gc);
NATIVE_FUNCTION(args);
NATIVE_FUNCTION(panic);

// String

NATIVE_FUNCTION(binary_string_add_string);
NATIVE_FUNCTION(binary_string_add_int);
NATIVE_FUNCTION(binary_string_add_double);
NATIVE_FUNCTION(binary_string_add_bool);

NATIVE_FUNCTION(string_to_int);

// Int

NATIVE_FUNCTION(binary_int_add_string);

NATIVE_FUNCTION(unary_prefix_int_negate);

NATIVE_FUNCTION(unary_prefix_int_decrement);
NATIVE_FUNCTION(unary_prefix_int_increment);

NATIVE_FUNCTION(unary_postfix_int_decrement);
NATIVE_FUNCTION(unary_postfix_int_increment);

// Double

NATIVE_FUNCTION(binary_double_add_string);

NATIVE_FUNCTION(unary_prefix_double_negate);

NATIVE_FUNCTION(unary_prefix_double_decrement);
NATIVE_FUNCTION(unary_prefix_double_increment);

NATIVE_FUNCTION(unary_postfix_double_decrement);
NATIVE_FUNCTION(unary_postfix_double_increment);

// Bool

NATIVE_FUNCTION(binary_bool_add_string);

NATIVE_FUNCTION(unary_prefix_bool_invert);

// Print

NATIVE_FUNCTION(println_int);
NATIVE_FUNCTION(println_double);
NATIVE_FUNCTION(println_string);
NATIVE_FUNCTION(println_bool);

// Array

NATIVE_FUNCTION(array_size);
NATIVE_FUNCTION(array_add);
NATIVE_FUNCTION(array_at);

#endif //RISOTTOPROJECT_NATIVE_FUNCTIONS_H
