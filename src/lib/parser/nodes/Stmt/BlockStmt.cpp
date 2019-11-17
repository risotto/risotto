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

    auto previousFrame = compiler->frame;
    compiler->frame = new Frame(previousFrame);
    bytes.push_back(new ByteResolver(OP_FRAME, nullptr));

    for (auto stmt : stmts) {
        auto stmtBytes = stmt->compile(compiler);
        bytes.insert(bytes.end(), stmtBytes.begin(), stmtBytes.end());
    }

    bytes.push_back(new ByteResolver(OP_FRAME_END, nullptr));
    compiler->frame = previousFrame;

    return bytes;
}

void BlockStmt::walk(ASTWalker *walker) {
    walker->walkBlockStmt(this);
}
