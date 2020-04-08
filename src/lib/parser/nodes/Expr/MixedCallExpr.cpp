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

bool MixedCallExpr::isArgumentReference(Compiler *compiler, unsigned int i) {
    return act<bool>(compiler, [i](FunctionTypeDescriptor *functionRef) {
        return functionRef->params[i]->asReference;
    }, [i](FunctionEntry *functionEntry) {
        return functionEntry->descriptor->params[i]->asReference;
    });
}

void MixedCallExpr::loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    act<void>(compiler, [this, compiler, &bytes](FunctionTypeDescriptor *functionRef) {
        loadVariableEntryAddr(compiler, bytes);

        loadOpCall(compiler, bytes);
    }, [this, compiler, &bytes](FunctionEntry *entry) {
        if (auto nativeEntry = dynamic_cast<NativeFunctionEntry *>(entry)) {
            loadOpCallNative(compiler, bytes, nativeEntry);
            return;
        }

        if (auto codeEntry = dynamic_cast<CodeFunctionEntry *>(entry)) {
            loadOpCallBytecode(compiler, bytes, codeEntry);
            return;
        }

        if (auto bytesEntry = dynamic_cast<BytesFunctionEntry *>(entry)) {
            auto genBytes = bytesEntry->get();

            bytes.insert(bytes.end(), genBytes.begin(), genBytes.end());

            return;
        }

        Utils::loadFunctionEntryAddr(compiler, entry, bytes);
        loadOpCall(compiler, bytes);
    });
}

ReturnTypes MixedCallExpr::getFunctionReturnTypes(Compiler *compiler) {
    return act<ReturnTypes>(compiler, [](FunctionTypeDescriptor *desc) {
        return desc->returnTypes;
    }, [](FunctionEntry *entry) {
        return entry->descriptor->returnTypes;
    });
}
