//
// Created by rvigee on 10/2/19.
//

#include "BlockStmt.h"

#include <utility>
#include <lib/compiler/Compiler.h>

BlockStmt::BlockStmt(std::vector<Stmt *> stmts) : stmts(std::move(stmts)) {

}

std::vector<ByteResolver *> BlockStmt::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    compiler->frame = frame;
    bytes.push_back(new ByteResolver(OP_FRAME, TODO_POSITION));

    for (auto stmt : stmts) {
        auto stmtBytes = stmt->compile(compiler);
        bytes.insert(bytes.end(), stmtBytes.begin(), stmtBytes.end());
    }

    bytes.push_back(new ByteResolver(OP_FRAME_END, TODO_POSITION));
    compiler->frame = compiler->frame->parent;

    return bytes;
}

void BlockStmt::symbolize(Compiler *compiler) {
    frame = new Frame(compiler->frame);
    compiler->frame = frame;

    for (auto stmt: stmts) {
        stmt->symbolize(compiler);
    }

    compiler->frame = compiler->frame->parent;
}
