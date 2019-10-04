//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_BINARYEXPR_H
#define RISOTTOV2_BINARYEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>
#include "CallExpr.h"

class BinaryExpr : public CallExpr {
public:
    BinaryExpr(Expr *left, Token *op, Expr *right);

    FunctionEntry *getFunctionEntry(Compiler *compiler) override;

    Expr *left();

    Expr *right();

    Token *op();
};


#endif //RISOTTOV2_BINARYEXPR_H
