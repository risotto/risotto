//
// Created by rvigee on 10/8/19.
//

#ifndef RISOTTOV2_ARRAYEXPR_H
#define RISOTTOV2_ARRAYEXPR_H


#include <lib/parser/nodes/Expr.h>

class ArrayExpr: public Expr {
public:
    TypeDescriptor *type;
    std::vector<Expr *> elements;

    ArrayExpr(TypeDescriptor *type, std::vector<Expr *> elements);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;
protected:
    TypeReferences computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_ARRAYEXPR_H
