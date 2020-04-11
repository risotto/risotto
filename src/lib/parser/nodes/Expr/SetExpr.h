//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_SETEXPR_H
#define RISOTTOV2_SETEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>
#include "lib/compiler/ReturnTypes.h"

class SetExpr: public Expr {
public:
    Expr *left;
    PToken op;
    Expr *right;

    SetExpr(Expr *left, PToken op, Expr *right);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    void symbolize(Compiler *compiler) override;

protected:
    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_SETEXPR_H
