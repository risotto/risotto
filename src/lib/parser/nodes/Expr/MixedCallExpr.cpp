//
// Created by rvigee on 10/15/19.
//

#include "MixedCallExpr.h"

#include <lib/compiler/Compiler.h>

MixedCallExpr::MixedCallExpr(const Token *rParen, const std::vector<Expr *> &args)
        : BaseCallExpr(rParen, args) {}

ReturnTypes MixedCallExpr::computeReturnType(Compiler *compiler) {
    return act<ReturnTypes>(compiler, [](FunctionTypeDescriptor *functionRef) {
        return ReturnTypes(functionRef->returnTypes);
    }, [](FunctionEntry *functionEntry) {
        return functionEntry->descriptor->returnTypes;
    });
}

bool MixedCallExpr::isArgumentReference(Compiler *compiler, unsigned int i) {
    return act<bool>(compiler, [i](FunctionTypeDescriptor *functionRef) {
        return functionRef->params[i]->asReference;
    }, [i](FunctionEntry *functionEntry) {
        return functionEntry->descriptor->params[i]->asReference;
    });
}

bool MixedCallExpr::loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    return act<bool>(compiler, [this, compiler, &bytes](FunctionTypeDescriptor *functionRef) {
        loadVariableEntryAddr(compiler, bytes);

        return false;
    }, [compiler, &bytes](FunctionEntry *functionEntry) {
        return Utils::loadFunctionEntryAddr(compiler, functionEntry, bytes);
    });
}

ReturnTypes MixedCallExpr::getFunctionReturnTypes(Compiler *compiler) {
    return act<ReturnTypes>(compiler, [](FunctionTypeDescriptor *desc) {
        return desc->returnTypes;
    }, [](FunctionEntry *entry) {
        return entry->descriptor->returnTypes;
    });
}
