//
// Created by raphael on 10/04/18.
//

#ifndef RISOTTOVM_CHUNK_H
#define RISOTTOVM_CHUNK_H

#include <stdlib.h>
#include "common.h"
#include "value.h"

typedef enum {
    OP_CONST,
    OP_POP,
    OP_COPY,
    OP_NIL,
    OP_RETURN,
    OP_JUMP,
    OP_JUMPT,
    OP_JUMPF,
    OP_CALL,
    OP_END,
    OP_LOAD,
    OP_LOAD_GLOBAL,
    OP_LOAD_LOCAL,
    OP_LOAD_STACK,
    OP_LOAD_INSTANCE,
    OP_NOOP,
    OP_NEW,
    OP_SET,
    OP_FRAME,
    OP_FRAME_END,
    OP_ARRAY,
    OP_ARRAY_INSERT,
    OP_TRUE,
    OP_FALSE,
    OP_EQ_NIL,
    OP_NEQ_NIL,
    OP_DYNAMIC_LOAD_INSTANCE,
    OP_RESOLVE_ADDR,

    Last = OP_RESOLVE_ADDR,
} OpCode;

typedef struct {
    int count;
    int capacity;
    OP_T* code;
    int* lines;
    ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, OP_T byte, int line);
int addConstant(Chunk* chunk, Value value);
void freeChunk(Chunk* chunk);


#endif //RISOTTOVM_CHUNK_H
