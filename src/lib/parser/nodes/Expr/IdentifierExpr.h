//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_IDENTIFIEREXPR_H
#define RISOTTOV2_IDENTIFIEREXPR_H


#include <lib/parser/nodes/Expr.h>

class IdentifierExpr: public Expr {
public:
    PToken name;

    explicit IdentifierExpr(PToken name);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    void symbolize(Compiler *compiler) override;

protected:
    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_IDENTIFIEREXPR_H
