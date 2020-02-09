//
// Created by raphael on 10/04/18.
//

#ifndef RISOTTOVM_CHUNK_H
#define RISOTTOVM_CHUNK_H

#include <stdlib.h>
#include "common.h"
#include "value.h"
#include "position.h"

#define OPCODES(X) \
    X(OP_CONST) \
    X(OP_POP) \
    X(OP_COPY) \
    X(OP_NIL) \
    X(OP_RETURN) \
    X(OP_JUMP) \
    X(OP_JUMPT) \
    X(OP_JUMPF) \
    X(OP_CALL) \
    X(OP_END) \
    X(OP_LOAD) \
    X(OP_LOAD_LOCAL) \
    X(OP_LOAD_STACK) \
    X(OP_LOAD_INSTANCE) \
    X(OP_NOOP) \
    X(OP_NEW) \
    X(OP_SET) \
    X(OP_FRAME) \
    X(OP_FRAME_END) \
    X(OP_ARRAY) \
    X(OP_TRUE) \
    X(OP_FALSE) \
    X(OP_EQ) \
    X(OP_NEQ) \
    X(OP_EQ_NIL) \
    X(OP_NEQ_NIL) \
    X(OP_RESOLVE_ADDR) \
    X(OP_IADD) \
    X(OP_ISUB) \
    X(OP_IMUL) \
    X(OP_IDIV) \
    X(OP_ILT) \
    X(OP_IGT) \
    X(OP_IMOD) \
    X(OP_DADD) \
    X(OP_DSUB) \
    X(OP_DMUL) \
    X(OP_DDIV) \
    X(OP_DLT) \
    X(OP_DGT) \
    X(OP_B_AND) \
    X(OP_B_OR) \
    X(OP_B_XOR) \
    X(OP_B_SHIFTL) \
    X(OP_B_SHIFTR) \
    X(OP_B_NOT) \
    X(OP_I2D) \
    X(OP_D2I) \

#define X_LIST(X) X,

typedef enum {
    OPCODES(X_LIST)

    Last = OP_D2I,
} OpCode;

typedef struct {
    int count;
    int capacity;
    OP_T *code;
    Position *positions;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, OP_T byte, Position line);

int addConstant(Chunk *chunk, Value value);

void freeChunk(Chunk *chunk);


#endif //RISOTTOVM_CHUNK_H
