//
// Created by rvigee on 10/16/19.
//

#ifndef RISOTTOV2_NEWCALLEXPR_H
#define RISOTTOV2_NEWCALLEXPR_H


#include <lib/compiler/TypeDefinition.h>
#include "IdentifierCallExpr.h"

class NewCallExpr: public IdentifierCallExpr {
public:
    NewCallExpr(PToken identifier, PToken rParen, const std::vector<Expr *> &args);

    std::vector<TypeDescriptor *> getArgumentsTypes(Compiler *compiler) override;

    void loadArgs(Compiler *compiler, std::vector<ByteResolver *> &bytes) override;

    FunctionNotFoundError getFunctionNotFoundError(Compiler *compiler) override;

protected:
    VariableEntry *getVariableEntry(Compiler *compiler) override;

    FunctionEntry *getFunctionEntry(Compiler *compiler) override;

    StructTypeDefinition *getStructTypeDef(Compiler *compiler);
};


#endif //RISOTTOV2_NEWCALLEXPR_H
