//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}
#include <lib/compiler/Compiler.h>
#include <lib/compiler/CompilerError.h>
#include "LogicalExpr.h"
#include "lib/compiler/TypeReference.h"

LogicalExpr::LogicalExpr(Expr *left, Token *op, Expr *right): left(left), op(op), right(right) {

}

std::vector<ByteResolver *>
LogicalExpr::generateOr(std::vector<ByteResolver *> leftBytes, std::vector<ByteResolver *> rightBytes, Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto trueByte = new ByteResolver(OP_TRUE, nullptr);
    auto falseByte = new ByteResolver(OP_FALSE, nullptr);

    // LEFT
    bytes.insert(bytes.end(), leftBytes.begin(), leftBytes.end());

    bytes.push_back(new ByteResolver(OP_JUMPT, &op->position));
    bytes.push_back(new ByteResolver([trueByte](Compiler *c){return c->getAddr(trueByte);}, nullptr));

    // RIGHT
    bytes.insert(bytes.end(), rightBytes.begin(), rightBytes.end());

    bytes.push_back(new ByteResolver(OP_JUMPT, nullptr));
    bytes.push_back(new ByteResolver([trueByte](Compiler *c){return c->getAddr(trueByte);}, nullptr));

    // EXIT
    bytes.insert(bytes.end(), falseByte);
    bytes.push_back(new ByteResolver(OP_JUMP, nullptr));
    bytes.push_back(new ByteResolver([trueByte](Compiler *c){return c->getAddr(trueByte) + 1;}, nullptr));

    bytes.insert(bytes.end(), trueByte);

    return bytes;
}

std::vector<ByteResolver *>
LogicalExpr::generateAnd(std::vector<ByteResolver *> leftBytes, std::vector<ByteResolver *> rightBytes, Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto trueByte = new ByteResolver(OP_TRUE, nullptr);
    auto falseByte = new ByteResolver(OP_FALSE, nullptr);

    // LEFT
    bytes.insert(bytes.end(), leftBytes.begin(), leftBytes.end());

    bytes.push_back(new ByteResolver(OP_JUMPF, &op->position));
    bytes.push_back(new ByteResolver([falseByte](Compiler *c){return c->getAddr(falseByte);}, nullptr));

    // RIGHT
    bytes.insert(bytes.end(), rightBytes.begin(), rightBytes.end());

    bytes.push_back(new ByteResolver(OP_JUMPF, nullptr));
    bytes.push_back(new ByteResolver([falseByte](Compiler *c){return c->getAddr(falseByte);}, nullptr));

    // EXIT
    bytes.insert(bytes.end(), trueByte);
    bytes.push_back(new ByteResolver(OP_JUMP, nullptr));
    bytes.push_back(new ByteResolver([falseByte](Compiler *c){return c->getAddr(falseByte) + 1;}, nullptr));

    bytes.insert(bytes.end(), falseByte);

    return bytes;
}

std::vector<ByteResolver *> LogicalExpr::compile(Compiler *compiler) {
    auto leftBytes = left->compile(compiler);
    auto rightBytes = right->compile(compiler);

    switch(op->type) {
        case Token::Type::AND:
            return generateAnd(leftBytes, rightBytes, compiler);
        case Token::Type::OR:
            return generateOr(leftBytes, rightBytes, compiler);
    }

    throw CompilerError("Unhandled logical operator", op->position);
}

TypesEntries LogicalExpr::computeReturnType(Compiler *compiler) {
    return compiler->frame->types.find("bool");
}
