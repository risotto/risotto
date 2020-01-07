//
// Created by raphael on 10/04/18.
//

#ifndef RISOTTOVM_DEBUG_H
#define RISOTTOVM_DEBUG_H

#include "chunk.h"

void printValue(Value value);
void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int i);
char *getName(OP_T instruction);

void printVtable(Value v);

#endif //RISOTTOVM_DEBUG_H
