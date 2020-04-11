//
// Created by raphael on 10/04/18.
//

#ifndef RISOTTOVM_VM_H
#define RISOTTOVM_VM_H

#include "chunk.h"
#include "value.h"
#include "types.h"

#define STACK_MAX 256
#define INITIAL_GC_THRESHOLD 1000

typedef enum {
    VMNone             = 0,
    TraceExecution     = 1 << 0,
    BenchmarkExecution = 1 << 1,
} VMFlags;

typedef int (*RisottoPrintf)(const char *, ...);

typedef struct {
    unsigned int flags;
    Chunk* chunk;
    OP_T* ip;
    Value stack[STACK_MAX];
    Value *maxstack;
    Value *sp;
    Value *fp;
    function_call_vec_t fcs;
    GCObject *firstObject;
    /* The total number of currently allocated values. */
    int numObjects;
    /* The number of values required to trigger a GC. */
    int maxObjects;
    RisottoPrintf printf;
    ValueArray *args;
    const PrimitiveTypes *primitiveTypes;
} VM;

extern VM vm;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM(unsigned int flags, RisottoPrintf printf, ValueArray *args);
VM *getVM();
void freeVM();
InterpretResult interpret(Chunk* chunk, long addr);
void loadInstance(int index);
void push(Value value);
void pushm(Value *values, unsigned int length);
Value pop();

void registerObject(GCObject *object);
void markAll();
bool markGCObject(GCObject* obj);
void markObject(Object* object);
void markArray(ValueArray* array);
void markValue(Value *value);
void sweep();
void gc();

bool hasFlag(VMFlags flag);

void cframe();
void dframe();

#endif //RISOTTOVM_VM_H
