//
// Created by rvigee on 3/14/20.
//

#ifndef RISOTTOV2_VTABLE_H
#define RISOTTOV2_VTABLE_H

#include "value.h"

typedef struct {
    int vaddr;
    Value addr;
} vtable_entry;

typedef vec_t(vtable_entry) vtable_entry_vec_t;

typedef struct {
    int argc;
    int retc;

    OP_T *ip;
    Value *sp;
    Value *fp;
} FunctionCall;

typedef vec_t(FunctionCall) function_call_vec_t;

struct vtable {
    unsigned int size;
    vtable_entry_vec_t addrs;
};

#endif //RISOTTOV2_VTABLE_H
