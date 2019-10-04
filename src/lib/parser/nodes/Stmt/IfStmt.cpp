//
// Created by rvigee on 10/2/19.
//

#include <lib/vm/chunk.h>
#include "IfStmt.h"
#include "lib/compiler/ByteResolver.h"
#include "lib/compiler/Compiler.h"

IfStmt::IfStmt(Expr *condition, Stmt *thenBranch) : condition(condition), thenBranch(thenBranch) {

}

std::vector<ByteResolver *> IfStmt::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    // Compile condition
    auto conditionBytes = condition->compile(compiler);
    bytes.insert(bytes.end(), conditionBytes.begin(), conditionBytes.end());

    // Compile then branch
    auto thenBytes = thenBranch->compile(compiler);

    // Keep reference to last byte of then
    auto lastThenByte = thenBytes.back();

    // If condition == false, jump after then branch
    bytes.push_back(new ByteResolver(OP_JUMPF, nullptr));
    bytes.push_back(new ByteResolver([lastThenByte](Compiler *c) {
        return c->getAddr(lastThenByte) + 1;
    }, nullptr));

    // Else, run then branch
    bytes.insert(bytes.end(), thenBytes.begin(), thenBytes.end());

    return bytes;
}
