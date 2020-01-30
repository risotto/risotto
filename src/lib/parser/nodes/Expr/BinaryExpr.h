//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_BINARYEXPR_H
#define RISOTTOV2_BINARYEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>
#include "CallExpr.h"
#include "GetCallExpr.h"

class BinaryExpr : public GetCallExpr {
public:
    BinaryExpr(Expr *left, Token *op, Expr *right);

    Expr *left();

    Expr *right();

    Token *op();

    FunctionEntry *getFunctionEntry(Compiler *compiler) override;

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

protected:
    VariableEntry *getVariableEntry(Compiler *compiler) override;

    FunctionNotFoundError getFunctionNotFoundError(Compiler *compiler) override;

    ReturnTypes computeReturnType(Compiler *compiler) override;
};

class Shortcut {
public:
    std::function<bool(Compiler *, BinaryExpr *)> applies;
    std::function<void(Compiler *, BinaryExpr *, std::vector<ByteResolver *> &)> bytesGen;
    std::function<TypeDescriptor *(Compiler *, BinaryExpr *)> returnTypeGen;

    Shortcut(
            std::function<bool(Compiler *, BinaryExpr *)> applies,
            std::function<void(Compiler *, BinaryExpr *, std::vector<ByteResolver *> &bytes)> bytesGen,
            std::function<TypeDescriptor *(Compiler *, BinaryExpr *)> returnTypeGen
    );
};

#endif //RISOTTOV2_BINARYEXPR_H
