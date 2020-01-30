//
// Created by rvigee on 2018-09-24.
//

#ifndef RISOTTOPROJECT_NATIVE_FUNCTIONS_H
#define RISOTTOPROJECT_NATIVE_FUNCTIONS_H

#include "value.h"

void vm_stats(Value args[], int argc, Value ret[]);
void run_gc(Value args[], int argc, Value ret[]);

// String

void binary_string_add_string(Value args[], int argc, Value ret[]);
void binary_string_add_int(Value args[], int argc, Value ret[]);
void binary_string_add_double(Value args[], int argc, Value ret[]);
void binary_string_add_bool(Value args[], int argc, Value ret[]);

// Int

void binary_int_add_string(Value args[], int argc, Value ret[]);

void unary_prefix_int_negate(Value args[], int argc, Value ret[]);

void unary_prefix_int_decrement(Value args[], int argc, Value ret[]);
void unary_prefix_int_increment(Value args[], int argc, Value ret[]);

void unary_postfix_int_decrement(Value args[], int argc, Value ret[]);
void unary_postfix_int_increment(Value args[], int argc, Value ret[]);

// Double

void binary_double_add_string(Value args[], int argc, Value ret[]);

void unary_prefix_double_negate(Value args[], int argc, Value ret[]);

void unary_prefix_double_decrement(Value args[], int argc, Value ret[]);
void unary_prefix_double_increment(Value args[], int argc, Value ret[]);

void unary_postfix_double_decrement(Value args[], int argc, Value ret[]);
void unary_postfix_double_increment(Value args[], int argc, Value ret[]);

// Bool

void binary_bool_add_string(Value args[], int argc, Value ret[]);

void unary_prefix_bool_invert(Value args[], int argc, Value ret[]);

// Print

void println_int(Value args[], int argc, Value ret[]);
void println_double(Value args[], int argc, Value ret[]);
void println_string(Value args[], int argc, Value ret[]);
void println_bool(Value args[], int argc, Value ret[]);

// Array

void array_size(Value args[], int argc, Value ret[]);
void array_add(Value args[], int argc, Value ret[]);
void array_at(Value args[], int argc, Value ret[]);

#endif //RISOTTOPROJECT_NATIVE_FUNCTIONS_H
