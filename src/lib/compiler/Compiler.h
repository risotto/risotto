//
// Created by rvigee on 10/3/19.
//

#ifndef RISOTTOV2_COMPILER_H
#define RISOTTOV2_COMPILER_H

#include <lib/parser/nodes/Stmt.h>
#include <vector>
#include "ByteResolver.h"
#include "Frame.h"

extern "C" {
#include <lib/vm/chunk.h>
}

class Compiler {
private:
    std::vector<Stmt *> stmts;
    std::vector<ByteResolver *> bytes;

    std::vector<FunctionEntry *> functions;
public:
    Chunk chunk;
    Frame *frame;

    explicit Compiler(std::vector<Stmt *> stmts);

    Chunk compile();

    OP_T registerConst(Value v);

    OP_T getAddr(ByteResolver *byte);

    unsigned long registerFunctionEntry(FunctionEntry * entry);
};


#endif //RISOTTOV2_COMPILER_H
