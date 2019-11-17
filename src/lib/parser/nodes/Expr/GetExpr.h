//
// Created by rvigee on 10/6/19.
//

#ifndef RISOTTOV2_GETEXPR_H
#define RISOTTOV2_GETEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"

class GetExpr: public Expr {
public:
    Expr *callee;
    Token *identifier;

    GetExpr(Expr *callee, Token *identifier);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;
protected:
    ReturnTypes computeReturnType(Compiler *compiler) override;

public:
    void walk(ASTWalker *walker) override;
};


#endif //RISOTTOV2_GETEXPR_H
