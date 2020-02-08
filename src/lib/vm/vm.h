//
// Created by raphael on 10/04/18.
//

#ifndef RISOTTOVM_VM_H
#define RISOTTOVM_VM_H

#include "chunk.h"
#include "value.h"
#include "stats.h"

#define STACK_MAX 256
#define INITIAL_GC_THRESHOLD 1000

typedef enum {
    VMNone             = 0,
    TraceExecution     = 1 << 0,
    BenchmarkExecution = 1 << 1,
} VMFlags;

typedef struct {
    unsigned int flags;
    Chunk* chunk;
    OP_T* ip;
    Value stack[STACK_MAX];
    Value *sp;
    Value *fp;
    Object *firstObject;
    /* The total number of currently allocated values. */
    int numObjects;
    /* The number of values required to trigger a GC. */
    int maxObjects;
    int (*printf) (const char *, ...);
    ValueArray *args;
    Stats *stats;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM(unsigned int flags, int (*printf) (const char *, ...), ValueArray *args, Stats *stats);
VM *getVM();
void freeVM();
InterpretResult interpret(Chunk* chunk, long addr);
void loadInstance(int index);
void set(Value origin, Value *target);
void push(Value value);
Value pop();
Value *popp();

void registerObject(Object *object);
void markAll();
void mark(Object* value);
void markValue(Value *value);
void sweep();
void gc();

bool hasFlag(VMFlags flag);

void cframe();
void dframe();

#endif //RISOTTOVM_VM_H
