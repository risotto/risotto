//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}
#include "SetExpr.h"

SetExpr::SetExpr(Expr *left, Token *op, Expr *right): left(left), op(op), right(right) {

}

std::vector<ByteResolver *> SetExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto objectBytes = left->compile(compiler);
    bytes.insert(bytes.end(), objectBytes.begin(), objectBytes.end());

    auto valueBytes = right->compile(compiler);
    bytes.insert(bytes.end(), valueBytes.begin(), valueBytes.end());

    bytes.push_back(new ByteResolver(OP_SET, &op->position));

    return bytes;
}

