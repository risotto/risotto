//
// Created by rvigee on 10/16/19.
//

#ifndef RISOTTOV2_IDENTIFIERCALLEXPR_H
#define RISOTTOV2_IDENTIFIERCALLEXPR_H


#include "MixedCallExpr.h"

class IdentifierCallExpr: public MixedCallExpr {
public:
    const Token *identifier;

    IdentifierCallExpr(const Token *identifier, const Token *rParen, const std::vector<Expr *> &args);

    FunctionNotFoundError getFunctionNotFoundError(Compiler *compiler) override;

protected:
    VariableEntry *getVariableEntry(Compiler *compiler) override;

    FunctionEntry *getFunctionEntry(Compiler *compiler) override;

    void loadVariableEntryAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) override;
};


#endif //RISOTTOV2_IDENTIFIERCALLEXPR_H
