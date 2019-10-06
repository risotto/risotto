//
// Created by rvigee on 10/6/19.
//

#ifndef RISOTTOV2_GETEXPR_H
#define RISOTTOV2_GETEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/compiler/utils/ReturnsCandidatesFunctions.h>

class GetExpr: public Expr, public ReturnsCandidatesFunctions {
public:
    Expr *callee;
    Token *identifier;

    GetExpr(Expr *callee, Token *identifier);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    std::string getCandidatesFunctionsFor() override;

protected:
    TypesEntries computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_GETEXPR_H
