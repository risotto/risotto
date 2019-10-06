//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_IDENTIFIEREXPR_H
#define RISOTTOV2_IDENTIFIEREXPR_H


#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/Expr.h>
#include <lib/compiler/utils/ReturnsCandidatesFunctions.h>

class IdentifierExpr: public Expr, public ReturnsCandidatesFunctions {
public:
    Token *name;

    explicit IdentifierExpr(Token *name);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    std::vector<FunctionEntry *> getCandidatesFunctions(Compiler *compiler) override;

    std::string getCandidatesFunctionsFor() override;

protected:
    TypeEntry *computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_IDENTIFIEREXPR_H
