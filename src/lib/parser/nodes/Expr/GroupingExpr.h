//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_GROUPINGEXPR_H
#define RISOTTOV2_GROUPINGEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>
#include "lib/compiler/ReturnTypes.h"

class GroupingExpr: public Expr {
public:
    Expr *expr;
    Token *lParen;
    Token *rParen;

    GroupingExpr(Expr *expr, Token *lParen, Token *rParen);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    void symbolize(Compiler *compiler) override;

protected:
    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_GROUPINGEXPR_H
