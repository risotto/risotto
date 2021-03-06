//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}

#include "ExpressionStmt.h"

ExpressionStmt::ExpressionStmt(Expr *expr) : expr(expr) {

}

std::vector<ByteResolver *> ExpressionStmt::compile(Compiler *compiler) {
    auto bytes = expr->compile(compiler);

    auto exprType = expr->getReturnType(compiler);

    if (!exprType.empty()) {
        bytes.push_back(new ByteResolver(OP_POP));
        bytes.push_back(new ByteResolver(exprType.size()));
    }

    return bytes;
}

void ExpressionStmt::symbolize(Compiler *compiler) {
    expr->symbolize(compiler);
}
