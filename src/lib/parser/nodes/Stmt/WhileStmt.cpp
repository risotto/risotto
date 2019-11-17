//
// Created by rvigee on 10/7/19.
//

#include <lib/vm/chunk.h>
#include "WhileStmt.h"
#include "lib/compiler/Compiler.h"

WhileStmt::WhileStmt(Expr *condition, BlockStmt *body) : condition(condition), body(body) {

}

std::vector<ByteResolver *> WhileStmt::compile(Compiler *compiler) {
    auto conditionBytes = condition->compile(compiler);

    auto bodyBytes = body->compile(compiler);

    auto loopJumpBytes = std::vector<ByteResolver *>();
    loopJumpBytes.push_back(new ByteResolver(OP_JUMP, nullptr));
    loopJumpBytes.push_back(new ByteResolver([conditionBytes](Compiler *c) {
        return c->getAddr(conditionBytes.front());
    }, nullptr));

    auto bytes = std::vector<ByteResolver *>();

    // Condition
    bytes.insert(bytes.end(), conditionBytes.begin(), conditionBytes.end());

    bytes.push_back(new ByteResolver(OP_JUMPF, nullptr));
    bytes.push_back(new ByteResolver([loopJumpBytes](Compiler *c) {
        return c->getAddr(loopJumpBytes.back()) + 1;
    }, nullptr));

    // Body
    bytes.insert(bytes.end(), bodyBytes.begin(), bodyBytes.end());

    // Loop jump
    bytes.insert(bytes.end(), loopJumpBytes.begin(), loopJumpBytes.end());

    return bytes;
}

void WhileStmt::walk(ASTWalker *walker) {
    walker->walkWhileStmt(this);
}
