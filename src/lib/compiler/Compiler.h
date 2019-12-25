//
// Created by rvigee on 10/3/19.
//

#ifndef RISOTTOV2_COMPILER_H
#define RISOTTOV2_COMPILER_H

#include <lib/parser/nodes/Stmt.h>
#include <vector>
#include <string>
#include "ByteResolver.h"
#include "Frame.h"
#include "TypesManager.h"

extern "C" {
#include <lib/vm/chunk.h>
}

class Compiler {
private:
    std::vector<Stmt *> stmts;
    std::vector<ByteResolver *> bytes;

public:
    Chunk chunk;
    Frame *frame;
    TypesManager *typesManager;

    explicit Compiler(std::vector<Stmt *> stmts);

    Chunk compile();

    OP_T registerConst(Value v);

    OP_T getAddr(ByteResolver *byte);
};


#endif //RISOTTOV2_COMPILER_H
