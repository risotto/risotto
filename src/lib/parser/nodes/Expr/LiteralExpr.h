//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_LITERALEXPR_H
#define RISOTTOV2_LITERALEXPR_H


#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/Expr.h>

class LiteralExpr: public Expr {
public:
    Token *value;

    LiteralExpr(Token *value);
};


#endif //RISOTTOV2_LITERALEXPR_H
