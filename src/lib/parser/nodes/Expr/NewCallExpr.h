//
// Created by rvigee on 10/16/19.
//

#ifndef RISOTTOV2_NEWCALLEXPR_H
#define RISOTTOV2_NEWCALLEXPR_H


#include "IdentifierCallExpr.h"

class NewCallExpr: public IdentifierCallExpr {
public:
    NewCallExpr(Token *identifier, Token *rParen, const std::vector<Expr *> &args);

    std::vector<TypeReference *> getArgumentsTypes(Compiler *compiler) override;

    void loadArgs(Compiler *compiler, std::vector<ByteResolver *> &bytes) override;

    FunctionNotFoundError getFunctionNotFoundError(Compiler *compiler) override;

protected:
    VariableEntry *getVariableEntry(Compiler *compiler) override;

    FunctionEntry *getFunctionEntry(Compiler *compiler) override;

    StructTypeDefinition *getStructTypeDef(Compiler *compiler);

public:
    void walk(ASTWalker *walker) override;
};


#endif //RISOTTOV2_NEWCALLEXPR_H
