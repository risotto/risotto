//
// Created by rvigee on 10/2/19.
//


#include "BinaryExpr.h"
#include "lib/compiler/Compiler.h"

BinaryExpr::BinaryExpr(Expr *left, Token *op, Expr *right) : CallExpr(left, op, {right}) {
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

    return leftTypeEntry[0]->operators.find(op()->lexeme, getArgumentsTypes(compiler));
}
