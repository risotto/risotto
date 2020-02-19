//
// Created by rvigee on 10/2/19.
//

#include <cassert>
#include <utility>
#include "IfStmt.h"
#include "lib/compiler/ByteResolver.h"
#include "lib/compiler/Compiler.h"

IfBranch::IfBranch(Expr *condition, Stmt *body) : condition(condition), body(body) {}

IfStmt::IfStmt(
        Expr *condition,
        Stmt *thenBranch,
        std::vector<IfStmt *> elseifs,
        Stmt *elseBranch
) : condition(condition),
    thenBranch(thenBranch),
    elseifs(elseifs),
    elseBranch(elseBranch) {

    branches.push_back(new IfBranch(condition, thenBranch));

    for (auto elseif: elseifs) {
        assert(elseif->elseifs.empty());
        assert(elseif->elseBranch == nullptr);

        branches.push_back(new IfBranch(elseif->condition, elseif->thenBranch));
    }

    if (elseBranch != nullptr) {
        branches.push_back(new IfBranch(nullptr, elseBranch));
    }
}

ByteResolver *IfStmt::generateNextBranchByte(IfBranch *currentBranch) {
    if (currentBranch == branches.back()) {
        return generateExitByte(currentBranch);
    }

    return new ByteResolver([this, currentBranch](Compiler *c) {
        ByteResolver *target = nullptr;

        bool found = false;
        for (auto it = branches.begin(); it != branches.end(); ++it) {
            auto br = *it;
            auto isLast = std::next(it) == branches.end();

            if (found) {
                if (!br->bytes.empty()) {
                    target = br->bytes.front();
                    break; // We got it!
                } else if (isLast) {
                    return generateExitByte(currentBranch)->resolver(c); // We didnt found any next branch with bytes, exit
                }
            } else {
                if (br == currentBranch) {
                    found = true;
                }
            }
        }

        assert(target != nullptr);

        return c->getAddr(target);
    });
}

ByteResolver *IfStmt::generateExitByte(IfBranch *branch) {
//    if (branch == branches.back()) {
//        return nullptr;
//    }

    return new ByteResolver([this](Compiler *c) {
        for (auto it = branches.rbegin(); it != branches.rend(); ++it) {
            auto br = *it;

            if (!br->bytes.empty()) {
                return c->getAddr(br->bytes.back()) + 1;
            }
        }
    });
}

std::vector<ByteResolver *> IfStmt::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    for (auto branch: branches) {
        auto branchBytes = std::vector<ByteResolver *>();

        if (branch->condition != nullptr) {
            auto conditionBytes = branch->condition->compile(compiler);
            branchBytes.insert(branchBytes.end(), conditionBytes.begin(), conditionBytes.end());

            auto nextBranchByte = generateNextBranchByte(branch);
            if (nextBranchByte != nullptr) {
                branchBytes.push_back(new ByteResolver(OpCode::OP_JUMPF));
                branchBytes.push_back(nextBranchByte);
            }
        }

        auto thenBytes = branch->body->compile(compiler);
        branchBytes.insert(branchBytes.end(), thenBytes.begin(), thenBytes.end());

        auto exitByte = generateExitByte(branch);
        if (exitByte != nullptr) {
            branchBytes.push_back(new ByteResolver(OpCode::OP_JUMP));
            branchBytes.push_back(exitByte);
        }

        branch->bytes = branchBytes;
        bytes.insert(bytes.end(), branchBytes.begin(), branchBytes.end());
    }

    return bytes;
}

void IfStmt::symbolize(Compiler *compiler) {
    for (auto branch: branches) {
        if (branch->condition != nullptr) {
            branch->condition->symbolize(compiler);
        }

        branch->body->symbolize(compiler);
    }
}
