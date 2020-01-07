//
// Created by rvigee on 10/16/19.
//

#ifndef RISOTTOV2_GETCALLEXPR_H
#define RISOTTOV2_GETCALLEXPR_H


#include "MixedCallExpr.h"

class GetCallExpr: public MixedCallExpr {
public:
    Expr *callee;
    Token *op;
    Token *identifier;

    GetCallExpr(Expr *callee, Token *op, Token *identifier, Token *rParen, const std::vector<Expr *> &args);

    std::vector<Expr *> getArguments(Compiler *compiler) override;

    FunctionNotFoundError getFunctionNotFoundError(Compiler *compiler) override;

    void symbolize(Compiler *compiler) override;

protected:
    VariableEntry *getVariableEntry(Compiler *compiler) override;

    FunctionEntry *getFunctionEntry(Compiler *compiler) override;

    void loadVariableEntryAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) override;
};


#endif //RISOTTOV2_GETCALLEXPR_H
