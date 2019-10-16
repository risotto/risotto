//
// Created by rvigee on 10/2/19.
//


#include <lib/compiler/utils/Utils.h>
#include "BinaryExpr.h"
#include "lib/compiler/Compiler.h"

BinaryExpr::BinaryExpr(Expr *left, Token *op, Expr *right) : GetCallExpr(left, op, op, op, {right}) {
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

FunctionEntry *BinaryExpr::getFunctionEntry(Compiler *compiler) {
    auto leftReturnType = left()->getReturnType(compiler);

    if (!leftReturnType.single()) {
        throw CompilerError("LHS of binary operation must be a single value", op()->position);
    }

    if (auto receiver = dynamic_cast<ReceiverTypeReference *>(leftReturnType[0])) {
        return receiver->findOperator(compiler->frame, op()->lexeme, getArgumentsTypes(compiler));
    }

    throw CompilerError("LHS must be a receiver", op()->position);
}

FunctionNotFoundError BinaryExpr::getFunctionNotFoundError(Compiler *compiler) {
    auto leftReturnType = left()->getReturnType(compiler);
    auto args = getArgumentsTypes(compiler);

    return FunctionNotFoundError(op()->lexeme, leftReturnType[0]->toString(), args, op());
}

VariableEntry *BinaryExpr::getVariableEntry(Compiler *compiler) {
    return nullptr;
}
