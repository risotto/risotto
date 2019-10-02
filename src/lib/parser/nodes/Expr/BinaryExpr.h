//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_BINARYEXPR_H
#define RISOTTOV2_BINARYEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>

class BinaryExpr : public Expr {
public:
    Expr *left;
    Token *op;
    Expr *right;

    BinaryExpr(Expr *left, Token *op, Expr *right);
};


#endif //RISOTTOV2_BINARYEXPR_H
