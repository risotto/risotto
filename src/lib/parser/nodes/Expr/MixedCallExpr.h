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

    bool isArgumentReference(Compiler *compiler, unsigned int i) override;

    void loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) override;

    ReturnTypes getFunctionReturnTypes(Compiler *compiler) override;

protected:
    virtual VariableEntry *getVariableEntry(Compiler *compiler) = 0;

    virtual FunctionEntry *getFunctionEntry(Compiler *compiler) = 0;

    virtual void loadVariableEntryAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) = 0;

    ReturnTypes computeReturnType(Compiler *compiler) override;

private:
    enum ActStatus {
        ACT_UNKNOWN, ACT_VARIABLE, ACT_FUNCTION
    };

    ActStatus actStatus = ACT_UNKNOWN;
    FunctionTypeDescriptor *actVariableFunctionDesc = nullptr;
    FunctionEntry *actFunctionEntry = nullptr;
};

template<typename T>
T MixedCallExpr::act(
        Compiler *compiler,
        const std::function<T(FunctionTypeDescriptor *)> &variableActor,
        const std::function<T(FunctionEntry *)> &functionActor
) {
    switch (actStatus) {
        case ACT_UNKNOWN: {
            auto variableEntry = getVariableEntry(compiler);

            if (variableEntry) {
                actVariableFunctionDesc = dynamic_cast<FunctionTypeDescriptor *>(variableEntry->typeRef);

                if (actVariableFunctionDesc) {
                    auto params = std::vector<TypeDescriptor *>();
                    for (const auto &param: actVariableFunctionDesc->params) {
                        params.push_back(param->type);
                    }

                    if (Utils::typesMatch(params, getArgumentsTypes(compiler), Utils::TypesCompatible)) {
                        actStatus = ACT_VARIABLE;
                        return variableActor(actVariableFunctionDesc);
                    } else {
                        throw getFunctionNotFoundError(compiler);
                    }
                } else {
                    throw CompilerError("Variable is not a function");
                }
            }

            actFunctionEntry = getFunctionEntry(compiler);

            if (actFunctionEntry) {
                actStatus = ACT_FUNCTION;
                return functionActor(actFunctionEntry);
            }

            throw getFunctionNotFoundError(compiler);
        }
        case ACT_VARIABLE:
            return variableActor(actVariableFunctionDesc);
        case ACT_FUNCTION:
            return functionActor(actFunctionEntry);
    }
}

#endif //RISOTTOV2_MIXEDCALLEXPR_H
