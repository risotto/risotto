//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_LOGICALEXPR_H
#define RISOTTOV2_LOGICALEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>

class LogicalExpr: public Expr {
public:
    Expr *left;
    Token *op;
    Expr *right;

    LogicalExpr(Expr *left, Token *op, Expr *right);
};


#endif //RISOTTOV2_LOGICALEXPR_H
