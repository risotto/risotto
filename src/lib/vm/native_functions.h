//
// Created by rvigee on 2018-09-24.
//

#ifndef RISOTTOPROJECT_NATIVE_FUNCTIONS_H
#define RISOTTOPROJECT_NATIVE_FUNCTIONS_H

#include "value.h"

// String

Value binary_string_add_string(Value args[], int argc);
Value binary_string_add_int(Value args[], int argc);
Value binary_string_add_double(Value args[], int argc);
Value binary_string_add_bool(Value args[], int argc);

Value binary_string_add_equal_string(Value args[], int argc);
Value binary_string_add_equal_int(Value args[], int argc);
Value binary_string_add_equal_double(Value args[], int argc);
Value binary_string_add_equal_bool(Value args[], int argc);

// Int

Value binary_int_add_string(Value args[], int argc);

Value binary_int_add_int(Value args[], int argc);
Value binary_int_add_double(Value args[], int argc);

Value binary_int_sub_int(Value args[], int argc);
Value binary_int_sub_double(Value args[], int argc);

Value binary_int_mul_int(Value args[], int argc);
Value binary_int_mul_double(Value args[], int argc);

Value binary_int_div_int(Value args[], int argc);
Value binary_int_div_double(Value args[], int argc);

Value binary_int_mod_int(Value args[], int argc);

Value binary_int_lower_int(Value args[], int argc);
Value binary_int_lower_double(Value args[], int argc);

Value binary_int_greater_int(Value args[], int argc);
Value binary_int_greater_double(Value args[], int argc);

Value binary_int_lower_equal_int(Value args[], int argc);
Value binary_int_lower_equal_double(Value args[], int argc);

Value binary_int_greater_equal_int(Value args[], int argc);
Value binary_int_greater_equal_double(Value args[], int argc);

Value binary_int_eq_int(Value args[], int argc);
Value binary_int_eq_double(Value args[], int argc);

Value binary_int_neq_int(Value args[], int argc);
Value binary_int_neq_double(Value args[], int argc);

Value unary_prefix_int_negate(Value *args, int argc);

Value unary_prefix_int_decrement(Value *args, int argc);
Value unary_prefix_int_increment(Value *args, int argc);

Value unary_postfix_int_decrement(Value *args, int argc);
Value unary_postfix_int_increment(Value *args, int argc);

Value binary_int_add_equal_int(Value args[], int argc);
Value binary_int_sub_equal_int(Value args[], int argc);
Value binary_int_mul_equal_int(Value args[], int argc);
Value binary_int_div_equal_int(Value args[], int argc);

Value binary_int_add_equal_double(Value args[], int argc);
Value binary_int_sub_equal_double(Value args[], int argc);
Value binary_int_mul_equal_double(Value args[], int argc);
Value binary_int_div_equal_double(Value args[], int argc);

// Double

Value binary_double_add_string(Value args[], int argc);

Value binary_double_add_int(Value args[], int argc);
Value binary_double_add_double(Value args[], int argc);

Value binary_double_sub_int(Value args[], int argc);
Value binary_double_sub_double(Value args[], int argc);

Value binary_double_mul_int(Value args[], int argc);
Value binary_double_mul_double(Value args[], int argc);

Value binary_double_div_int(Value args[], int argc);
Value binary_double_div_double(Value args[], int argc);

Value binary_double_lower_int(Value args[], int argc);
Value binary_double_lower_double(Value args[], int argc);

Value binary_double_greater_int(Value args[], int argc);
Value binary_double_greater_double(Value args[], int argc);

Value binary_double_lower_equal_int(Value args[], int argc);
Value binary_double_lower_equal_double(Value args[], int argc);

Value binary_double_greater_equal_int(Value args[], int argc);
Value binary_double_greater_equal_double(Value args[], int argc);

Value binary_double_eq_int(Value args[], int argc);
Value binary_double_eq_double(Value args[], int argc);

Value binary_double_neq_int(Value args[], int argc);
Value binary_double_neq_double(Value args[], int argc);

Value unary_prefix_double_negate(Value *args, int argc);

Value unary_prefix_double_decrement(Value *args, int argc);
Value unary_prefix_double_increment(Value *args, int argc);

Value unary_postfix_double_decrement(Value *args, int argc);
Value unary_postfix_double_increment(Value *args, int argc);

Value binary_double_add_equal_int(Value args[], int argc);
Value binary_double_sub_equal_int(Value args[], int argc);
Value binary_double_mul_equal_int(Value args[], int argc);
Value binary_double_div_equal_int(Value args[], int argc);

Value binary_double_add_equal_double(Value args[], int argc);
Value binary_double_sub_equal_double(Value args[], int argc);
Value binary_double_mul_equal_double(Value args[], int argc);
Value binary_double_div_equal_double(Value args[], int argc);

// Bool

Value binary_bool_add_string(Value args[], int argc);

Value unary_prefix_bool_invert(Value args[], int argc);

Value print_int(Value args[], int argc);

#endif //RISOTTOPROJECT_NATIVE_FUNCTIONS_H
