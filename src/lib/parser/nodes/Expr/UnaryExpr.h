//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_UNARYEXPR_H
#define RISOTTOV2_UNARYEXPR_H


#include <lib/tokenizer/Token.h>
#include "CallExpr.h"
#include "GetCallExpr.h"

class UnaryExpr: public GetCallExpr {
public:
    Expr *right();
    PToken op();

    UnaryExpr(PToken op, Expr *right);

    FunctionEntry *getFunctionEntry(Compiler *compiler) override;

protected:
    VariableEntry *getVariableEntry(Compiler *compiler) override;

protected:
    std::vector<Expr *> getArguments(Compiler *compiler) override;

    FunctionNotFoundError getFunctionNotFoundError(Compiler *compiler) override;
};


#endif //RISOTTOV2_UNARYEXPR_H
