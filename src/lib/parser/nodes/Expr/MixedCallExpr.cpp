//
// Created by rvigee on 10/15/19.
//

#include "MixedCallExpr.h"

#include <lib/compiler/utils/Utils.h>
#include <lib/compiler/Compiler.h>

MixedCallExpr::MixedCallExpr(Token *rParen, const std::vector<Expr *> &args)
        : BaseCallExpr(rParen, args) {}

ReturnTypes MixedCallExpr::computeReturnType(Compiler *compiler) {
    return act<ReturnTypes>(compiler, [](FunctionTypeReference *functionRef) {
        return ReturnTypes(functionRef->returnTypes);
    }, [](FunctionEntry *functionEntry) {
        return functionEntry->returnTypes;
    });
}

bool MixedCallExpr::isArgumentReference(Compiler *compiler, int i) {
    return act<bool>(compiler, [i](FunctionTypeReference *functionRef) {
        return functionRef->params[i].asReference;
    }, [i](FunctionEntry *functionEntry) {
        return functionEntry->params[i].asReference;
    });
}

template<typename T>
T MixedCallExpr::act(
        Compiler *compiler,
        const std::function<T(FunctionTypeReference *)> &variableActor,
        const std::function<T(FunctionEntry *)> &functionActor
) {
    auto variableEntry = getVariableEntry(compiler);

    if (variableEntry) {
        if (auto functionRef = dynamic_cast<FunctionTypeReference *>(variableEntry->typeRef)) {
            auto params = std::vector<TypeReference *>();
            for (const auto &param: functionRef->params) {
                params.push_back(param.type);
            }

            if (Utils::typesMatch(params, getArgumentsTypes(compiler))) {
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

void MixedCallExpr::loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    auto loadBytes = act<std::vector<ByteResolver *>>(compiler, [this, compiler](FunctionTypeReference *functionRef) {
        auto bytes = std::vector<ByteResolver *>();

        loadVariableEntryAddr(compiler, bytes);

        return bytes;
    }, [compiler](FunctionEntry *functionEntry) {
        auto bytes = std::vector<ByteResolver *>();

        Utils::loadFunctionEntryAddr(compiler, functionEntry, bytes);

        return bytes;
    });

    bytes.insert(bytes.end(), loadBytes.begin(), loadBytes.end());
}

void MixedCallExpr::walk(ASTWalker *walker) {
    walker->walkMixedCallExpr(this);
}
