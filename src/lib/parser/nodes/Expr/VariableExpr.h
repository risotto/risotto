//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_VARIABLEEXPR_H
#define RISOTTOV2_VARIABLEEXPR_H


#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/Expr.h>

class VariableExpr: public Expr {
public:
    Token *name;

    explicit VariableExpr(Token *name);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

protected:
    TypeEntry *computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_VARIABLEEXPR_H
