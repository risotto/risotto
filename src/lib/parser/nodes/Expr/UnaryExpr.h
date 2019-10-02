//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_UNARYEXPR_H
#define RISOTTOV2_UNARYEXPR_H


#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/Expr.h>

class UnaryExpr: public Expr {
public:
    Token *op;
    Expr *right;

    UnaryExpr(Token *op, Expr *right);
};


#endif //RISOTTOV2_UNARYEXPR_H
