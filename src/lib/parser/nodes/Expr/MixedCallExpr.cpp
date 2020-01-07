//
// Created by rvigee on 10/15/19.
//

#include "MixedCallExpr.h"

#include <lib/compiler/Compiler.h>

MixedCallExpr::MixedCallExpr(Token *rParen, const std::vector<Expr *> &args)
        : BaseCallExpr(rParen, args) {}

ReturnTypes MixedCallExpr::computeReturnType(Compiler *compiler) {
    return act<ReturnTypes>(compiler, [](FunctionTypeDescriptor *functionRef) {
        return ReturnTypes(functionRef->returnTypes);
    }, [](FunctionEntry *functionEntry) {
        return functionEntry->descriptor->returnTypes;
    });
}

bool MixedCallExpr::isArgumentReference(Compiler *compiler, int i) {
    return act<bool>(compiler, [i](FunctionTypeDescriptor *functionRef) {
        return functionRef->params[i]->asReference;
    }, [i](FunctionEntry *functionEntry) {
        return functionEntry->descriptor->params[i]->asReference;
    });
}

void MixedCallExpr::loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    auto loadBytes = act<std::vector<ByteResolver *>>(compiler, [this, compiler](FunctionTypeDescriptor *functionRef) {
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
