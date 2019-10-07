//
// Created by rvigee on 10/2/19.
//


#include "BinaryExpr.h"
#include "lib/compiler/Compiler.h"

BinaryExpr::BinaryExpr(Expr *left, Token *op, Expr *right) : CallExpr(left, op, {right}, true) {
}

Expr *BinaryExpr::left() {
    return callee;
}

Expr *BinaryExpr::right() {
    return args[0];
}

Token *BinaryExpr::op() {
    return rParen;
}

std::vector<Expr *> BinaryExpr::getArguments(Compiler *compiler) {
    return {left(), right()};
}

FunctionEntry *BinaryExpr::getFunctionEntry(Compiler *compiler) {
    auto leftTypeEntry = left()->getReturnType(compiler);

    if (!leftTypeEntry.single()) {
        throw CompilerError("LHS of binary operation must be a single value", op()->position);
    }

    return leftTypeEntry[0]->operators.find(op()->lexeme, getArgumentsTypes(compiler));
}

FunctionNotFoundError BinaryExpr::getFunctionNotFoundError(Compiler *compiler) {
    return FunctionNotFoundError(op()->lexeme, left()->getReturnType(compiler)[0]->name, getArgumentsTypes(compiler), op());
}
