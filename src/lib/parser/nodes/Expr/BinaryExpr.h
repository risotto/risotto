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

    void walk(ASTWalker *walker) override;

protected:
    VariableEntry *getVariableEntry(Compiler *compiler) override;

protected:
    FunctionNotFoundError getFunctionNotFoundError(Compiler *compiler) override;
};


#endif //RISOTTOV2_BINARYEXPR_H
