//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}
#include <cassert>
#include <utility>
#include "IfStmt.h"
#include "lib/compiler/ByteResolver.h"
#include "lib/compiler/Compiler.h"

IfStmt::IfStmt(
        Expr *condition,
        Stmt *thenBranch,
        std::vector<IfStmt *> elseifs,
        Stmt *elseBranch
) : condition(condition),
    thenBranch(thenBranch),
    elseifs(std::move(std::move(elseifs))),
    elseBranch(elseBranch) {}

ByteResolver *generateNextBranchByte(std::vector<std::vector<ByteResolver *>> *branchesBytes) {
    auto nextBranchByte = branchesBytes->back().front();

    return new ByteResolver([nextBranchByte](Compiler *c) {
        return c->getAddr(nextBranchByte);
    }, nullptr);
}

ByteResolver *generateExitByte(std::vector<std::vector<ByteResolver *>> *branchesBytes) {
    auto lastElseByte = branchesBytes->front().back();
    return new ByteResolver([lastElseByte](Compiler *c) {
        return c->getAddr(lastElseByte) + 1;
    }, nullptr);
}

std::vector<ByteResolver *> generateElseifBytes(Compiler  *compiler, IfStmt *elseif,
                                        std::vector<std::vector<ByteResolver *>> *branchesBytes) {
    auto bytes = std::vector<ByteResolver *>();

    auto conditionBytes = elseif->condition->compile(compiler);
    bytes.insert(bytes.end(), conditionBytes.begin(), conditionBytes.end());

    bytes.push_back(new ByteResolver(OpCode::OP_JUMPF, nullptr));
    bytes.push_back(generateNextBranchByte(branchesBytes));

    auto thenBytes = elseif->thenBranch->compile(compiler);
    if (thenBytes.empty()) {
        thenBytes.push_back(new ByteResolver(OpCode::OP_NOOP, nullptr));
    }

    bytes.insert(bytes.end(), thenBytes.begin(), thenBytes.end());
    bytes.push_back(new ByteResolver(OpCode::OP_JUMP, nullptr));
    bytes.push_back(generateExitByte(branchesBytes));

    return bytes;
}

std::vector<ByteResolver *> IfStmt::compile(Compiler  *compiler) {
    auto branchesBytes = std::vector<std::vector<ByteResolver *>>();

    auto elseBytes = std::vector<ByteResolver *>();
    if (elseBranch) {
        elseBytes = elseBranch->compile(compiler);
    }

    if (elseBytes.empty()) {
        elseBytes.push_back(new ByteResolver(OpCode::OP_NOOP, nullptr));
    }

    branchesBytes.push_back(elseBytes);

    for (auto it = elseifs.rbegin(); it != elseifs.rend(); ++it) {
        auto elseif = *it;

        assert(elseif->elseBranch == nullptr);
        assert(elseif->elseifs.empty());

        auto branchBytes = generateElseifBytes(compiler, elseif, &branchesBytes);
        branchesBytes.push_back(branchBytes);
    }

    auto mainBytes = generateElseifBytes(compiler, this, &branchesBytes);
    branchesBytes.push_back(mainBytes);

    auto bytes = std::vector<ByteResolver *>();

    for (auto it = branchesBytes.rbegin(); it != branchesBytes.rend(); ++it) {
        auto branchBytes = *it;
        bytes.insert(bytes.end(), branchBytes.begin(), branchBytes.end());
    }

    return bytes;
}
