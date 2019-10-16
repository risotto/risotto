//
// Created by rvigee on 10/7/19.
//

#include <lib/compiler/utils/Utils.h>
#include "FunctionExpr.h"
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"
#include "lib/compiler/TypeDefinition.h"

FunctionExpr::FunctionExpr(FunctionStmt *functionStmt) : functionStmt(functionStmt) {}

ReturnTypes FunctionExpr::computeReturnType(Compiler *compiler) {
    functionStmt->getFunctionEntry(compiler);

    auto params = std::vector<FunctionTypeReferenceParameter>();
    for (auto param: functionStmt->parameters) {
        params.emplace_back(param.name->lexeme, param.type->toTypeReference(compiler), param.asReference);
    }

    auto returnTypes = std::vector<TypeReference *>();
    for (auto returnType: functionStmt->returnTypes) {
        returnTypes.push_back(returnType->toTypeReference(compiler));
    }

    return new FunctionTypeReference(params, returnTypes);
}

std::vector<ByteResolver *> FunctionExpr::compile(Compiler *compiler) {
    auto bytes = functionStmt->compile(compiler);

    Utils::loadFunctionEntryAddr(compiler, functionStmt->getFunctionEntry(compiler), bytes);

    return bytes;
}
