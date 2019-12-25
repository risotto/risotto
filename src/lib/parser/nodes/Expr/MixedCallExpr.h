//
// Created by rvigee on 10/15/19.
//

#ifndef RISOTTOV2_MIXEDCALLEXPR_H
#define RISOTTOV2_MIXEDCALLEXPR_H


#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/Expr.h>
#include <lib/compiler/VariablesTable.h>
#include <lib/compiler/FunctionsTable.h>
#include <lib/parser/nodes/TypeDescriptor.h>
#include "CallExpr.h"

class MixedCallExpr : public BaseCallExpr {
public:
    MixedCallExpr(Token *rParen, const std::vector<Expr *> &args);

    template<typename T>
    T act(
            Compiler *compiler,
            const std::function<T(FunctionTypeDescriptor *)> &variableActor,
            const std::function<T(FunctionEntry *)> &functionActor
    );

    bool isArgumentReference(Compiler *compiler, int i) override;

    void loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) override;

protected:
    virtual VariableEntry *getVariableEntry(Compiler *compiler) = 0;

    virtual FunctionEntry *getFunctionEntry(Compiler *compiler) = 0;

    virtual void loadVariableEntryAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) = 0;

    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_MIXEDCALLEXPR_H
