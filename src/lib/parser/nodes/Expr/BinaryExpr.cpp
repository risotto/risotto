//
// Created by rvigee on 10/2/19.
//


#include "BinaryExpr.h"
#include "lib/compiler/Compiler.h"

BinaryExpr::BinaryExpr(Expr *left, Token *op, Expr *right) : CallExpr(op, op, {left, right}) {
}

Expr *BinaryExpr::left() {
    return args[0];
}

Expr *BinaryExpr::right() {
    return args[1];
}

Token *BinaryExpr::op() {
    return identifier;
}

FunctionEntry *BinaryExpr::getFunctionEntry(Compiler *compiler) {
    auto leftTypeEntry = left()->getReturnType(compiler);

    return leftTypeEntry->operators.find(op()->lexeme);
}
