//
// Created by rvigee on 10/2/19.
//

#include "UnaryExpr.h"

UnaryExpr::UnaryExpr(Token *op, Expr *right): CallExpr(right, op, {}, true) {

}

Expr *UnaryExpr::right() {
    return callee;
}

Token *UnaryExpr::op() {
    return rParen;
}

std::vector<Expr *> UnaryExpr::getArguments(Compiler *compiler) {
    return {right()};
}

FunctionEntry *UnaryExpr::getFunctionEntry(Compiler *compiler) {
    auto leftTypes = right()->getReturnType(compiler);

    if (!leftTypes.single()) {
        throw CompilerError("RHS of unary operation must be a single value", op()->position);
    }

    return leftTypes[0]->findPrefix(compiler, op()->lexeme, getArgumentsTypes(compiler));
}

FunctionNotFoundError UnaryExpr::getFunctionNotFoundError(Compiler *compiler) {
    return FunctionNotFoundError(op()->lexeme, "", getArgumentsTypes(compiler), op());
}
