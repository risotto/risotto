//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_SETEXPR_H
#define RISOTTOV2_SETEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>
#include "lib/compiler/TypeReference.h"

class SetExpr: public Expr {
public:
    Expr *left;
    Token *op;
    Expr *right;

    SetExpr(Expr *left, Token *op, Expr *right);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

protected:
    TypeReferences computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_SETEXPR_H
