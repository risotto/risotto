//
// Created by rvigee on 1/7/20.
//

#ifndef RISOTTOV2_SUBSCRIPTEXPR_H
#define RISOTTOV2_SUBSCRIPTEXPR_H


#include <lib/parser/nodes/Expr.h>
#include "MixedCallExpr.h"
#include "GetCallExpr.h"

class SubscriptExpr : public GetCallExpr {
public:
    SubscriptExpr(Expr *expr, Token *rParen, const std::vector<Expr *>& args);

    bool isArgumentReference(Compiler *compiler, int i) override;

    void loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) override;

    FunctionEntry *getFunctionEntry(Compiler *compiler) override;

protected:
    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_SUBSCRIPTEXPR_H
