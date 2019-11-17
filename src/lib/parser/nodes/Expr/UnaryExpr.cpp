//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>
#include "UnaryExpr.h"

UnaryExpr::UnaryExpr(Token *op, Expr *right) : GetCallExpr(right, op, op, op, {}) {

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
    auto rightTypes = right()->getReturnType(compiler);

    if (!rightTypes.single()) {
        throw CompilerError("RHS of unary operation must be a single value", op()->position);
    }

    if (auto receiver = dynamic_cast<ReceiverTypeReference *>(rightTypes[0])) {
        return receiver->findPrefix(compiler->frame, op()->lexeme, getArgumentsTypes(compiler));
    }

    throw CompilerError("RHS must be a receiver", op()->position);
}

FunctionNotFoundError UnaryExpr::getFunctionNotFoundError(Compiler *compiler) {
    auto rightReturnType = right()->getReturnType(compiler);

    return FunctionNotFoundError(op()->lexeme + rightReturnType[0]->toString(), getArgumentsTypes(compiler), op());
}

VariableEntry *UnaryExpr::getVariableEntry(Compiler *compiler) {
    return nullptr;
}

void UnaryExpr::walk(ASTWalker *walker) {
    walker->walkUnaryExpr(this);
}
