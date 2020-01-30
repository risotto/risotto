//
// Created by raphael on 10/04/18.
//

#include "chunk.h"
#include "memory.h"
#include "value.h"
#include "position.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->positions = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, OP_T byte, Position position) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(chunk->code, OP_T,
                                 oldCapacity, chunk->capacity);
        chunk->positions = GROW_ARRAY(chunk->positions, Position,
                                  oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->positions[chunk->count] = position;
    chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.object.size - 1;
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(OP_T, chunk->code, chunk->capacity);
    FREE_ARRAY(Position, chunk->positions, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}
