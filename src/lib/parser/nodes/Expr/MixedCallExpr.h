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
#include <lib/compiler/utils/Utils.h>

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

template<typename T>
T MixedCallExpr::act(
        Compiler *compiler,
        const std::function<T(FunctionTypeDescriptor *)> &variableActor,
        const std::function<T(FunctionEntry *)> &functionActor
) {
    auto variableEntry = getVariableEntry(compiler);

    if (variableEntry) {
        if (auto functionRef = dynamic_cast<FunctionTypeDescriptor *>(variableEntry->typeRef)) {
            auto params = std::vector<TypeDescriptor *>();
            for (const auto &param: functionRef->params) {
                params.push_back(param->type);
            }

            if (Utils::typesMatch(params, getArgumentsTypes(compiler), Utils::TypesCompatible)) {
                return variableActor(functionRef);
            }
        }
    }

    auto functionEntry = getFunctionEntry(compiler);

    if (functionEntry) {
        return functionActor(functionEntry);
    }

    throw getFunctionNotFoundError(compiler);
}

#endif //RISOTTOV2_MIXEDCALLEXPR_H
