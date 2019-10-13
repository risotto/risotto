//
// Created by rvigee on 10/13/19.
//

#ifndef RISOTTOV2_NEWEXPR_H
#define RISOTTOV2_NEWEXPR_H


#include "CallExpr.h"

class NewExpr: public Expr {
public:
    TypeDescriptor *identifier;

    NewExpr(TypeDescriptor *identifier);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    StructTypeDefinition *getStructDef(Compiler *compiler);

protected:
    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_NEWEXPR_H
